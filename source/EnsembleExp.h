#ifndef ENSEMBLE_EXP_H
#define ENSEMBLE_EXP_H

// @includes
#include <iostream>
#include <string>
#include <utility>
#include <fstream>
#include <sys/stat.h>
#include <algorithm>
#include <functional>
#include <ctime>

#include "base/Ptr.h"
#include "base/vector.h"
#include "control/Signal.h"
#include "Evolve/World.h"
#include "Evolve/Resource.h"
#include "Evolve/SystematicsAnalysis.h"
#include "Evolve/World_output.h"
#include "games/Othello8.h"
#include "hardware/EventDrivenGP.h"
#include "tools/BitVector.h"
#include "tools/Random.h"
#include "tools/random_utils.h"
#include "tools/math.h"
#include "tools/string_utils.h"
#include "TestcaseSet.h"
#include "OthelloHW.h"
#include "OthelloLookup.h"
#include "ensemble-config.h"

// @constants
constexpr int TESTCASE_FILE__DARK_ID = 1;
constexpr int TESTCASE_FILE__LIGHT_ID = -1;
constexpr int TESTCASE_FILE__OPEN_ID = 0;

constexpr size_t SGP__TAG_WIDTH = 16;

constexpr size_t TRAIT_ID__MOVE = 0;
constexpr size_t TRAIT_ID__DONE = 1;

constexpr int AGENT_VIEW__ILLEGAL_ID = -1;
constexpr int AGENT_VIEW__OPEN_ID = 0;
constexpr int AGENT_VIEW__SELF_ID = 1;
constexpr int AGENT_VIEW__OPP_ID = 2;

constexpr size_t SELECTION_METHOD_ID__TOURNAMENT = 0;
constexpr size_t SELECTION_METHOD_ID__LEXICASE = 1;

constexpr size_t OTHELLO_BOARD_WIDTH = 8;
constexpr size_t OTHELLO_BOARD_NUM_CELLS = OTHELLO_BOARD_WIDTH * OTHELLO_BOARD_WIDTH;

class EnsembleExp {
public:
  // @aliases
  using othello_t = emp::Othello8;
  using player_t = othello_t::Player;
  using facing_t = othello_t::Facing;
  using othello_idx_t = othello_t::Index;
  // SignalGP-specific type aliases:
  using SGP__hardware_t = emp::EventDrivenGP_AW<SGP__TAG_WIDTH>;
  using SGP__program_t = SGP__hardware_t::Program;
  using SGP__state_t = SGP__hardware_t::State;
  using SGP__inst_t = SGP__hardware_t::inst_t;
  using SGP__inst_lib_t = SGP__hardware_t::inst_lib_t;
  using SGP__event_t = SGP__hardware_t::event_t;
  using SGP__event_lib_t = SGP__hardware_t::event_lib_t;
  using SGP__memory_t = SGP__hardware_t::memory_t;
  using SGP__tag_t = SGP__hardware_t::affinity_t;

  struct SignalGPAgent
  {
    SGP__program_t program;
    size_t agent_id;
    size_t GetID() const { return agent_id; }
    void SetID(size_t id) { agent_id = id; }

    SignalGPAgent(const SGP__program_t &_p)
        : program(_p)
    {
      ;
    }

    SignalGPAgent(const SignalGPAgent &&in)
        :agent_id(in.agent_id), program(in.program)
    {
      ;
    }

    SignalGPAgent(const SignalGPAgent &in)
        : agent_id(in.agent_id), program(in.program)
    {
      ;
    }

    SGP__program_t &GetGenome() { return program; }
  };

