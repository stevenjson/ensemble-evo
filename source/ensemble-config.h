#ifndef ENSEMBLE_CONFIG_H
#define ENSEMBLE_CONFIG_H

#include "config/config.h"

EMP_BUILD_CONFIG( EnsembleConfig,
  GROUP(DEFAULT_GROUP, "General Settings"),
  VALUE(RUN_MODE, size_t, 0, "What mode are we running in? \n0: Native experiment\n1: Analyze mode"),
  VALUE(RANDOM_SEED, int, -1, "Random number seed (negative value for based on time)"),
  VALUE(POP_SIZE, size_t, 1000, "Total population size"),
  VALUE(GENERATIONS, size_t, 5000, "How many generations should we run evolution?"),
  VALUE(EVAL_TIME, size_t, 1000, "Agent evaluation time (how much time an agent has on a single turn)"),
  VALUE(REPRESENTATION, size_t, 0, "Which ensemble method are we using? \n0: Individual \n1: Group \n2: Group with Communication"),
  VALUE(GROUP_SIZE, size_t, 5, "How many organisms in an ensemble?"),

  GROUP(SELECTION_GROUP, "Selection Settings"),
  VALUE(SELECTION_METHOD, size_t, 0, "Which selection method are we using? \n0: Tournament\n1: Lexicase\n2: Eco-EA (resource)\n3: MAP-Elites\n4: Roulette"),
  VALUE(ELITE_SELECT__ELITE_CNT, size_t, 1, "How many elites get free reproduction passes?"),
  VALUE(TOURNAMENT_SIZE, size_t, 4, "How big are tournaments when using tournament selection or any selection method that uses tournaments?"),

  GROUP(OTHELLO_GROUP, "Othello-specific Settings"),
  VALUE(OTHELLO_HW_BOARDS, size_t, 1, "How many dream boards are given to agents for them to manipulate?"),

  GROUP(SGP_PROGRAM_GROUP, "SignalGP program Settings"),
  VALUE(SGP_MAX_FUNCTION_LEN, size_t, 50, "Used for generating SGP programs. How long are functions?"),
  VALUE(SGP_MIN_FUNCTION_LEN, size_t, 1, "TODO"),
  VALUE(SGP_MAX_FUNCTION_CNT, size_t, 4, "Used for generating SGP programs. How many functions do we generate?"),
  VALUE(SGP_MIN_FUNC_CNT, size_t, 1, "TODO"),
  VALUE(SGP_PROG_MAX_LENGTH, size_t, 200, "Maximum length of SGP program (used for variable length programs)"),


  GROUP(SGP_HARDWARE_GROUP, "SignalGP Hardware Settings"),
  VALUE(SGP_HW_MAX_CORES, size_t, 16, "Max number of hardware cores; i.e., max number of simultaneous threads of execution hardware will support."),
  VALUE(SGP_HW_MAX_CALL_DEPTH, size_t, 128, "Max call depth of hardware unit"),
  VALUE(SGP_HW_MIN_BIND_THRESH, double, 0.0, "Hardware minimum referencing threshold"),

  GROUP(SGP_MUTATION_GROUP, "SignalGP Mutation Settings"),
  VALUE(SGP_VARIABLE_LENGTH, bool, true, "Are SGP programs variable or fixed length?"),
  VALUE(SGP_PROG_MAX_ARG_VAL, int, 16, "Maximum argument value for instructions."),
  VALUE(SGP_PER_BIT__TAG_BFLIP_RATE, double, 0.005, "Per-bit mutation rate of tag bit flips."),
  VALUE(SGP_PER_INST__SUB_RATE, double, 0.005, "Per-instruction/argument subsitution rate."),
  VALUE(SGP_PER_INST__INS_RATE, double, 0.005, "Per-instruction insertion mutation rate."),
  VALUE(SGP_PER_INST__DEL_RATE, double, 0.005, "Per-instruction deletion mutation rate."),
  VALUE(SGP_PER_FUNC__FUNC_DUP_RATE, double, 0.05, "Per-function rate of function duplications."),
  VALUE(SGP_PER_FUNC__FUNC_DEL_RATE, double, 0.05, "Per-function rate of function deletions."),
  VALUE(SGP_PER_FUNC__SLIP_RATE, double, 0.05, "TODO"),

  GROUP(DATA_GROUP, "Data Collection Settings"),
  VALUE(FITNESS_INTERVAL, size_t, 100, "Interval to record fitness summary stats."),
  VALUE(POP_SNAPSHOT_INTERVAL, size_t, 5000, "Interval to take a full snapshot of the population."),
  VALUE(DATA_DIRECTORY, std::string, "./", "Location to dump data output.")
)

#endif
