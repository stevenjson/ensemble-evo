#ifndef ENSEMBLE_FUNC_H
#define ENSEMBLE_FUNC_H
#include "ensemble.h"

void EnsembleExp::ConfigSGP() {
  // Configure the world.
  sgp_world->Reset();
  sgp_world->SetWellMixed(true);

  // Setup mutation function.
  if (SGP_VARIABLE_LENGTH)
  {
    sgp_world->SetMutFun([this](SignalGPAgent &agent, emp::Random &rnd) { return this->SGP__Mutate_VariableLength(agent, rnd); }, ELITE_SELECT__ELITE_CNT);
  }
  else
  {
    // NOTE: second argument specifies that we're not mutating the first thing int the pop (we're doing elite selection in all of our stuff).
    sgp_world->SetMutFun([this](SignalGPAgent &agent, emp::Random &rnd) { return this->SGP__Mutate_FixedLength(agent, rnd); }, ELITE_SELECT__ELITE_CNT);
  }

  ConfigSGP_InstLib();

  sgp_eval_hw = emp::NewPtr<SGP__hardware_t>(sgp_inst_lib, sgp_event_lib, random);
  sgp_eval_hw->SetMinBindThresh(SGP_HW_MIN_BIND_THRESH);
  sgp_eval_hw->SetMaxCores(SGP_HW_MAX_CORES);
  sgp_eval_hw->SetMaxCallDepth(SGP_HW_MAX_CALL_DEPTH);

  // do_pop_snapshot
  do_pop_snapshot_sig.AddAction([this](size_t update) { this->SGP_Snapshot_SingleFile(update); });
}

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

void EnsembleExp::ConfigHeuristics()
{
  std::cout<<"Configuring Heuristic Functions!"<<std::endl;

  std::function<othello_idx_t(SignalGPAgent &)> random_player = [this](SignalGPAgent &agent) {
    emp::vector<othello_idx_t> options = game_hw->GetMoveOptions();
    return options[random->GetUInt(0, options.size())];
  };

  // TODO Add the rest of the heuristic functions here

  heuristics.push_back(random_player);

  // TODO Put other heuristic functions in vector

}

void EnsembleExp::RunSetup()
{
  std::cout << "Doing initial run setup." << std::endl;

  // Setup fitness tracking.
  auto &fit_file = sgp_world->SetupFitnessFile(DATA_DIRECTORY + "fitness.csv");
  fit_file.SetTimingRepeat(FITNESS_INTERVAL);
  record_fit_sig.AddAction([this](size_t pos, double fitness) { sgp_world->GetGenotypeAt(pos)->GetData().RecordFitness(fitness); });

  // Generate the initial population.
  SGP__InitPopulation_Random();

  ConfigHeuristics();
}

/// Do a single step of evolution.
void EnsembleExp::RunStep()
{
  Evaluate();   // Update agent scores.
  // do_selection_sig.Trigger();    // Do selection (selection, reproduction, mutation).
  // do_world_update_sig.Trigger(); // Do world update (population turnover, clear score caches).
}

void EnsembleExp::Run()
{
  std::clock_t base_start_time = std::clock();

  RunSetup();
  for (update = 0; update <= GENERATIONS; ++update)
  {
    RunStep();
    break; //TODO Remove this
    if (update % POP_SNAPSHOT_INTERVAL == 0)
      do_pop_snapshot_sig.Trigger(update);
  }

  std::clock_t base_tot_time = std::clock() - base_start_time;
  std::cout << "Time = " << 1000.0 * ((double)base_tot_time) / (double)CLOCKS_PER_SEC
            << " ms." << std::endl;
}

void EnsembleExp::ResetHardware()
{
  SGP__ResetHW();
  othello_dreamware->Reset(*game_hw); // TODO is this correct?
  othello_dreamware->SetActiveDream(0);
}

EnsembleExp::othello_idx_t EnsembleExp::EvalMove(SignalGPAgent &agent)
{
  ResetHardware();
  // Run agent until time is up or until agent indicates it is done evaluating.
  for (eval_time = 0; eval_time < EVAL_TIME && !(bool)sgp_eval_hw->GetTrait(TRAIT_ID__DONE); ++eval_time)
  { 
    sgp_eval_hw->SingleProcess();
  }

  return GetOthelloIndex((size_t)sgp_eval_hw->GetTrait(TRAIT_ID__MOVE));
}

double EnsembleExp::EvalGame(SignalGPAgent &agent, std::function<othello_idx_t(SignalGPAgent &)> &heuristic_func)
{
  game_hw->Reset();
  double score = 0;
  bool curr_player = random->GetInt(0,2); //Choose start player, 0 is individual, 1 is heuristic
  othello_dreamware->SetPlayerID( (curr_player == 0) ? othello_t::DARK : othello_t::LIGHT); //TODO

  for(size_t round_num = 0; round_num < OTHELLO_MAX_ROUND_CNT; ++round_num)
  {
    othello_idx_t move = (curr_player == 0) ? EvalMove(agent) : heuristic_func(agent);
    score = round_num;

    //Check if valid move
    if (!game_hw->IsValidMove(game_hw->GetCurPlayer(), move))
    {
      emp_assert(curr_player != 1); // Heuristic should not give an invalid move
      return score;
    }

    bool go_again = game_hw->DoNextMove(move); // Do move
    if (game_hw->IsOver()) break;
    if (!go_again) !curr_player; //Change current player if you don't get another turn
  }

  emp_assert(game_hw->IsOver());
  int rounds_left = OTHELLO_MAX_ROUND_CNT - (score + 1); // no bonus if you use all rounds
  emp_assert(rounds_left >= 0); 
  score = 2 * OTHELLO_MAX_ROUND_CNT;
  int game_score = game_hw->GetScore(game_hw->GetCurPlayer()) - game_hw->GetScore(game_hw->GetOpponent());
  exit(0);

  return 0;
}


void EnsembleExp::Evaluate()
{
  double best_score = -32767;
  best_agent_id = 0;

  for (size_t id = 0; id < sgp_world->GetSize(); ++id)
  {
    // std::cout << "Evaluating agent: " << id << std::endl;
    // Evaluate agent given by id.
    SignalGPAgent &our_hero = sgp_world->GetOrg(id);
    our_hero.SetID(id);
    sgp_eval_hw->SetProgram(our_hero.GetGenome());
    for (size_t i = 0; i < heuristics.size(); ++i)
    {
      EvalGame(our_hero, heuristics[i]);
    }
    Phenotype &phen = agent_phen_cache[id];
    if (phen.aggregate_score > best_score)
    {
      best_score = phen.aggregate_score;
      best_agent_id = id;
    }
  }
  std::cout << "Update: " << update << " Max score: " << best_score << std::endl;
}

#endif