  // More aliases
  using phenotype_t = emp::vector<double>;
  using data_t = emp::mut_landscape_info<phenotype_t>;
  using mut_count_t = std::unordered_map<std::string, double>;
  using SGP__world_t = emp::World<SignalGPAgent, data_t>;
  using SGP__genotype_t = SGP__world_t::genotype_t;

protected:
  // == Configurable experiment parameters ==
  // @config_declarations
  // General parameters
  size_t RUN_MODE;
  int RANDOM_SEED;
  size_t POP_SIZE;
  size_t GENERATIONS;
  size_t EVAL_TIME;
  // Selection Group parameters
  size_t SELECTION_METHOD;
  size_t ELITE_SELECT__ELITE_CNT;
  size_t TOURNAMENT_SIZE;
  // Othello Group parameters
  size_t OTHELLO_HW_BOARDS;
  // SignalGP program group parameters
  size_t SGP_FUNCTION_LEN;
  size_t SGP_FUNCTION_CNT;
  size_t SGP_PROG_MAX_LENGTH;
  // SignalGP Hardware Group parameters
  size_t SGP_HW_MAX_CORES;
  size_t SGP_HW_MAX_CALL_DEPTH;
  double SGP_HW_MIN_BIND_THRESH;
  // SignalGP Mutation Group parameters
  int SGP_PROG_MAX_ARG_VAL;
  double SGP_PER_BIT__TAG_BFLIP_RATE;
  double SGP_PER_INST__SUB_RATE;
  bool SGP_VARIABLE_LENGTH;
  double SGP_PER_INST__INS_RATE;
  double SGP_PER_INST__DEL_RATE;
  double SGP_PER_FUNC__FUNC_DUP_RATE;
  double SGP_PER_FUNC__FUNC_DEL_RATE;
  // Data Collection parameters
  size_t SYSTEMATICS_INTERVAL;
  size_t FITNESS_INTERVAL;
  size_t POP_SNAPSHOT_INTERVAL;
  std::string DATA_DIRECTORY;

  // Experiment variables.
  emp::Ptr<emp::Random> random;

  size_t update;                ///< Current update/generation.
  size_t eval_time;             ///< Current evaluation time point (within an agent's turn).
  size_t OTHELLO_MAX_ROUND_CNT; ///< What are the maximum number of rounds in game?
  size_t best_agent_id;

  emp::vector<std::function<double(SignalGPAgent &)>> sgp_lexicase_fit_set; ///< Fit set for SGP lexicase selection.

  emp::Ptr<OthelloHardware> othello_dreamware; ///< Othello game board dreamware!

  OthelloLookup othello_lookup;

  // SignalGP-specifics.
  emp::Ptr<SGP__world_t> sgp_world;         ///< World for evolving SignalGP agents.
  emp::Ptr<SGP__inst_lib_t> sgp_inst_lib;   ///< SignalGP instruction library.
  emp::Ptr<SGP__event_lib_t> sgp_event_lib; ///< SignalGP event library.
  emp::Ptr<SGP__hardware_t> sgp_eval_hw;    ///< Hardware used to evaluate SignalGP programs during evolution/analysis.

  // --- Signals and functors! ---
  // Many of these are hardware-specific.
  // Experiment running/setup signals.
  emp::Signal<void(void)> do_begin_run_setup_sig; ///< Triggered at begining of run. Shared between AGP and SGP
  emp::Signal<void(void)> do_pop_init_sig;        ///< Triggered during run setup. Defines way population is initialized.
  emp::Signal<void(void)> do_evaluation_sig;      ///< Triggered during run step. Should trigger population-wide agent evaluation.
  emp::Signal<void(void)> do_selection_sig;       ///< Triggered during run step. Should trigger selection (which includes selection, reproduction, and mutation).
  emp::Signal<void(void)> do_world_update_sig;    ///< Triggered during run step. Should trigger world->Update(), and whatever else should happen right before/after population turnover.
  // Systematics-specific signals.
  emp::Signal<void(size_t)> do_pop_snapshot_sig;                      ///< Triggered if we should take a snapshot of the population (as defined by POP_SNAPSHOT_INTERVAL). Should call appropriate functions to take snapshot.
  emp::Signal<void(size_t pos, double)> record_fit_sig;               ///< Trigger signal before organism gives birth.
  // Agent evaluation signals.
  emp::Signal<void(const othello_t &)> begin_turn_sig; ///< Called at beginning of agent turn during evaluation.
  emp::Signal<void(void)> agent_advance_sig;           ///< Called during agent's turn. Should cause agent to advance by a single timestep.

