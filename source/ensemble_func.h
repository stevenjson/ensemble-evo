#ifndef ENSEMBLE_FUNC_H
#define ENSEMBLE_FUNC_H
#include "ensemble.h"

/// Configure the world settings for the expirement
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

  sgp_world->SetFitFun([this](SignalGPAgent &agent) { return this->CalcFitness(agent); });

  // Setup fitness tracking.
  auto &fit_file = sgp_world->SetupFitnessFile(DATA_DIRECTORY + "fitness.csv");
  fit_file.SetTimingRepeat(FITNESS_INTERVAL);
  record_fit_sig.AddAction([this](size_t pos, double fitness) { sgp_world->GetGenotypeAt(pos)->GetData().RecordFitness(fitness); });

  // Setup phenotype tracking
  //TODO: AddBestPhenotypeFile(*sgp_world, DATA_DIRECTORY + "best_phenotype.csv").SetTimingRepeat(SYSTEMATICS_INTERVAL);
  record_phen_sig.AddAction([this](size_t pos, const phenotype_t &phen) { sgp_world->GetGenotypeAt(pos)->GetData().RecordPhenotype(phen); });

  // Generate the initial population.
  do_pop_init_sig.AddAction([this]() { this->SGP__InitPopulation_Random(); });
}

/// Configure the world settings for the expirement
void EnsembleExp::ConfigSGPG() {
  // Configure the world.
  sgpg_world->Reset();
  sgpg_world->SetWellMixed(true);

  // Setup mutation function.
  if (SGP_VARIABLE_LENGTH)
  {
    sgpg_world->SetMutFun([this](GroupSignalGPAgent &agent, emp::Random &rnd) { return this->SGPG__Mutate_VariableLength(agent, rnd); }, ELITE_SELECT__ELITE_CNT);
  }
  else
  {
    // NOTE: second argument specifies that we're not mutating the first thing int the pop (we're doing elite selection in all of our stuff).
    sgpg_world->SetMutFun([this](GroupSignalGPAgent &agent, emp::Random &rnd) { return this->SGPG__Mutate_FixedLength(agent, rnd); }, ELITE_SELECT__ELITE_CNT);
  }

  sgpg_world->SetFitFun([this](GroupSignalGPAgent &agent) { return this->CalcFitness(agent); });

  // Setup fitness tracking.
  auto &fit_file = sgpg_world->SetupFitnessFile(DATA_DIRECTORY + "fitness.csv");
  fit_file.SetTimingRepeat(FITNESS_INTERVAL);
  record_fit_sig.AddAction([this](size_t pos, double fitness) { sgpg_world->GetGenotypeAt(pos)->GetData().RecordFitness(fitness); });

  // Setup phenotype tracking
  //TODO: AddBestPhenotypeFile(*sgpg_world, DATA_DIRECTORY + "best_phenotype.csv").SetTimingRepeat(SYSTEMATICS_INTERVAL);
  record_phen_sig.AddAction([this](size_t pos, const phenotype_t &phen) { sgpg_world->GetGenotypeAt(pos)->GetData().RecordPhenotype(phen); });

  // Generate the initial population.
  do_pop_init_sig.AddAction([this]() { this->SGPG__InitPopulation_Random(); });
}

/// Reset the SignalGP evaluation hardware, setting input memory of
/// main thread to be equal to main_in_mem.
void EnsembleExp::SGP__ResetHW(const SGP__memory_t &main_in_mem)
{
  sgp_eval_hw->ResetHardware();
  sgp_eval_hw->SetTrait(TRAIT_ID__MOVE, -1);
  sgp_eval_hw->SetTrait(TRAIT_ID__DONE, 0);
  sgp_eval_hw->SpawnCore(0, main_in_mem, true);
}

/// Write genomes of entire population to a file at the given update.
/// param: update, the current update the population is being writen from.
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

