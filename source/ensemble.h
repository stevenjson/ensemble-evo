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
#include "OthelloHW.h"
#include "ensemble-config.h"
#include "../othelloAI/game.h"

// SignalGP Specific Constants
constexpr size_t SGP__TAG_WIDTH = 16;

// Othello Specific Constants
constexpr size_t OTHELLO_BOARD_WIDTH = 8;
constexpr size_t OTHELLO_BOARD_NUM_CELLS = OTHELLO_BOARD_WIDTH * OTHELLO_BOARD_WIDTH;

// Program Representation Options
constexpr size_t REPRESENTATION_ID__SIGNALGP = 0;
constexpr size_t REPRESENTATION_ID__SIGNALGPGROUP = 1;

// Coordinator Representation Options
constexpr size_t COORDINATOR_REP_NONE = 0;
constexpr size_t COORDINATOR_REP_FIRST = 1;
constexpr size_t COORDINATOR_REP_ALL = 2;
constexpr size_t COORDINATOR_REP_SPECIAL = 3;

// Instruction Knockout types
constexpr size_t INST_KO_NONE = 0;
constexpr size_t INST_KO_MULTI = 1;
constexpr size_t INST_KO_CONF = 2;
constexpr size_t INST_KO_COMM = 3;

// Agent trait locations
constexpr size_t TRAIT_ID__MOVE = 0;
constexpr size_t TRAIT_ID__DONE = 1;
constexpr size_t TRAIT_ID__GID = 2;
constexpr size_t TRAIT_ID__LOC = 3;
constexpr size_t TRAIT_ID__CONF = 4;
constexpr size_t TRAIT_ID__CAST = 5;
constexpr size_t TRAIT_ID__INVALID = 6;

// Population Initialization Method Options 
constexpr size_t INIT_RANDOM = 0;
constexpr size_t INIT_ANCESTOR = 1;

// Constants for how evolving agents should see Othello boards
constexpr int AGENT_VIEW__ILLEGAL_ID = -1;
constexpr int AGENT_VIEW__OPEN_ID = 0;
constexpr int AGENT_VIEW__SELF_ID = 1;
constexpr int AGENT_VIEW__OPP_ID = 2;

// Selection Method Options
constexpr size_t SELECTION_METHOD_ID__TOURNAMENT = 0;
constexpr size_t SELECTION_METHOD_ID__LEXICASE = 1;


// Master class that runs the entire experiment and keeps track of config settings
class EnsembleExp {
// Aliases and Wrapper Structs
public:
  // Othello type aliases
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

  /// Agent structure to be used to wrap organisms
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

  /// Agent structure to be used to wrap ensembles
  struct GroupSignalGPAgent
  {
    emp::vector<SGP__program_t> programs;
    size_t agent_id;
    size_t GetID() const { return agent_id; }
    void SetID(size_t id) { agent_id = id; }

    GroupSignalGPAgent(const emp::vector<SGP__program_t> &_p)
        : programs(_p)
    {
      ;
    }

    GroupSignalGPAgent(const GroupSignalGPAgent &&in)
        : agent_id(in.agent_id), programs(in.programs)
    {
      ;
    }

    GroupSignalGPAgent(const GroupSignalGPAgent &in)
        : agent_id(in.agent_id), programs(in.programs)
    {
      ;
    }

    emp::vector<SGP__program_t> &GetGenome() { return programs; }
  };

  /// Struct to keep track of fitness for all heuristic functions
  struct Phenotype
  {
    emp::vector<double> heuristic_scores;
    size_t illegal_move_total;
    double aggregate_score;
    double median_score;
  };