  std::function<size_t(void)> get_eval_agent_move;                     ///< Should return eval_hardware's current move selection. Hardware-specific! TODO
  std::function<bool(void)> get_eval_agent_done;                       ///< Should return whether or not eval_hardware is done. Hardware-specific!
  std::function<player_t(void)> get_eval_agent_playerID;               ///< Should return eval_hardware's current playerID. Hardware-specific!
  //std::function<double(test_case_t &, othello_idx_t)> calc_test_score; ///< Given a test case and a move, what is the appropriate score? Shared between hardware types.

  /// Get othello board index given *any* position.
  /// If position can't be used to make an Othello::Index struct, clamp it so that it can.
  othello_idx_t GetOthelloIndex(size_t pos)
  {
    return (pos > OTHELLO_BOARD_NUM_CELLS) ? OTHELLO_BOARD_NUM_CELLS : pos;
  }

  facing_t IntToFacing(int dir)
  {
    dir = emp::Mod(dir, othello_t::NUM_DIRECTIONS);
    switch (dir)
    {
    case 0:
      return facing_t::N;
    case 1:
      return facing_t::NE;
    case 2:
      return facing_t::E;
    case 3:
      return facing_t::SE;
    case 4:
      return facing_t::S;
    case 5:
      return facing_t::SW;
    case 6:
      return facing_t::W;
    case 7:
      return facing_t::NW;
    }
    emp_assert(false);
    return facing_t::N; //< Should never get here.
  }

public:
  EnsembleExp(const EnsembleConfig &config)                                                                                  // @constructor
      : update(0), eval_time(0), OTHELLO_MAX_ROUND_CNT(0), best_agent_id(0) //,
                                                                                                                           // sgp_muller_file(DATA_DIRECTORY + "muller_data.dat"),
                                                                                                                           // agp_muller_file(DATA_DIRECTORY + "muller_data.dat")
  {
    // Localize configs.
    RUN_MODE = config.RUN_MODE();
    RANDOM_SEED = config.RANDOM_SEED();
    POP_SIZE = config.POP_SIZE();
    GENERATIONS = config.GENERATIONS();
    EVAL_TIME = config.EVAL_TIME();
    SELECTION_METHOD = config.SELECTION_METHOD();
    ELITE_SELECT__ELITE_CNT = config.ELITE_SELECT__ELITE_CNT();
    TOURNAMENT_SIZE = config.TOURNAMENT_SIZE();
    OTHELLO_HW_BOARDS = config.OTHELLO_HW_BOARDS();
    SGP_FUNCTION_LEN = config.SGP_FUNCTION_LEN();
    SGP_FUNCTION_CNT = config.SGP_FUNCTION_CNT();
    SGP_PROG_MAX_LENGTH = config.SGP_PROG_MAX_LENGTH();
    SGP_HW_MAX_CORES = config.SGP_HW_MAX_CORES();
    SGP_HW_MAX_CALL_DEPTH = config.SGP_HW_MAX_CALL_DEPTH();
    SGP_HW_MIN_BIND_THRESH = config.SGP_HW_MIN_BIND_THRESH();
    SGP_PROG_MAX_ARG_VAL = config.SGP_PROG_MAX_ARG_VAL();
    SGP_PER_BIT__TAG_BFLIP_RATE = config.SGP_PER_BIT__TAG_BFLIP_RATE();
    SGP_PER_INST__SUB_RATE = config.SGP_PER_INST__SUB_RATE();
    SGP_VARIABLE_LENGTH = config.SGP_VARIABLE_LENGTH();
    SGP_PER_INST__INS_RATE = config.SGP_PER_INST__INS_RATE();
    SGP_PER_INST__DEL_RATE = config.SGP_PER_INST__DEL_RATE();
    SGP_PER_FUNC__FUNC_DUP_RATE = config.SGP_PER_FUNC__FUNC_DUP_RATE();
    SGP_PER_FUNC__FUNC_DEL_RATE = config.SGP_PER_FUNC__FUNC_DEL_RATE();
    FITNESS_INTERVAL = config.FITNESS_INTERVAL();
    POP_SNAPSHOT_INTERVAL = config.POP_SNAPSHOT_INTERVAL();
    DATA_DIRECTORY = config.DATA_DIRECTORY();

    // Make a random number generator.
    random = emp::NewPtr<emp::Random>(RANDOM_SEED);

    // What is the maximum number of rounds for an othello game?
    OTHELLO_MAX_ROUND_CNT = (OTHELLO_BOARD_WIDTH * OTHELLO_BOARD_WIDTH) - 4;

    // Configure the dreamware!
    othello_dreamware = emp::NewPtr<OthelloHardware>(1);

    // Make the world(s)!
    // - SGP World -
    sgp_world = emp::NewPtr<SGP__world_t>(random, "SGP-Ensemble-World");

    // Configure instruction/event libraries.
    sgp_inst_lib = emp::NewPtr<SGP__inst_lib_t>();
    sgp_event_lib = emp::NewPtr<SGP__event_lib_t>();

    // Make data directory.
    mkdir(DATA_DIRECTORY.c_str(), ACCESSPERMS);
    if (DATA_DIRECTORY.back() != '/') DATA_DIRECTORY += '/';
  }

