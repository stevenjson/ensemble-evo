#ifndef ENSEMBLE_H
#define ENSEMBLE_H

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

constexpr size_t SGP__TAG_WIDTH = 16;

constexpr size_t OTHELLO_BOARD_WIDTH = 8;
constexpr size_t OTHELLO_BOARD_NUM_CELLS = OTHELLO_BOARD_WIDTH * OTHELLO_BOARD_WIDTH;

constexpr size_t TRAIT_ID__MOVE = 0;
constexpr size_t TRAIT_ID__DONE = 1;

constexpr int AGENT_VIEW__ILLEGAL_ID = -1;
constexpr int AGENT_VIEW__OPEN_ID = 0;
constexpr int AGENT_VIEW__SELF_ID = 1;
constexpr int AGENT_VIEW__OPP_ID = 2;

constexpr size_t SELECTION_METHOD_ID__TOURNAMENT = 0;
constexpr size_t SELECTION_METHOD_ID__LEXICASE = 1;

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
        : agent_id(in.agent_id), program(in.program)
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

  struct Phenotype
  {
    emp::vector<double> testcase_scores;
    size_t illegal_move_total;
    size_t valid_move_total;
    size_t expert_move_total;
    double aggregate_score;
  };

  // More aliases
  using phenotype_t = emp::vector<double>;
  using data_t = emp::mut_landscape_info<phenotype_t>;
  using SGP__world_t = emp::World<SignalGPAgent, data_t>;
  using SGP__genotype_t = SGP__world_t::genotype_t;

protected:
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

  emp::Ptr<OthelloHardware> othello_dreamware; ///< Othello game board dreamware!

  size_t update;                ///< Current update/generation.
  size_t eval_time;             ///< Current evaluation time point (within an agent's turn).
  size_t OTHELLO_MAX_ROUND_CNT; ///< What are the maximum number of rounds in game?
  size_t best_agent_id;

  // SignalGP-specifics.
  emp::Ptr<SGP__world_t> sgp_world;         ///< World for evolving SignalGP agents.
  emp::Ptr<SGP__inst_lib_t> sgp_inst_lib;   ///< SignalGP instruction library.
  emp::Ptr<SGP__event_lib_t> sgp_event_lib; ///< SignalGP event library.
  emp::Ptr<SGP__hardware_t> sgp_eval_hw;    ///< Hardware used to evaluate SignalGP programs during evolution/analysis.

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
  EnsembleExp(const EnsembleConfig &config) 
    : update(0), eval_time(0), OTHELLO_MAX_ROUND_CNT(0), best_agent_id(0) {

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

    ConfigSGP();
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

  // Config functions. These do all of the hardware-specific experiment setup/configuration.
  void ConfigSGP();
  void ConfigSGP_InstLib();

  // Mutation functions
  size_t SGP__Mutate_FixedLength(SignalGPAgent &agent, emp::Random &rnd);
  size_t SGP__Mutate_VariableLength(SignalGPAgent &agent, emp::Random &rnd);

  // Population snapshot functions
  void SGP_Snapshot_SingleFile(size_t update);

  void EvalGame();

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

#include "Ensemble_Instructions.h"
#include "ensemble_func.h"



#endif