  // Aliases for defined structs
  using phenotype_t = emp::vector<double>;
  using data_t = emp::mut_landscape_info<phenotype_t>;
  using SGP__world_t = emp::World<SignalGPAgent, data_t>;
  using SGPG__world_t = emp::World<GroupSignalGPAgent, data_t>;
  using SGP__genotype_t = SGP__world_t::genotype_t;

// Declaring Member Variables and board navigation functions
protected:
  // General parameters
  size_t RUN_MODE;
  int RANDOM_SEED;
  size_t POP_SIZE;
  size_t GENERATIONS;
  size_t EVAL_TIME;
  size_t NUM_GAMES;
  size_t REPRESENTATION;
  std::string ANCESTOR_FPATH;
  size_t INIT_METHOD;
  // Proogram Competition Parameters
  size_t COMPETE_TYPE;
  std::string COMPETE_FPATH_1;
  std::string COMPETE_FPATH_2;
  double TIMEOUT;
  int AGENT_KO;
  size_t INST_KO;
  // Ensemble Group parameters
  size_t GROUP_SIZE;
  bool COMMUNICATION;
  bool CONFIDENCE;
  bool MULTIVOTE;
  size_t PENALTY;
  size_t COORDINATOR;
  // Selection Group parameters
  size_t SELECTION_METHOD;
  size_t ELITE_SELECT__ELITE_CNT;
  size_t TOURNAMENT_SIZE;
  // Othello Group parameters
  size_t OTHELLO_HW_BOARDS;
  // SignalGP program group parameters
  size_t SGP_MAX_FUNCTION_LEN;
  size_t SGP_MIN_FUNCTION_LEN;
  size_t SGP_MAX_FUNCTION_CNT;
  size_t SGP_MIN_FUNC_CNT;
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
  double SGP_PER_FUNC__SLIP_RATE;
  // Data Collection parameters
  size_t SYSTEMATICS_INTERVAL;
  size_t FITNESS_INTERVAL;
  size_t POP_SNAPSHOT_INTERVAL;
  std::string DATA_DIRECTORY;

  emp::Ptr<emp::Random> random;

  // Othello Player Types
  emp::Othello8::Player dark = emp::Othello8::Player::DARK;
  emp::Othello8::Player light = emp::Othello8::Player::LIGHT;

  // Expirement hardware
  emp::Ptr<OthelloHardware> othello_dreamware;          ///< Othello game board dreamware!
  emp::vector<emp::Ptr<OthelloHardware>> all_dreamware; ///< Ensemble Othello game board dreamware!
  emp::Ptr<SGP__hardware_t> sgp_eval_hw;                ///< Hardware used to evaluate SignalGP programs during evolution/analysis.
  emp::vector <emp::Ptr<SGP__hardware_t>> sgpg_eval_hw; ///< Hardware used to evaluate Ensembles during evolution/analysis.
  emp::Ptr<othello_t> game_hw;                          ///< Hardware used to evaluate games during fitness calculation
  emp::Ptr<othello_t> test_hw;                          ///< Hardware used to run heuristic functions

  // Expirement variables
  size_t update;                ///< Current update/generation.
  size_t eval_time;             ///< Current evaluation time point (within an agent's turn).
  size_t OTHELLO_MAX_ROUND_CNT; ///< What are the maximum number of rounds in game?
  size_t best_agent_id;         ///< What is the id of the current best organism?
  size_t vote_penalties;
  double h_bonus;
  int coordinator_id;

  /// Fitness vectors
  emp::vector<Phenotype> agent_phen_cache;                                        ///< Cache for organims fitness.
  emp::vector<std::function<othello_idx_t()>> heuristics;                         ///< Heuristic functions for fitness evaluation.
  emp::vector<std::function<double(SignalGPAgent &)>> sgp_lexicase_fit_set;       ///< Fit set for SGP lexicase selection.
  emp::vector<std::function<double(GroupSignalGPAgent &)>> sgpg_lexicase_fit_set; ///< Fit set for SGP lexicase selection.
  emp::array<size_t, OTHELLO_BOARD_NUM_CELLS + 1> agent_votes = {};
  emp::array<size_t, OTHELLO_BOARD_NUM_CELLS + 1> h_choices = {};