/// Mutation function where genome length is always the same for each function in SGP
/// param: agent, our current organism we are mutating
/// param: rnd, the random number generator
/// return: mut_count, total mutations for this agent
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

/// Mutation function where genome length is always the same for each function in SGP
/// param: agent, our current organism we are mutating
/// param: rnd, the random number generator
/// return: mut_count, total mutations for this agent
size_t EnsembleExp::SGPG__Mutate_FixedLength(GroupSignalGPAgent &agent, emp::Random &rnd)
{
  emp::vector<SGP__program_t> &programs = agent.GetGenome();
  size_t mut_cnt = 0;

  for (auto program : programs){
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
  }
  return mut_cnt;
}

/// Mutation function where genome length can vary from insertions and deletions for each function in SGP.
/// param: agent, our current organism we are mutating
/// param: rnd, the random number generator
/// return: mut_count, total mutations for this agent
size_t EnsembleExp::SGP__Mutate_VariableLength(SignalGPAgent &agent, emp::Random &rnd)
{
  SGP__program_t &program = agent.GetGenome();
  size_t mut_cnt = 0;
  size_t expected_prog_len = program.GetInstCnt();

  // Duplicate a (single) function?
  if (rnd.P(SGP_PER_FUNC__FUNC_DUP_RATE) && program.GetSize() < SGP_MAX_FUNCTION_CNT)
  {
    const uint32_t fID = rnd.GetUInt(program.GetSize());
    // Would function duplication make expected program length exceed max?
    if (expected_prog_len + program[fID].GetSize() <= SGP_PROG_MAX_LENGTH)
    {
      program.PushFunction(program[fID]);
      expected_prog_len += program[fID].GetSize();
      ++mut_cnt;
    }
  }

  // Delete a (single) function?
  if (rnd.P(SGP_PER_FUNC__FUNC_DEL_RATE) && program.GetSize() > SGP_MIN_FUNC_CNT)
  {
    const uint32_t fID = rnd.GetUInt(program.GetSize());
    expected_prog_len -= program[fID].GetSize();
    program[fID] = program[program.GetSize() - 1];
    program.program.resize(program.GetSize() - 1);
    ++mut_cnt;
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

    // Slip-mutation?
    if (rnd.P(SGP_PER_FUNC__SLIP_RATE))
    {
      uint32_t begin = rnd.GetUInt(program[fID].GetSize());
      uint32_t end = rnd.GetUInt(program[fID].GetSize());
      const bool dup = begin < end;
      const bool del = begin > end;
      const int dup_size = end - begin;
      const int del_size = begin - end;
      // If we would be duplicating and the result will not exceed maximum program length, duplicate!
      if (dup && (expected_prog_len + dup_size <= SGP_PROG_MAX_LENGTH) && (program[fID].GetSize() + dup_size <= SGP_MAX_FUNCTION_LEN))
      {
        // duplicate begin:end
        const size_t new_size = program[fID].GetSize() + (size_t)dup_size;
        SGP__hardware_t::Function new_fun(program[fID].GetAffinity());
        for (size_t i = 0; i < new_size; ++i)
        {
          if (i < end)
            new_fun.PushInst(program[fID][i]);
          else
            new_fun.PushInst(program[fID][i - dup_size]);
        }
        program[fID] = new_fun;
        ++mut_cnt;
        expected_prog_len += dup_size;
      }
      else if (del && ((program[fID].GetSize() - del_size) >= SGP_MIN_FUNCTION_LEN))
      {
        // delete end:begin
        SGP__hardware_t::Function new_fun(program[fID].GetAffinity());
        for (size_t i = 0; i < end; ++i)
          new_fun.PushInst(program[fID][i]);
        for (size_t i = begin; i < program[fID].GetSize(); ++i)
          new_fun.PushInst(program[fID][i]);
        program[fID] = new_fun;
        ++mut_cnt;
        expected_prog_len -= del_size;
      }
    }

    // Substitution mutations? (pretty much completely safe)
    for (size_t i = 0; i < program[fID].GetSize(); ++i)
    {
      SGP__inst_t &inst = program[fID][i];
      // Mutate affinity (even when it doesn't use it).
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
    // - Compute number of insertions.
    int num_ins = rnd.GetRandBinomial(program[fID].GetSize(), SGP_PER_INST__INS_RATE);
    // Ensure that insertions don't exceed maximum program length.
    if ((num_ins + program[fID].GetSize()) > SGP_MAX_FUNCTION_LEN)
    {
      num_ins = SGP_MAX_FUNCTION_LEN - program[fID].GetSize();
    }
    if ((num_ins + expected_prog_len) > SGP_PROG_MAX_LENGTH)
    {
      num_ins = SGP_PROG_MAX_LENGTH - expected_prog_len;
    }
    expected_prog_len += num_ins;

    // Do we need to do any insertions or deletions?
    if (num_ins > 0 || SGP_PER_INST__DEL_RATE > 0.0)
    {
      size_t expected_func_len = num_ins + program[fID].GetSize();
      // Compute insertion locations and sort them.
      emp::vector<size_t> ins_locs = emp::RandomUIntVector(rnd, num_ins, 0, program[fID].GetSize());
      if (ins_locs.size())
        std::sort(ins_locs.begin(), ins_locs.end(), std::greater<size_t>());
      SGP__hardware_t::Function new_fun(program[fID].GetAffinity());
      size_t rhead = 0;
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
        if (rnd.P(SGP_PER_INST__DEL_RATE) && (expected_func_len > SGP_MIN_FUNCTION_LEN))
        {
          ++mut_cnt;
          --expected_prog_len;
          --expected_func_len;
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

/// Mutation function where genome length can vary from insertions and deletions for each function in SGP.
/// param: agent, our current organism we are mutating
/// param: rnd, the random number generator
/// return: mut_count, total mutations for this agent
size_t EnsembleExp::SGPG__Mutate_VariableLength(GroupSignalGPAgent &agent, emp::Random &rnd)
{
  emp::vector<SGP__program_t> &programs = agent.GetGenome();
  size_t mut_cnt = 0;

  for (auto program : programs){
    size_t expected_prog_len = program.GetInstCnt();

    // Duplicate a (single) function?
    if (rnd.P(SGP_PER_FUNC__FUNC_DUP_RATE) && program.GetSize() < SGP_MAX_FUNCTION_CNT)
    {
      const uint32_t fID = rnd.GetUInt(program.GetSize());
      // Would function duplication make expected program length exceed max?
      if (expected_prog_len + program[fID].GetSize() <= SGP_PROG_MAX_LENGTH)
      {
        program.PushFunction(program[fID]);
        expected_prog_len += program[fID].GetSize();
        ++mut_cnt;
      }
    }

    // Delete a (single) function?
    if (rnd.P(SGP_PER_FUNC__FUNC_DEL_RATE) && program.GetSize() > SGP_MIN_FUNC_CNT)
    {
      const uint32_t fID = rnd.GetUInt(program.GetSize());
      expected_prog_len -= program[fID].GetSize();
      program[fID] = program[program.GetSize() - 1];
      program.program.resize(program.GetSize() - 1);
      ++mut_cnt;
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

      // Slip-mutation?
      if (rnd.P(SGP_PER_FUNC__SLIP_RATE))
      {
        uint32_t begin = rnd.GetUInt(program[fID].GetSize());
        uint32_t end = rnd.GetUInt(program[fID].GetSize());
        const bool dup = begin < end;
        const bool del = begin > end;
        const int dup_size = end - begin;
        const int del_size = begin - end;
        // If we would be duplicating and the result will not exceed maximum program length, duplicate!
        if (dup && (expected_prog_len + dup_size <= SGP_PROG_MAX_LENGTH) && (program[fID].GetSize() + dup_size <= SGP_MAX_FUNCTION_LEN))
        {
          // duplicate begin:end
          const size_t new_size = program[fID].GetSize() + (size_t)dup_size;
          SGP__hardware_t::Function new_fun(program[fID].GetAffinity());
          for (size_t i = 0; i < new_size; ++i)
          {
            if (i < end)
              new_fun.PushInst(program[fID][i]);
            else
              new_fun.PushInst(program[fID][i - dup_size]);
          }
          program[fID] = new_fun;
          ++mut_cnt;
          expected_prog_len += dup_size;
        }
        else if (del && ((program[fID].GetSize() - del_size) >= SGP_MIN_FUNCTION_LEN))
        {
          // delete end:begin
          SGP__hardware_t::Function new_fun(program[fID].GetAffinity());
          for (size_t i = 0; i < end; ++i)
            new_fun.PushInst(program[fID][i]);
          for (size_t i = begin; i < program[fID].GetSize(); ++i)
            new_fun.PushInst(program[fID][i]);
          program[fID] = new_fun;
          ++mut_cnt;
          expected_prog_len -= del_size;
        }
      }

      // Substitution mutations? (pretty much completely safe)
      for (size_t i = 0; i < program[fID].GetSize(); ++i)
      {
        SGP__inst_t &inst = program[fID][i];
        // Mutate affinity (even when it doesn't use it).
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
      // - Compute number of insertions.
      int num_ins = rnd.GetRandBinomial(program[fID].GetSize(), SGP_PER_INST__INS_RATE);
      // Ensure that insertions don't exceed maximum program length.
      if ((num_ins + program[fID].GetSize()) > SGP_MAX_FUNCTION_LEN)
      {
        num_ins = SGP_MAX_FUNCTION_LEN - program[fID].GetSize();
      }
      if ((num_ins + expected_prog_len) > SGP_PROG_MAX_LENGTH)
      {
        num_ins = SGP_PROG_MAX_LENGTH - expected_prog_len;
      }
      expected_prog_len += num_ins;

      // Do we need to do any insertions or deletions?
      if (num_ins > 0 || SGP_PER_INST__DEL_RATE > 0.0)
      {
        size_t expected_func_len = num_ins + program[fID].GetSize();
        // Compute insertion locations and sort them.
        emp::vector<size_t> ins_locs = emp::RandomUIntVector(rnd, num_ins, 0, program[fID].GetSize());
        if (ins_locs.size())
          std::sort(ins_locs.begin(), ins_locs.end(), std::greater<size_t>());
        SGP__hardware_t::Function new_fun(program[fID].GetAffinity());
        size_t rhead = 0;
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
          if (rnd.P(SGP_PER_INST__DEL_RATE) && (expected_func_len > SGP_MIN_FUNCTION_LEN))
          {
            ++mut_cnt;
            --expected_prog_len;
            --expected_func_len;
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
  }
  return mut_cnt;
}

/// Creates initial population of organisms with random genomes and injects them into the world.
void EnsembleExp::SGP__InitPopulation_Random()
{
  std::cout << "Initializing population randomly!" << std::endl;
  for (size_t p = 0; p < POP_SIZE; ++p)
  {
    SGP__program_t prog(sgp_inst_lib);
    for (size_t f = 0; f < SGP_MAX_FUNCTION_CNT; ++f)
    {
      prog.PushFunction();
      prog[f].affinity.Randomize(*random);
      for (size_t i = 0; i < SGP_MAX_FUNCTION_LEN; ++i)
      {
        const size_t instID = random->GetUInt(sgp_inst_lib->GetSize()); //  Get random instruction
        const size_t a0 = random->GetUInt(0, SGP_PROG_MAX_ARG_VAL); //      Get random register for arg 1
        const size_t a1 = random->GetUInt(0, SGP_PROG_MAX_ARG_VAL); //      Get random register for arg 2
        const size_t a2 = random->GetUInt(0, SGP_PROG_MAX_ARG_VAL); //      Get random register for arg 3
        SGP__inst_t inst(instID, a0, a1, a2);
        inst.affinity.Randomize(*random);
        prog[f].PushInst(inst);
      }
    }
    sgp_world->Inject(prog, 1);
  }
}

/// Creates initial population of organisms with random genomes and injects them into the world.
void EnsembleExp::SGPG__InitPopulation_Random()
{
  std::cout << "Initializing population randomly!" << std::endl;

  for (size_t p = 0; p < POP_SIZE; ++p)
  {
    emp::vector<SGP__program_t> programs;
    for (size_t i = 0; i < GROUP_SIZE; ++i)
    {
      SGP__program_t prog(sgp_inst_lib);
      for (size_t f = 0; f < SGP_MAX_FUNCTION_CNT; ++f)
      {
        prog.PushFunction();
        prog[f].affinity.Randomize(*random);
        for (size_t i = 0; i < SGP_MAX_FUNCTION_LEN; ++i)
        {
          const size_t instID = random->GetUInt(sgp_inst_lib->GetSize()); // Get random instruction
          const size_t a0 = random->GetUInt(0, SGP_PROG_MAX_ARG_VAL);     // Get random register for arg 1
          const size_t a1 = random->GetUInt(0, SGP_PROG_MAX_ARG_VAL);     // Get random register for arg 2
          const size_t a2 = random->GetUInt(0, SGP_PROG_MAX_ARG_VAL);     // Get random register for arg 3
          SGP__inst_t inst(instID, a0, a1, a2);
          inst.affinity.Randomize(*random);
          prog[f].PushInst(inst);
        }
      }
      programs.push_back(prog);
    }
    sgpg_world->Inject(programs, 1);
  }
}

/// Configure different types of othello programs to compete organisms against 
/// when calculating fitness.
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

/// Setup data files and recording, and initialize starting population
void EnsembleExp::RunSetup()
{
  std::cout << "Doing initial run setup." << std::endl;

  // Generate the initial population.
  do_pop_init_sig.Trigger();
  exit(0);
}

/// Do a single step of evolution.
void EnsembleExp::RunStep()
{
  Evaluate();   // Update agent scores.
  Selection();    // Do selection (selection, reproduction, mutation).
  sgp_world->Update(); // Do world update (population turnover, clear score caches).
}

/// The 'main' function for the class. Calling it starts the expirement.
void EnsembleExp::Run()
{
  std::clock_t base_start_time = std::clock();

  RunSetup();
  for (update = 0; update <= GENERATIONS; ++update)
  {
    RunStep();
    if (update % POP_SNAPSHOT_INTERVAL == 0)
      do_pop_snapshot_sig.Trigger(update);
  }

  std::clock_t base_tot_time = std::clock() - base_start_time;
  std::cout << "Time = " << 1000.0 * ((double)base_tot_time) / (double)CLOCKS_PER_SEC
            << " ms." << std::endl;
}

/// Resets the state of the organism being evaluated.
void EnsembleExp::ResetHardware()
{
  SGP__ResetHW();
  othello_dreamware->Reset(*game_hw);
  othello_dreamware->SetActiveDream(0);
}

/// Evaluate a single move for the currently selected organism.
/// param: agent, the current organism to evaluate
/// returns: the given agent's move
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

/// Evaluates an organism on a game of Othello.
/// param: agent, the organism to be evaluated
/// param: heuristic_func, the handwritten AI the organism will compete against
/// returns: the score of the organism.
double EnsembleExp::EvalGame(SignalGPAgent &agent, std::function<othello_idx_t(SignalGPAgent &)> &heuristic_func)
{
  // Initialize othello game
  game_hw->Reset();
  double score = 0;
  bool curr_player = random->GetInt(0,2); //Choose start player, 0 is individual, 1 is heuristic
  othello_dreamware->SetPlayerID( (curr_player == 0) ? othello_t::DARK : othello_t::LIGHT);

  // Main game loop
  for(size_t round_num = 0; round_num < OTHELLO_MAX_ROUND_CNT; ++round_num)
  {
    othello_idx_t move = (curr_player == 0) ? EvalMove(agent) : heuristic_func(agent);
    score = round_num;

    //If a invalid move is given, fitness becomes rounds completed w/o error
    if (!game_hw->IsValidMove(game_hw->GetCurPlayer(), move))
    {
      emp_assert(curr_player != 1); // Heuristic should not give an invalid move
      return score;
    }

    bool go_again = game_hw->DoNextMove(move);
    if (game_hw->IsOver()) break;
    if (!go_again) curr_player = !curr_player; //Change current player if you don't get another turn
  }

  // Setup for score calculation
  int rounds_left = OTHELLO_MAX_ROUND_CNT - (score + 1);
  double hero_score = game_hw->GetScore(game_hw->GetCurPlayer());
  double opp_score = game_hw->GetScore(game_hw->GetOpponent(game_hw->GetCurPlayer()));
  emp_assert(rounds_left >= 0);
  emp_assert(game_hw->IsOver());

  // Bonus for completing game, increased by performance in game. Max possible fitness is 235 per heuristic.
  score = 2 * OTHELLO_MAX_ROUND_CNT + hero_score;
  if (hero_score > opp_score) {score += 2 * rounds_left;} // If you win, you get points for rounds left

  return score;
}

/// Calculate fitness for all organisms in the population.
void EnsembleExp::Evaluate()
{
  double best_score = -32767;
  best_agent_id = 0;

  for (size_t id = 0; id < sgp_world->GetSize(); ++id)
  {
    // Evaluate agent given by id.
    SignalGPAgent &our_hero = sgp_world->GetOrg(id);
    our_hero.SetID(id);
    sgp_eval_hw->SetProgram(our_hero.GetGenome());
    // Initialize fitness tracking object
    Phenotype &phen = agent_phen_cache[id];
    phen.aggregate_score = 0;
    phen.illegal_move_total = 0;

    for (size_t i = 0; i < heuristics.size(); ++i)
    {
      phen.heuristic_scores[i] = EvalGame(our_hero, heuristics[i]);
      phen.aggregate_score += phen.heuristic_scores[i]; // Sum of scores is fitness of organism
      if (phen.heuristic_scores[i] < OTHELLO_MAX_ROUND_CNT) {phen.illegal_move_total++;}
    }

    // Write current fitness information to file
    record_fit_sig.Trigger(id, phen.aggregate_score);
    record_phen_sig.Trigger(id, phen.heuristic_scores);

    if (phen.aggregate_score > best_score)
    {
      best_score = phen.aggregate_score;
      best_agent_id = id;
    }
  }
  std::cout << "Update: " << update << " Max score: " << best_score << std::endl;
}

/// Select organisms for next generation using given selection method. 
void EnsembleExp::Selection()
{
  emp::EliteSelect(*sgp_world, ELITE_SELECT__ELITE_CNT, 1); // Always add best organism to next gen
  
  switch (SELECTION_METHOD)
  {
    case SELECTION_METHOD_ID__TOURNAMENT:
      emp::TournamentSelect(*sgp_world, TOURNAMENT_SIZE, POP_SIZE - ELITE_SELECT__ELITE_CNT);
      break;
    case SELECTION_METHOD_ID__LEXICASE:
    {
      sgp_lexicase_fit_set.resize(0);
      for (size_t i = 0; i < heuristics.size(); ++i)
      {
        sgp_lexicase_fit_set.push_back([i, this](SignalGPAgent &agent) {
          Phenotype &phen = agent_phen_cache[agent.GetID()];
          return phen.heuristic_scores[i];
        });
      }
      emp::LexicaseSelect(*sgp_world, sgp_lexicase_fit_set, POP_SIZE - ELITE_SELECT__ELITE_CNT);
      break;
    }
    default:
      std::cout << "Unrecognized selection method! Exiting..." << std::endl;
      exit(-1);
  }
}

#endif