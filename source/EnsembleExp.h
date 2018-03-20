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

protected:
  // == Configurable experiment parameters ==
  // @config_declarations
  // General parameters
  size_t RUN_MODE;
  int RANDOM_SEED;
  size_t POP_SIZE;
  size_t GENERATIONS;
  size_t EVAL_TIME;
  size_t REPRESENTATION;
  std::string TEST_CASE_FILE;
  std::string ANCESTOR_FPATH;
  size_t POP_INITIALIZATION_METHOD;
  // Selection Group parameters
  size_t SELECTION_METHOD;
  size_t ELITE_SELECT__ELITE_CNT;
  size_t TOURNAMENT_SIZE;
  size_t RESOURCE_SELECT__MODE;
  double RESOURCE_SELECT__RES_AMOUNT;
  double RESOURCE_SELECT__RES_INFLOW;
  double RESOURCE_SELECT__OUTFLOW;
  double RESOURCE_SELECT__FRAC;
  double RESOURCE_SELECT__MAX_BONUS;
  double RESOURCE_SELECT__COST;
  size_t RESOURCE_SELECT__GAME_PHASE_LEN;
  // Scoring Group parameters
  double SCORE_MOVE__ILLEGAL_MOVE_VALUE;
  double SCORE_MOVE__LEGAL_MOVE_VALUE;
  double SCORE_MOVE__EXPERT_MOVE_VALUE;
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
  // AvidaGP Program group parameters
  size_t AGP_GENOME_SIZE;
  // AvidaGP Mutation Group parameters
  double AGP_PER_INST__SUB_RATE;
  // Data Collection parameters
  size_t SYSTEMATICS_INTERVAL;
  size_t FITNESS_INTERVAL;
  size_t POP_SNAPSHOT_INTERVAL;
  std::string DATA_DIRECTORY;
  // Analysis parameters
  size_t ANALYSIS_TYPE;
  std::string ANALYZE_PROGRAM_FPATH;

  // Experiment variables.
  emp::Ptr<emp::Random> random;

  size_t update;                ///< Current update/generation.
  size_t eval_time;             ///< Current evaluation time point (within an agent's turn).
  size_t OTHELLO_MAX_ROUND_CNT; ///< What are the maximum number of rounds in game?
  size_t best_agent_id;

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

  }

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
};

#endif