  // SignalGP-specifics.
  emp::Ptr<SGP__world_t> sgp_world;         ///< World for evolving SignalGP agents.
  emp::Ptr<SGPG__world_t> sgpg_world;       ///< World for evolving Group SignalGP agents.
  emp::Ptr<SGP__inst_lib_t> sgp_inst_lib;   ///< SignalGP instruction library.
  emp::Ptr<SGP__inst_lib_t> coord_inst_lib;   ///< SignalGP Coordinator instruction library.
  emp::Ptr<SGP__event_lib_t> sgp_event_lib; ///< SignalGP event library.

  // Systematics-specific signals for data tracking.
  emp::Signal<void(size_t)> do_pop_snapshot_sig;                      ///< Triggered if we should take a snapshot of the population (as defined by POP_SNAPSHOT_INTERVAL). Should call appropriate functions to take snapshot.
  emp::Signal<void(size_t pos, double)> record_fit_sig;               ///< Trigger signal before organism gives birth that records fitness.
  emp::Signal<void(size_t pos, const phenotype_t &)> record_phen_sig; ///< Trigger signal before organism gives birth that records phenotypic info.
  emp::Signal<void(void)> do_pop_init_sig;                            ///< Triggered during run setup. Defines way population is initialized.
  emp::Signal<void(void)> do_evaluation_sig;                          ///< Triggered during run step. Should trigger population-wide agent evaluation.
  emp::Signal<void(void)> do_selection_sig;                           ///< Triggered during run step. Should trigger selection (which includes selection, reproduction, and mutation).
  emp::Signal<void(void)> do_world_update_sig;                        ///< Triggered during run step. Should trigger world->Update(), and whatever else should happen right before/after population turnover.

  /// Get othello board index given *any* position.
  /// If position can't be used to make an Othello::Index struct, clamp it so that it can (becomes 64 if invalid).
  othello_idx_t GetOthelloIndex(size_t pos)
  {
    return (pos > OTHELLO_BOARD_NUM_CELLS) ? OTHELLO_BOARD_NUM_CELLS : pos;
  }

