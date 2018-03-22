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

#endif