  ~EnsembleExp()
  {
    random.Delete();
    othello_dreamware.Delete();
    sgp_world.Delete();
    sgp_inst_lib.Delete();
    sgp_event_lib.Delete();
    sgp_eval_hw.Delete();
  }

  /// Do a single step of evolution.
  void RunStep()
  {
    do_evaluation_sig.Trigger();   // Update agent scores.
    do_selection_sig.Trigger();    // Do selection (selection, reproduction, mutation).
    do_world_update_sig.Trigger(); // Do world update (population turnover, clear score caches).
  }

  // // Fitness function(s)
  // double CalcFitness(Agent &agent)
  // {
  //   return agent_phen_cache[agent.GetID()].aggregate_score;
  // }

  // Config functions. These do all of the hardware-specific experiment setup/configuration.
  void ConfigSGP();
  void ConfigSGP_InstLib();

  // Mutation functions
  size_t SGP__Mutate_FixedLength(SignalGPAgent &agent, emp::Random &rnd);
  size_t SGP__Mutate_VariableLength(SignalGPAgent &agent, emp::Random &rnd);

  // Population snapshot functions
  void SGP_Snapshot_SingleFile(size_t update);
  void AGP_Snapshot_SingleFile(size_t update);

  // SignalGP utility functions.
  void SGP__InitPopulation_Random();
  void SGP__ResetHW(const SGP__memory_t &main_in_mem = SGP__memory_t());