  /// Converts a numeric direction to an othello game facing.
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

// Experiment Method Declarations (with some definitions)
public:
  /// Constructor for the expirement.
  /// param: config, the configured parameters for the expirement
  EnsembleExp(const EnsembleConfig &config) 
    : update(0), eval_time(0), OTHELLO_MAX_ROUND_CNT(0), best_agent_id(0) {

    // Localize configs.
    RUN_MODE = config.RUN_MODE();
    RANDOM_SEED = config.RANDOM_SEED();
    POP_SIZE = config.POP_SIZE();
    GENERATIONS = config.GENERATIONS();
    EVAL_TIME = config.EVAL_TIME();
    NUM_GAMES = config.NUM_GAMES();
    REPRESENTATION = config.REPRESENTATION();
    ANCESTOR_FPATH = config.ANCESTOR_FPATH();
    INIT_METHOD = config.INIT_METHOD();
    COMPETE_TYPE = config.COMPETE_TYPE();
    COMPETE_FPATH_1 = config.COMPETE_FPATH_1();
    COMPETE_FPATH_2 = config.COMPETE_FPATH_2();
    AGENT_KO = config.AGENT_KO();
    TIMEOUT = config.TIMEOUT();
    INST_KO = config.INST_KO();
    GROUP_SIZE = config.GROUP_SIZE();
    COMMUNICATION = config.COMMUNICATION();
    CONFIDENCE = config.CONFIDENCE();
    MULTIVOTE = config.MULTIVOTE();
    PENALTY = config.PENALTY();
    COORDINATOR = config.COORDINATOR();
    SELECTION_METHOD = config.SELECTION_METHOD();
    ELITE_SELECT__ELITE_CNT = config.ELITE_SELECT__ELITE_CNT();
    TOURNAMENT_SIZE = config.TOURNAMENT_SIZE();
    OTHELLO_HW_BOARDS = config.OTHELLO_HW_BOARDS();
    SGP_MAX_FUNCTION_LEN = config.SGP_MAX_FUNCTION_LEN();
    SGP_MIN_FUNCTION_LEN = config.SGP_MIN_FUNCTION_LEN();
    SGP_MAX_FUNCTION_CNT = config.SGP_MAX_FUNCTION_CNT();
    SGP_MIN_FUNC_CNT = config.SGP_MIN_FUNC_CNT();
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
    SGP_PER_FUNC__SLIP_RATE = config.SGP_PER_FUNC__SLIP_RATE();
    FITNESS_INTERVAL = config.FITNESS_INTERVAL();
    POP_SNAPSHOT_INTERVAL = config.POP_SNAPSHOT_INTERVAL();
    DATA_DIRECTORY = config.DATA_DIRECTORY();

    // Make a random number generator.
    random = emp::NewPtr<emp::Random>(RANDOM_SEED);

    // What is the maximum number of rounds for an othello game?
    OTHELLO_MAX_ROUND_CNT = (OTHELLO_BOARD_WIDTH * OTHELLO_BOARD_WIDTH) - 4;

    ConfigHeuristics(); // Add defined heuristics to class

    // Initialize fitness caching
    agent_phen_cache.resize(POP_SIZE);
    for (size_t i = 0; i < agent_phen_cache.size(); ++i)
    {
      agent_phen_cache[i].heuristic_scores.resize(NUM_GAMES);
      agent_phen_cache[i].illegal_move_total = 0;
      agent_phen_cache[i].aggregate_score = 0;
    }

    // Configure the dreamware!
    for (size_t i = 0; i < GROUP_SIZE; ++i)
    {
      all_dreamware.push_back(emp::NewPtr<OthelloHardware>(1));
    }
    othello_dreamware = all_dreamware[0];

    // Configure game evaluation hardware.
    game_hw = emp::NewPtr<othello_t>();
    test_hw = emp::NewPtr<othello_t>();

    // Make the worlds
    sgp_world = emp::NewPtr<SGP__world_t>(random, "SGP-Ensemble-World");
    sgpg_world = emp::NewPtr<SGPG__world_t>(random, "SGP-Group-Ensemble-World");

    // Initialize instruction/event libraries.
    sgp_inst_lib = emp::NewPtr<SGP__inst_lib_t>();
    coord_inst_lib = emp::NewPtr<SGP__inst_lib_t>();
    sgp_event_lib = emp::NewPtr<SGP__event_lib_t>();

    // Make data directory.
    mkdir(DATA_DIRECTORY.c_str(), ACCESSPERMS);
    if (DATA_DIRECTORY.back() != '/') DATA_DIRECTORY += '/';

    // Configure agent evaluation hardware
    sgp_eval_hw = emp::NewPtr<SGP__hardware_t>(sgp_inst_lib, sgp_event_lib, random);
    sgp_eval_hw->SetMinBindThresh(SGP_HW_MIN_BIND_THRESH);
    sgp_eval_hw->SetMaxCores(SGP_HW_MAX_CORES);
    sgp_eval_hw->SetMaxCallDepth(SGP_HW_MAX_CALL_DEPTH);

    for (size_t i = 0; i < GROUP_SIZE; ++i)
    {
      emp::Ptr<SGP__hardware_t> temp;
      if (COORDINATOR == COORDINATOR_REP_SPECIAL && i == 0)
      {
        temp = emp::NewPtr<SGP__hardware_t>(coord_inst_lib, sgp_event_lib, random);
      }
      else{
        temp = emp::NewPtr<SGP__hardware_t>(sgp_inst_lib, sgp_event_lib, random);
      }
      
      temp->SetMinBindThresh(SGP_HW_MIN_BIND_THRESH);
      temp->SetMaxCores(SGP_HW_MAX_CORES);
      temp->SetMaxCallDepth(SGP_HW_MAX_CALL_DEPTH);

      sgpg_eval_hw.push_back(temp);
    }

    ConfigSGP_InstLib(); // Configure instruction/Event libraries

    switch (REPRESENTATION) 
    {
      case REPRESENTATION_ID__SIGNALGP:
        ConfigSGP();
        break;

      case REPRESENTATION_ID__SIGNALGPGROUP:
        emp_assert(POP_SIZE % GROUP_SIZE == 0);
        POP_SIZE = POP_SIZE / GROUP_SIZE;
        ConfigSGPG();
        break;

      default:
        std::cout << "Unrecognized representation configuration setting (" << REPRESENTATION << "). Exiting..." << std::endl;
        exit(-1);
    }

    switch (COORDINATOR) 
    {
      case COORDINATOR_REP_NONE:
        if (REPRESENTATION == REPRESENTATION_ID__SIGNALGPGROUP) do_evaluation_sig.AddAction([this]() { this->EvaluateGroup(); });
        coordinator_id = -1;
        break;

      case COORDINATOR_REP_FIRST:
        emp_assert(REPRESENTATION == REPRESENTATION_ID__SIGNALGPGROUP);
        sgp_inst_lib->AddInst("GetCoordinator",
                              [this](SGP__hardware_t &hw, const SGP__inst_t &inst) {
                                SGP__state_t &state = hw.GetCurState();
                                state.SetLocal(inst.args[0], coordinator_id);
                              },
                              1, "Returns the location of the current coordinator in the ensemble");
        coordinator_id = 0;
        do_evaluation_sig.AddAction([this]() { this->EvaluateGroup(); });
        break;

      case COORDINATOR_REP_ALL:
        emp_assert(REPRESENTATION == REPRESENTATION_ID__SIGNALGPGROUP);
        do_evaluation_sig.AddAction([this]() { this->EvaluateAll(); });
        sgp_inst_lib->AddInst("GetCoordinator",
                              [this](SGP__hardware_t &hw, const SGP__inst_t &inst) {
                                SGP__state_t &state = hw.GetCurState();
                                state.SetLocal(inst.args[0], coordinator_id);
                              },
                              1, "Returns the location of the current coordinator in the ensemble");
        coordinator_id = 0;
        break;

      case COORDINATOR_REP_SPECIAL:
        emp_assert(REPRESENTATION == REPRESENTATION_ID__SIGNALGPGROUP);
        do_evaluation_sig.AddAction([this]() { this->EvaluateGroup(); });
        ConfigCoordinatorLib();
        coordinator_id = 0;
        break;

      default:
        std::cout << "Coordinator Configuration (" << COORDINATOR << ") is invalid. Exiting..." << std::endl;
        exit(-1);
    }

    if (COMMUNICATION)
    {
      emp_assert(REPRESENTATION == REPRESENTATION_ID__SIGNALGPGROUP);
      ConfigCommunicationLib();
    }

    if (CONFIDENCE)
    {
      ConfigConfidenceLib();
    }

    if (MULTIVOTE)
    {
      sgp_inst_lib->AddInst("CastVote",
                            [this](SGP__hardware_t &hw, const SGP__inst_t &inst) { this->SGP__Inst_CastVote(hw, inst); },
                            0, "Adds the current set move and confidence to total votes");
      
      coord_inst_lib->AddInst("CastVote",
                            [this](SGP__hardware_t &hw, const SGP__inst_t &inst) { this->SGP__Inst_CastVote(hw, inst); },
                            0, "Adds the current set move and confidence to total votes");
    }
    else
    {
      sgp_inst_lib->AddInst("CastVote",
                            [this](SGP__hardware_t &hw, const SGP__inst_t &inst) { 
                              this->SGP__Inst_CastVote(hw, inst);
                              this->SGP_Inst_EndTurn(hw, inst); 
                            },
                            0, "Ends the agents turn");

      coord_inst_lib->AddInst("CastVote",
                            [this](SGP__hardware_t &hw, const SGP__inst_t &inst) {
                              this->SGP__Inst_CastVote(hw, inst);
                              this->SGP_Inst_EndTurn(hw, inst);
                            },
                            0, "Ends the agents turn");
    }
    std::cout<<"Configured."<<std::endl;
  }

  /// Destructor for the expirement.
  ~EnsembleExp()
  {
    random.Delete();
    sgp_world.Delete();
    sgpg_world.Delete();
    sgp_inst_lib.Delete();
    coord_inst_lib.Delete();
    sgp_event_lib.Delete();
    sgp_eval_hw.Delete();
	  game_hw.Delete();
	  test_hw.Delete();
    for (auto ptr : sgpg_eval_hw) {ptr.Delete();}
    for (auto ptr : all_dreamware) {ptr.Delete();}
  }

  /// Fitness function for cached fitness of individual agents
  double CalcFitness(SignalGPAgent &agent)
  {
    return agent_phen_cache[agent.GetID()].median_score;
  }

  /// Fitness function for cached fitness of ensembles
  double CalcFitness(GroupSignalGPAgent &agent)
  {
    return agent_phen_cache[agent.GetID()].median_score;
  }

  /// Record phenotypic performance of best agent/ensemble in population 
  template <typename WORLD_TYPE>
  emp::DataFile &AddBestPhenotypeFile(WORLD_TYPE &world, const std::string &fpath = "best_phenotype.csv")
  {
    auto &file = world.SetupFile(fpath);

    std::function<size_t(void)> get_update = [&world]() { return world.GetUpdate(); };
    file.AddFun(get_update, "update", "Update");

    // // NOTE: when this gets called, world->GetOrg(best_agent_id) is no longer accurate.
    // //       But... agent_phen_cache is still good to go.
    for (size_t i = 0; i < NUM_GAMES; ++i)
    {
      std::function<double(void)> get_score = [&world, this, i]() {
        Phenotype &best_phen = this->agent_phen_cache[this->best_agent_id];
        return best_phen.heuristic_scores[i];
      };
      file.AddFun(get_score, "Game_" + std::to_string(i), "get best phenotype score from this update");
    }

    // std::function<double(void)> get_random_second_score = [&world, this]() {
    //   Phenotype &best_phen = this->agent_phen_cache[this->best_agent_id];
    //   return best_phen.heuristic_scores[1];
    // };
    // file.AddFun(get_random_second_score, "RandomSecond", "get best phenotype score from this update");

    // std::function<double(void)> get_greedy_score = [&world, this]() {
    //   Phenotype &best_phen = this->agent_phen_cache[this->best_agent_id];
    //   return best_phen.heuristic_scores[2];
    // };
    // file.AddFun(get_greedy_score, "Greedy", "get best phenotype score from this update");

    // std::function<double(void)> get_corner_score = [&world, this]() {
    //   Phenotype &best_phen = this->agent_phen_cache[this->best_agent_id];
    //   return best_phen.heuristic_scores[3];
    //   ;
    // };
    // file.AddFun(get_corner_score, "Corner", "get best phenotype score from this update");

    // std::function<double(void)> get_frontier_score = [&world, this]() {
    //   Phenotype &best_phen = this->agent_phen_cache[this->best_agent_id];
    //   return best_phen.heuristic_scores[4];
    // };
    // file.AddFun(get_frontier_score, "Frontier", "get best phenotype score from this update");

    // std::function<double(void)> get_defense_score = [&world, this]() {
    //   Phenotype &best_phen = this->agent_phen_cache[this->best_agent_id];
    //   return best_phen.heuristic_scores[5];
    // };
    // file.AddFun(get_defense_score, "Defense", "get best phenotype score from this update");
    file.PrintHeaderKeys();
    return file;
  }