  // -- SignalGP Instructions --
  // Fork
  void SGP__Inst_Fork(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // BoardWidth
  void SGP_Inst_GetBoardWidth(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // EndTurn
  void SGP_Inst_EndTurn(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // SetMove
  void SGP__Inst_SetMoveXY(SGP__hardware_t &hw, const SGP__inst_t &inst);
  void SGP__Inst_SetMoveID(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // GetMove
  void SGP__Inst_GetMoveXY(SGP__hardware_t &hw, const SGP__inst_t &inst);
  void SGP__Inst_GetMoveID(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // Adjacent
  void SGP__Inst_AdjacentXY(SGP__hardware_t &hw, const SGP__inst_t &inst);
  void SGP__Inst_AdjacentID(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // IsValid
  void SGP__Inst_IsValidXY_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  void SGP__Inst_IsValidID_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // IsValidOpp
  void SGP__Inst_IsValidOppXY_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  void SGP__Inst_IsValidOppID_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // ValidMovesCnt
  void SGP__Inst_ValidMoveCnt_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // ValidOppMovesCnt
  void SGP__Inst_ValidOppMoveCnt_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // GetBoardValue
  void SGP__Inst_GetBoardValueXY_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  void SGP__Inst_GetBoardValueID_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // Place
  void SGP__Inst_PlaceDiskXY_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  void SGP__Inst_PlaceDiskID_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // PlaceOpp
  void SGP__Inst_PlaceOppDiskXY_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  void SGP__Inst_PlaceOppDiskID_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // FlipCnt
  void SGP__Inst_FlipCntXY_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  void SGP__Inst_FlipCntID_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // OppFlipCnt
  void SGP__Inst_OppFlipCntXY_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  void SGP__Inst_OppFlipCntID_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // FrontierCnt
  void SGP__Inst_FrontierCnt_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // ResetBoard
  void SGP__Inst_ResetBoard_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // IsOver
  void SGP__Inst_IsOver_HW(SGP__hardware_t &hw, const SGP__inst_t &inst);
};

// SignalGP Functions
/// Reset the SignalGP evaluation hardware, setting input memory of
/// main thread to be equal to main_in_mem.
void EnsembleExp::SGP__ResetHW(const SGP__memory_t &main_in_mem)
{
  sgp_eval_hw->ResetHardware();
  sgp_eval_hw->SetTrait(TRAIT_ID__MOVE, -1);
  sgp_eval_hw->SetTrait(TRAIT_ID__DONE, 0);
  sgp_eval_hw->SpawnCore(0, main_in_mem, true);
}

void EnsembleExp::SGP__InitPopulation_Random()
{
  std::cout << "Initializing population randomly!" << std::endl;
  for (size_t p = 0; p < POP_SIZE; ++p)
  {
    SGP__program_t prog(sgp_inst_lib);
    for (size_t f = 0; f < SGP_FUNCTION_CNT; ++f)
    {
      prog.PushFunction();
      prog[f].affinity.Randomize(*random);
      for (size_t i = 0; i < SGP_FUNCTION_LEN; ++i)
      {
        const size_t instID = random->GetUInt(sgp_inst_lib->GetSize());
        const size_t a0 = random->GetUInt(0, SGP_PROG_MAX_ARG_VAL);
        const size_t a1 = random->GetUInt(0, SGP_PROG_MAX_ARG_VAL);
        const size_t a2 = random->GetUInt(0, SGP_PROG_MAX_ARG_VAL);
        SGP__inst_t inst(instID, a0, a1, a2);
        inst.affinity.Randomize(*random);
        prog[f].PushInst(inst);
      }
    }
    sgp_world->Inject(prog, 1);
  }
}

size_t EnsembleExp::SGP__Mutate_FixedLength(SignalGPAgent &agent, emp::Random &rnd)
{
  SGP__program_t &program = agent.GetGenome();
  size_t mut_cnt = 0;
  // For each function:
  for (size_t fID = 0; fID < program.GetSize(); ++fID)
  {
    // Mutate affinity.
    for (size_t i = 0; i < program[fID].GetAffinity().GetSize(); ++i)
    {
      SGP__tag_t &aff = program[fID].GetAffinity();
      if (rnd.P(SGP_PER_BIT__TAG_BFLIP_RATE))
      {
        ++mut_cnt;
        aff.Set(i, !aff.Get(i));
      }
    }
    // Substitutions?
    for (size_t i = 0; i < program[fID].GetSize(); ++i)
    {
      SGP__inst_t &inst = program[fID][i];
      // Mutate affinity (even if it doesn't have one).
      for (size_t k = 0; k < inst.affinity.GetSize(); ++k)
      {
        if (rnd.P(SGP_PER_BIT__TAG_BFLIP_RATE))
        {
          ++mut_cnt;
          inst.affinity.Set(k, !inst.affinity.Get(k));
        }
      }
      // Mutate instruction.
      if (rnd.P(SGP_PER_INST__SUB_RATE))
      {
        ++mut_cnt;
        inst.id = rnd.GetUInt(program.GetInstLib()->GetSize());
      }
      // Mutate arguments (even if they aren't relevent to instruction).
      for (size_t k = 0; k < SGP__hardware_t::MAX_INST_ARGS; ++k)
      {
        if (rnd.P(SGP_PER_INST__SUB_RATE))
        {
          ++mut_cnt;
          inst.args[k] = rnd.GetInt(SGP_PROG_MAX_ARG_VAL);
        }
      }
    }
  }
  return mut_cnt;
}

size_t EnsembleExp::SGP__Mutate_VariableLength(SignalGPAgent &agent, emp::Random &rnd)
{
  SGP__program_t &program = agent.GetGenome();
  size_t mut_cnt = 0;
  // Duplicate a function?
  size_t expected_prog_len = program.GetInstCnt();
  size_t old_content_wall = program.GetSize(); ///< First position (or invalid position) after old content.
  size_t fID = 0;
  while (fID < old_content_wall)
  {
    bool dup = rnd.P(SGP_PER_FUNC__FUNC_DUP_RATE);
    bool del = rnd.P(SGP_PER_FUNC__FUNC_DEL_RATE);
    if (dup && del)
    {
      dup = false;
      del = false;
    } // If we're about to dup and del, don't do anything.
    // Do we duplicate?
    if (dup && ((expected_prog_len + program[fID].GetSize()) <= SGP_PROG_MAX_LENGTH))
    {
      // Adjust expected program length (total instructions).
      expected_prog_len += program[fID].GetSize();
      // Duplication.
      program.PushFunction(program[fID]);
      ++mut_cnt;
      // Do we delete?
    }
    else if (del && program.GetSize() > 1)
    {
      // Adjust expected program length (total instructions).
      expected_prog_len -= program[fID].GetSize();
      const size_t mfID = program.GetSize() - 1;
      // Deletion.
      program[fID] = program[mfID];
      program.program.resize(mfID);
      // Should we adjust the wall?
      if (mfID < old_content_wall)
      {
        // We're moving from within the wall, adjust wall.
        --old_content_wall;
        --fID;
      }
      ++mut_cnt;
    }
    ++fID;
  }
  // For each function...
  for (size_t fID = 0; fID < program.GetSize(); ++fID)
  {
    // Mutate affinity
    for (size_t i = 0; i < program[fID].GetAffinity().GetSize(); ++i)
    {
      SGP__tag_t &aff = program[fID].GetAffinity();
      if (rnd.P(SGP_PER_BIT__TAG_BFLIP_RATE))
      {
        ++mut_cnt;
        aff.Set(i, !aff.Get(i));
      }
    }

    // Substitution mutations?
    for (size_t i = 0; i < program[fID].GetSize(); ++i)
    {
      SGP__inst_t &inst = program[fID][i];
      // Mutate affinity (even if it doesn't have one).
      for (size_t k = 0; k < inst.affinity.GetSize(); ++k)
      {
        if (rnd.P(SGP_PER_BIT__TAG_BFLIP_RATE))
        {
          ++mut_cnt;
          inst.affinity.Set(k, !inst.affinity.Get(k));
        }
      }
      // Mutate instruction.
      if (rnd.P(SGP_PER_INST__SUB_RATE))
      {
        ++mut_cnt;
        inst.id = rnd.GetUInt(program.GetInstLib()->GetSize());
      }
      // Mutate arguments (even if they aren't relevent to instruction).
      for (size_t k = 0; k < SGP__hardware_t::MAX_INST_ARGS; ++k)
      {
        if (rnd.P(SGP_PER_INST__SUB_RATE))
        {
          ++mut_cnt;
          inst.args[k] = rnd.GetInt(SGP_PROG_MAX_ARG_VAL);
        }
      }
    }
    // Insertion/deletion mutations?
    // - Compute insertions.
    int num_ins = rnd.GetRandBinomial(program[fID].GetSize(), SGP_PER_INST__INS_RATE);
    // Ensure that insertions don't exceed maximum program length.
    if ((num_ins + expected_prog_len) > SGP_PROG_MAX_LENGTH)
    {
      num_ins = SGP_PROG_MAX_LENGTH - expected_prog_len;
    }
    expected_prog_len += num_ins;

    // Do we need to do any insertions or deletions?
    if (num_ins > 0 || SGP_PER_INST__DEL_RATE > 0.0)
    {
      emp::vector<size_t> ins_locs = emp::RandomUIntVector(rnd, num_ins, 0, program[fID].GetSize());
      if (ins_locs.size())
        std::sort(ins_locs.begin(), ins_locs.end(), std::greater<size_t>());
      SGP__hardware_t::Function new_fun(program[fID].GetAffinity());
      size_t rhead = 0;
      size_t num_dels = 0;
      while (rhead < program[fID].GetSize())
      {
        if (ins_locs.size())
        {
          if (rhead >= ins_locs.back())
          {
            // Insert a random instruction.
            new_fun.PushInst(rnd.GetUInt(program.GetInstLib()->GetSize()),
                              rnd.GetInt(SGP_PROG_MAX_ARG_VAL),
                              rnd.GetInt(SGP_PROG_MAX_ARG_VAL),
                              rnd.GetInt(SGP_PROG_MAX_ARG_VAL),
                              SGP__tag_t());
            new_fun.inst_seq.back().affinity.Randomize(rnd);
            ++mut_cnt;
            ins_locs.pop_back();
            continue;
          }
        }
        // Do we delete this instruction?
        if (rnd.P(SGP_PER_INST__DEL_RATE) && num_dels < (program[fID].GetSize() - 1))
        {
          ++mut_cnt;
          ++num_dels;
          --expected_prog_len;
        }
        else
        {
          new_fun.PushInst(program[fID][rhead]);
        }
        ++rhead;
      }
      program[fID] = new_fun;
    }
  }
  return mut_cnt;
}

void EnsembleExp::SGP_Snapshot_SingleFile(size_t update)
{
  std::string snapshot_dir = DATA_DIRECTORY + "pop_" + emp::to_string((int)update);
  mkdir(snapshot_dir.c_str(), ACCESSPERMS);
  // For each program in the population, dump the full program description in a single file.
  std::ofstream prog_ofstream(snapshot_dir + "/pop_" + emp::to_string((int)update) + ".pop");
  for (size_t i = 0; i < sgp_world->GetSize(); ++i)
  {
    if (i)
      prog_ofstream << "===\n";
    SignalGPAgent &agent = sgp_world->GetOrg(i);
    agent.program.PrintProgramFull(prog_ofstream);
  }
  prog_ofstream.close();
}

#include "EnsembleExp__InstructionImpl.h"

void Run()
{

  std::clock_t base_start_time = std::clock();

  // do_begin_run_setup_sig.Trigger();
  // for (update = 0; update <= GENERATIONS; ++update)
  // {
  //   RunStep();
  //   if (update % POP_SNAPSHOT_INTERVAL == 0)
  //     do_pop_snapshot_sig.Trigger(update);
  // }

  std::clock_t base_tot_time = std::clock() - base_start_time;
  std::cout << "Time = " << 1000.0 * ((double)base_tot_time) / (double)CLOCKS_PER_SEC
            << " ms." << std::endl;

  
}


#endif