  // -- Declaration of methods defined in ensemble_func.h --
  // General Functions to manage the evolution
  void Run();
  void RunStep();
  void RunSetup();
  void ResetHardware();
  void ResetHardwareGroup();

  // Functions to manage othello games
  double EvalGame(SignalGPAgent &agent, SignalGPAgent &opp, bool start_player);
  double EvalGameGroup(GroupSignalGPAgent &agent, GroupSignalGPAgent &opp, bool start_player);
  othello_idx_t EvalMove(SignalGPAgent &agent);
  othello_idx_t EvalMoveGroup(GroupSignalGPAgent &agent);
  othello_idx_t EvalMoveAI(Game *game);
  
  // Functions to manage competition of evolved agents/ensembles
  void Compete();
  void AgentKnockout(GroupSignalGPAgent &ensemble, size_t ko_idx);
  Board::Move ConvertToMoveAI(Game *game, othello_idx_t move);
  emp::vector<SGP__program_t> LoadGroupCompete(std::string path);
  SGP__program_t LoadIndividualCompete(std::string path);

  // Functions run in each step of evolution
  void Evaluate();
  void EvaluateAll();
  void Selection();
  void EvaluateGroup();
  void SelectionGroup();

  // Config functions. These do all of the hardware-specific experiment setup/configuration.
  void ConfigSGP();
  void ConfigSGPG();
  void ConfigSGP_InstLib();
  void ConfigCommunicationLib();
  void ConfigCoordinatorLib();
  void ConfigConfidenceLib();
  void ConfigHeuristics();

  // Mutation functions
  size_t SGP__Mutate_FixedLength(SignalGPAgent &agent, emp::Random &rnd);
  size_t SGP__Mutate_VariableLength(SignalGPAgent &agent, emp::Random &rnd);
  size_t SGPG__Mutate_FixedLength(GroupSignalGPAgent &agent, emp::Random &rnd);
  size_t SGPG__Mutate_VariableLength(GroupSignalGPAgent &agent, emp::Random &rnd);

  // Population snapshot functions (writes genomes of current population to file)
  void SGP_Snapshot_SingleFile(size_t update);
  void SGPG_Snapshot_SingleFile(size_t update);

  // SignalGP utility functions.
  void SGP__InitPopulation_Random();
  void SGP__InitPopulation_FromAncestorFile();
  void SGPG__InitPopulation_Random();
  void SGPG__InitPopulation_FromAncestorFile();
  void SGP__ResetHW(const SGP__memory_t &main_in_mem = SGP__memory_t());
  void SGPG__ResetHW(const SGP__memory_t &main_in_mem = SGP__memory_t());

  // -- Declarations of SignalGP Instructions defined in Ensemble_Instructions.h --
  // Event-based communication instructions
  static void Inst_SendMsgFacing(SGP__hardware_t &hw, const SGP__inst_t &inst);
  static void Inst_BroadcastMsg(SGP__hardware_t &hw, const SGP__inst_t &inst);
  static void Inst_SetFace(SGP__hardware_t &hw, const SGP__inst_t &inst);
  static void Inst_GetFace(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // Event library instructions for communication
  static void HandleEvent_MessageForking(SGP__hardware_t &hw, const SGP__event_t &event);
  void EventDriven__DispatchMessageFacing(SGP__hardware_t &hw, const SGP__event_t &event);
  void EventDriven__DispatchMessageBroadcast(SGP__hardware_t &hw, const SGP__event_t &event);

  // Fork
  void SGP__Inst_Fork(SGP__hardware_t &hw, const SGP__inst_t &inst);
  // Cast Vote
  void SGP__Inst_CastVote(SGP__hardware_t &hw, const SGP__inst_t &inst);
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
  // Move Confidence
  void SGP__Inst_SetMoveConfidence(SGP__hardware_t &hw, const SGP__inst_t &inst);
  void SGP__Inst_GetMoveConfidence(SGP__hardware_t &hw, const SGP__inst_t &inst);
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