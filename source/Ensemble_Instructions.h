#ifndef ENSEMBLE_INST_H
#define ENSEMBLE_INST_H

#include "ensemble.h"

void EnsembleExp::ConfigSGP_InstLib() {
  // Configure the instruction set.
  // - Default instruction set.
  sgp_inst_lib->AddInst("Inc", SGP__hardware_t::Inst_Inc, 1, "Increment value in local memory Arg1");
  sgp_inst_lib->AddInst("Dec", SGP__hardware_t::Inst_Dec, 1, "Decrement value in local memory Arg1");
  sgp_inst_lib->AddInst("Not", SGP__hardware_t::Inst_Not, 1, "Logically toggle value in local memory Arg1");
  sgp_inst_lib->AddInst("Add", SGP__hardware_t::Inst_Add, 3, "Local memory: Arg3 = Arg1 + Arg2");
  sgp_inst_lib->AddInst("Sub", SGP__hardware_t::Inst_Sub, 3, "Local memory: Arg3 = Arg1 - Arg2");
  sgp_inst_lib->AddInst("Mult", SGP__hardware_t::Inst_Mult, 3, "Local memory: Arg3 = Arg1 * Arg2");
  sgp_inst_lib->AddInst("Div", SGP__hardware_t::Inst_Div, 3, "Local memory: Arg3 = Arg1 / Arg2");
  sgp_inst_lib->AddInst("Mod", SGP__hardware_t::Inst_Mod, 3, "Local memory: Arg3 = Arg1 % Arg2");
  sgp_inst_lib->AddInst("TestEqu", SGP__hardware_t::Inst_TestEqu, 3, "Local memory: Arg3 = (Arg1 == Arg2)");
  sgp_inst_lib->AddInst("TestNEqu", SGP__hardware_t::Inst_TestNEqu, 3, "Local memory: Arg3 = (Arg1 != Arg2)");
  sgp_inst_lib->AddInst("TestLess", SGP__hardware_t::Inst_TestLess, 3, "Local memory: Arg3 = (Arg1 < Arg2)");
  sgp_inst_lib->AddInst("If", SGP__hardware_t::Inst_If, 1, "Local memory: If Arg1 != 0, proceed; else, skip block.", emp::ScopeType::BASIC, 0, {"block_def"});
  sgp_inst_lib->AddInst("While", SGP__hardware_t::Inst_While, 1, "Local memory: If Arg1 != 0, loop; else, skip block.", emp::ScopeType::BASIC, 0, {"block_def"});
  sgp_inst_lib->AddInst("Countdown", SGP__hardware_t::Inst_Countdown, 1, "Local memory: Countdown Arg1 to zero.", emp::ScopeType::BASIC, 0, {"block_def"});
  sgp_inst_lib->AddInst("Close", SGP__hardware_t::Inst_Close, 0, "Close current block if there is a block to close.", emp::ScopeType::BASIC, 0, {"block_close"});
  sgp_inst_lib->AddInst("Break", SGP__hardware_t::Inst_Break, 0, "Break out of current block.");
  sgp_inst_lib->AddInst("Call", SGP__hardware_t::Inst_Call, 0, "Call function that best matches call affinity.", emp::ScopeType::BASIC, 0, {"affinity"});
  sgp_inst_lib->AddInst("Return", SGP__hardware_t::Inst_Return, 0, "Return from current function if possible.");
  sgp_inst_lib->AddInst("SetMem", SGP__hardware_t::Inst_SetMem, 2, "Local memory: Arg1 = numerical value of Arg2");
  sgp_inst_lib->AddInst("CopyMem", SGP__hardware_t::Inst_CopyMem, 2, "Local memory: Arg1 = Arg2");
  sgp_inst_lib->AddInst("SwapMem", SGP__hardware_t::Inst_SwapMem, 2, "Local memory: Swap values of Arg1 and Arg2.");
  sgp_inst_lib->AddInst("Input", SGP__hardware_t::Inst_Input, 2, "Input memory Arg1 => Local memory Arg2.");
  sgp_inst_lib->AddInst("Output", SGP__hardware_t::Inst_Output, 2, "Local memory Arg1 => Output memory Arg2.");
  sgp_inst_lib->AddInst("Commit", SGP__hardware_t::Inst_Commit, 2, "Local memory Arg1 => Shared memory Arg2.");
  sgp_inst_lib->AddInst("Pull", SGP__hardware_t::Inst_Pull, 2, "Shared memory Arg1 => Shared memory Arg2.");
  sgp_inst_lib->AddInst("Nop", SGP__hardware_t::Inst_Nop, 0, "No operation.");
  // - Non-default instruction set.
  sgp_inst_lib->AddInst("Fork",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_Fork(hw, inst); },
                        0, "Generate internally-handled signal tagged with instuction's tag (spawns another thread w/local memory as new thread's input memory).");
  sgp_inst_lib->AddInst("GetBoardWidth",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP_Inst_GetBoardWidth(hw, inst); },
                        1, "WM[ARG1] = Othello board width");
  sgp_inst_lib->AddInst("EndTurn",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP_Inst_EndTurn(hw, inst); },
                        0, "End current othello turn");
  sgp_inst_lib->AddInst("SetMoveXY",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_SetMoveXY(hw, inst); },
                        2, "MoveXY = (WM[ARG1], WM[ARG2])");
  sgp_inst_lib->AddInst("SetMoveID",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_SetMoveID(hw, inst); },
                        1, "MoveID = (WM[ARG1])");
  sgp_inst_lib->AddInst("GetMoveXY",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_GetMoveXY(hw, inst); },
                        2, "WM[ARG1] = Current moveX; WM[ARG2] = Current moveY");
  sgp_inst_lib->AddInst("GetMoveID",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_GetMoveID(hw, inst); },
                        1, "WM[ARG1] = Current moveID");
  sgp_inst_lib->AddInst("IsValidXY-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_IsValidXY_HW(hw, inst); },
                        3, "WM[ARG3] = IsValidMoveXY(WM[ARG1], WM[ARG2])");
  sgp_inst_lib->AddInst("IsValidID-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_IsValidID_HW(hw, inst); },
                        2, "WM[ARG2] = IsValidMoveID(WM[ARG1])");
  sgp_inst_lib->AddInst("IsValidOppXY-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_IsValidOppXY_HW(hw, inst); },
                        3, "WM[ARG3] = IsValidOppMoveXY(WM[ARG1], WM[ARG2])");
  sgp_inst_lib->AddInst("IsValidOppID-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_IsValidOppID_HW(hw, inst); },
                        2, "WM[ARG2] = IsValidOppMoveID(WM[ARG1])");
  sgp_inst_lib->AddInst("AdjacentXY",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_AdjacentXY(hw, inst); },
                        3, "Adjusts WM[ARG1], WM[ARG2] to give adjacent location (X,Y) in direction specified by WM[ARG3]");
  sgp_inst_lib->AddInst("AdjacentID",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_AdjacentID(hw, inst); },
                        2, "Adjusts WM[ARG1] to give adjacent location (ID) in direction specified by WM[ARG2]");
  sgp_inst_lib->AddInst("ValidMoveCnt-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_ValidMoveCnt_HW(hw, inst); },
                        1, "WM[ARG1] = Number of valid moves on active othello hardware board");
  sgp_inst_lib->AddInst("ValidOppMoveCnt-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_ValidOppMoveCnt_HW(hw, inst); },
                        1, "WM[ARG1] = Number of opponent's valid moves on active othello hardware board");
  sgp_inst_lib->AddInst("GetBoardValueXY-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_GetBoardValueXY_HW(hw, inst); },
                        3, "WM[ARG3] = owner of position X (WM[ARG1]), Y (WM[ARG2]) on othello hardware board");
  sgp_inst_lib->AddInst("GetBoardValueID-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_GetBoardValueID_HW(hw, inst); },
                        2, "WM[ARG2] = owner of position ID (WM[ARG1]) on othello hardware board");
  sgp_inst_lib->AddInst("PlaceDiskXY-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_PlaceDiskXY_HW(hw, inst); },
                        3, "Place disk (of own type) on hardware board at position X (WM[ARG1]), Y (WM[ARG2]). Only successful if valid. WM[ARG3] is used to indicate move success.");
  sgp_inst_lib->AddInst("PlaceDiskID-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_PlaceDiskID_HW(hw, inst); },
                        2, "Place disk (of own type) on hardware board at position ID (WM[ARG1]). Only successful if valid. WM[ARG3] is used to indicate move success");
  sgp_inst_lib->AddInst("PlaceOppDiskXY-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_PlaceOppDiskXY_HW(hw, inst); },
                        3, "Place disk (of opponent's type) on hardware board at position X (WM[ARG1]), Y (WM[ARG2]). Only successful if valid. WM[ARG3] is used to indicate move success");
  sgp_inst_lib->AddInst("PlaceOppDiskID-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_PlaceOppDiskID_HW(hw, inst); },
                        2, "Place disk (of opponent's type) on hardware board at position ID (WM[ARG1]). Only successful if valid. WM[ARG3] is used to indicate move success");
  sgp_inst_lib->AddInst("FlipCntXY-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_FlipCntXY_HW(hw, inst); },
                        3, "WM[ARG3] = Number of disk flips if agent places disk at X (WM[ARG1]), Y (WM[ARG2])");
  sgp_inst_lib->AddInst("FlipCntID-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_FlipCntID_HW(hw, inst); },
                        2, "WM[ARG3] = Number of disk flips if agent places disk at ID (WM[ARG1])");
  sgp_inst_lib->AddInst("OppFlipCntXY-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_OppFlipCntXY_HW(hw, inst); },
                        3, "WM[ARG3] = Number of disk flips if agent's opponent places disk at X (WM[ARG1]), Y (WM[ARG2])");
  sgp_inst_lib->AddInst("OppFlipCntID-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_OppFlipCntID_HW(hw, inst); },
                        2, "WM[ARG3] = Number of disk flips if agent's opponent places disk at ID (WM[ARG1])");
  sgp_inst_lib->AddInst("FrontierCnt-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_FrontierCnt_HW(hw, inst); },
                        1, "WM[ARG1] = Agent's frontier count on othello hardware board");
  sgp_inst_lib->AddInst("ResetBoard-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_ResetBoard_HW(hw, inst); },
                        0, "Reset active othello hardware board.");
  sgp_inst_lib->AddInst("IsOver-HW",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_IsOver_HW(hw, inst); },
                        1, "Is game over on active othello hardware board?");
}


void EnsembleExp::ConfigCommunicationLib() {

  sgp_inst_lib->AddInst("SendMsgFacing", Inst_SendMsgFacing, 0, "Send output memory as message event to faced neighbor.", emp::ScopeType::BASIC, 0, {"affinity"});
  sgp_inst_lib->AddInst("BroadcastMsg", Inst_BroadcastMsg, 0, "Broadcast output memory as message event.", emp::ScopeType::BASIC, 0, {"affinity"});
  sgp_inst_lib->AddInst("GetFace", Inst_GetFace, 1, "Local memory Arg1 => Current deme member selected.");
  sgp_inst_lib->AddInst("SetFace", Inst_SetFace, 1, "Set facing trait to Local Reg Arg1");

  coord_inst_lib->AddInst("SendMsgFacing", Inst_SendMsgFacing, 0, "Send output memory as message event to faced neighbor.", emp::ScopeType::BASIC, 0, {"affinity"});
  coord_inst_lib->AddInst("BroadcastMsg", Inst_BroadcastMsg, 0, "Broadcast output memory as message event.", emp::ScopeType::BASIC, 0, {"affinity"});
  coord_inst_lib->AddInst("GetFace", Inst_GetFace, 1, "Local memory Arg1 => Current deme member selected.");
  coord_inst_lib->AddInst("SetFace", Inst_SetFace, 1, "Set facing trait to Local Reg Arg1");

  sgp_event_lib->AddEvent("MessageFacing", HandleEvent_MessageForking, "Event for messaging neighbors.");
  sgp_event_lib->AddEvent("MessageBroadcast", HandleEvent_MessageForking, "Event for broadcasting a message.");

  // Event-driven-specific.
  sgp_event_lib->RegisterDispatchFun("MessageFacing", [this](SGP__hardware_t &hw, const SGP__event_t &event) {
    this->EventDriven__DispatchMessageFacing(hw, event);
  });
  sgp_event_lib->RegisterDispatchFun("MessageBroadcast", [this](SGP__hardware_t &hw, const SGP__event_t &event) {
    this->EventDriven__DispatchMessageBroadcast(hw, event);
  });
}

void EnsembleExp::ConfigCoordinatorLib()
{
  coord_inst_lib->AddInst("Inc", SGP__hardware_t::Inst_Inc, 1, "Increment value in local memory Arg1");
  coord_inst_lib->AddInst("Dec", SGP__hardware_t::Inst_Dec, 1, "Decrement value in local memory Arg1");
  coord_inst_lib->AddInst("Not", SGP__hardware_t::Inst_Not, 1, "Logically toggle value in local memory Arg1");
  coord_inst_lib->AddInst("Add", SGP__hardware_t::Inst_Add, 3, "Local memory: Arg3 = Arg1 + Arg2");
  coord_inst_lib->AddInst("Sub", SGP__hardware_t::Inst_Sub, 3, "Local memory: Arg3 = Arg1 - Arg2");
  coord_inst_lib->AddInst("Mult", SGP__hardware_t::Inst_Mult, 3, "Local memory: Arg3 = Arg1 * Arg2");
  coord_inst_lib->AddInst("Div", SGP__hardware_t::Inst_Div, 3, "Local memory: Arg3 = Arg1 / Arg2");
  coord_inst_lib->AddInst("Mod", SGP__hardware_t::Inst_Mod, 3, "Local memory: Arg3 = Arg1 % Arg2");
  coord_inst_lib->AddInst("TestEqu", SGP__hardware_t::Inst_TestEqu, 3, "Local memory: Arg3 = (Arg1 == Arg2)");
  coord_inst_lib->AddInst("TestNEqu", SGP__hardware_t::Inst_TestNEqu, 3, "Local memory: Arg3 = (Arg1 != Arg2)");
  coord_inst_lib->AddInst("TestLess", SGP__hardware_t::Inst_TestLess, 3, "Local memory: Arg3 = (Arg1 < Arg2)");
  coord_inst_lib->AddInst("If", SGP__hardware_t::Inst_If, 1, "Local memory: If Arg1 != 0, proceed; else, skip block.", emp::ScopeType::BASIC, 0, {"block_def"});
  coord_inst_lib->AddInst("While", SGP__hardware_t::Inst_While, 1, "Local memory: If Arg1 != 0, loop; else, skip block.", emp::ScopeType::BASIC, 0, {"block_def"});
  coord_inst_lib->AddInst("Countdown", SGP__hardware_t::Inst_Countdown, 1, "Local memory: Countdown Arg1 to zero.", emp::ScopeType::BASIC, 0, {"block_def"});
  coord_inst_lib->AddInst("Close", SGP__hardware_t::Inst_Close, 0, "Close current block if there is a block to close.", emp::ScopeType::BASIC, 0, {"block_close"});
  coord_inst_lib->AddInst("Break", SGP__hardware_t::Inst_Break, 0, "Break out of current block.");
  coord_inst_lib->AddInst("Call", SGP__hardware_t::Inst_Call, 0, "Call function that best matches call affinity.", emp::ScopeType::BASIC, 0, {"affinity"});
  coord_inst_lib->AddInst("Return", SGP__hardware_t::Inst_Return, 0, "Return from current function if possible.");
  coord_inst_lib->AddInst("SetMem", SGP__hardware_t::Inst_SetMem, 2, "Local memory: Arg1 = numerical value of Arg2");
  coord_inst_lib->AddInst("CopyMem", SGP__hardware_t::Inst_CopyMem, 2, "Local memory: Arg1 = Arg2");
  coord_inst_lib->AddInst("SwapMem", SGP__hardware_t::Inst_SwapMem, 2, "Local memory: Swap values of Arg1 and Arg2.");
  coord_inst_lib->AddInst("Input", SGP__hardware_t::Inst_Input, 2, "Input memory Arg1 => Local memory Arg2.");
  coord_inst_lib->AddInst("Output", SGP__hardware_t::Inst_Output, 2, "Local memory Arg1 => Output memory Arg2.");
  coord_inst_lib->AddInst("Commit", SGP__hardware_t::Inst_Commit, 2, "Local memory Arg1 => Shared memory Arg2.");
  coord_inst_lib->AddInst("Pull", SGP__hardware_t::Inst_Pull, 2, "Shared memory Arg1 => Shared memory Arg2.");
  coord_inst_lib->AddInst("Nop", SGP__hardware_t::Inst_Nop, 0, "No operation.");
  coord_inst_lib->AddInst("EndTurn",
                        [this](SGP__hardware_t &hw, const SGP__inst_t &inst) { this->SGP_Inst_EndTurn(hw, inst); },
                        0, "End current othello turn");
  coord_inst_lib->AddInst("SetMoveXY",
                        [this](SGP__hardware_t &hw, const SGP__inst_t &inst) { this->SGP__Inst_SetMoveXY(hw, inst); },
                        2, "MoveXY = (WM[ARG1], WM[ARG2])");
  coord_inst_lib->AddInst("SetMoveID",
                        [this](SGP__hardware_t &hw, const SGP__inst_t &inst) { this->SGP__Inst_SetMoveID(hw, inst); },
                        1, "MoveID = (WM[ARG1])");
}

void EnsembleExp::ConfigConfidenceLib() 
{
  sgp_inst_lib->AddInst("SetConfidence",
                        [this](SGP__hardware_t &hw, const SGP__inst_t &inst) { this->SGP__Inst_SetMoveConfidence(hw, inst); },
                        1, "MoveConfidence = (WM[ARG1])");
  
  sgp_inst_lib->AddInst("GetMoveConfidence",
                        [this](SGP__hardware_t & hw, const SGP__inst_t & inst) { this->SGP__Inst_GetMoveConfidence(hw, inst); },
                        1, "WM[ARG1] = Current move confidence");
}

void EnsembleExp::Inst_SetFace(SGP__hardware_t &hw, const SGP__inst_t &inst)
{
  hw.SetTrait(TRAIT_ID__GID, (size_t)inst.args[0]);
}

void EnsembleExp::Inst_GetFace(SGP__hardware_t &hw, const SGP__inst_t &inst)
{
  SGP__state_t & state = hw.GetCurState();
  state.SetLocal(inst.args[0], hw.GetTrait(TRAIT_ID__GID));
}

void EnsembleExp::SGP__Inst_CastVote(SGP__hardware_t &hw, const SGP__inst_t &inst)
{
  othello_idx_t move = GetOthelloIndex((size_t)hw.GetTrait(TRAIT_ID__MOVE));
  hw.SetTrait(TRAIT_ID__CAST, hw.GetTrait(TRAIT_ID__CAST) + 1);

  if (!game_hw->IsValidMove(game_hw->GetCurPlayer(), move)) 
  {
    hw.SetTrait(TRAIT_ID__INVALID, hw.GetTrait(TRAIT_ID__INVALID) + 1);
  }
  const size_t confidence = hw.GetTrait(TRAIT_ID__CONF);
  
  if (coordinator_id >= 0)
  {
    if (hw.GetTrait(TRAIT_ID__LOC) == coordinator_id)
    {
      agent_votes[move.pos] += confidence;
    }

  }
  else
  {
    agent_votes[move.pos] += confidence;
  }
}

/// Instruction: SendMsgFacing
/// Description: Send message to faced neighbor (as determined by hardware direction trait).
void EnsembleExp::Inst_SendMsgFacing(SGP__hardware_t &hw, const SGP__inst_t &inst)
{
  SGP__state_t &state = hw.GetCurState();
  hw.TriggerEvent("MessageFacing", inst.affinity, state.output_mem, {"send"});
}

/// Instruction: BroadcastMsg
/// Description: Broadcast a message to all neighbors.
void EnsembleExp::Inst_BroadcastMsg(SGP__hardware_t &hw, const SGP__inst_t &inst)
{
  SGP__state_t &state = hw.GetCurState();
  hw.TriggerEvent("MessageBroadcast", inst.affinity, state.output_mem, {"broadcast"});
}

/// Dispatch straight to faced neighbor.
/// NOTE: needs access to eval_deme to know who neighbors are.
void EnsembleExp::EventDriven__DispatchMessageFacing(SGP__hardware_t &hw, const SGP__event_t &event)
{
  //const size_t loc = (size_t)hw.GetTrait(TRAIT_ID__LOC);
  //emp_assert(GROUP_SIZE > 1); // cant mod by 0
  const size_t facing_id = emp::Mod(hw.GetTrait(TRAIT_ID__GID), GROUP_SIZE); //emp::Mod(loc + 1 + emp::Mod(hw.GetTrait(TRAIT_ID__GID), GROUP_SIZE - 1), GROUP_SIZE); // Can get all group ids except own TODO
  sgpg_eval_hw[facing_id]->QueueEvent(event);
}

/// Dispatch to all of hw's neighbors.
/// NOTE: needs access to eval_deme to know who neighbors are.
void EnsembleExp::EventDriven__DispatchMessageBroadcast(SGP__hardware_t &hw, const SGP__event_t &event)
{
  emp_assert(GROUP_SIZE > 1); // cant mod by 0
  for (size_t i = 1; i < GROUP_SIZE; ++i)
  {
    size_t facing_id = emp::Mod(hw.GetTrait(TRAIT_ID__LOC) + i, GROUP_SIZE); // Can get all group ids except own
    sgpg_eval_hw[facing_id]->QueueEvent(event);
  }
}

/// Event handler: MessageForking
/// Description: Handles a message by spawning a new core with event data.
void EnsembleExp::HandleEvent_MessageForking(SGP__hardware_t &hw, const SGP__event_t &event)
{
  // Spawn a new core.
  hw.SpawnCore(event.affinity, hw.GetMinBindThresh(), event.msg);
}

// SGP__Inst_SetMoveConfidence
void EnsembleExp::SGP__Inst_SetMoveConfidence(SGP__hardware_t &hw, const SGP__inst_t &inst)
{
  SGP__state_t &state = hw.GetCurState();
  const size_t confidence = (size_t)state.GetLocal(inst.args[0]);
  hw.SetTrait(TRAIT_ID__CONF, confidence);
}

// SGP__Inst_GetMoveConfidence
void EnsembleExp::SGP__Inst_GetMoveConfidence(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  const size_t confidence = hw.GetTrait(TRAIT_ID__MOVE);
  state.SetLocal(inst.args[0], confidence);
}

// --- SGP instruction implementations ---
// SGP__Inst_Fork
void EnsembleExp::SGP__Inst_Fork(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  hw.SpawnCore(inst.affinity, hw.GetMinBindThresh(), state.local_mem);
}
// SGP_Inst_GetBoardWidth
void EnsembleExp::SGP_Inst_GetBoardWidth(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  state.SetLocal(inst.args[0], OTHELLO_BOARD_WIDTH);
}
// SGP_Inst_EndTurn
void EnsembleExp::SGP_Inst_EndTurn(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  hw.SetTrait(TRAIT_ID__DONE, 1);
}
// SGP__Inst_SetMoveXY
void EnsembleExp::SGP__Inst_SetMoveXY(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  const size_t move_x = (size_t)state.GetLocal(inst.args[0]);
  const size_t move_y = (size_t)state.GetLocal(inst.args[1]);
  const othello_idx_t move(move_x, move_y);
  hw.SetTrait(TRAIT_ID__MOVE, move.pos);
}
// SGP__Inst_SetMoveID
void EnsembleExp::SGP__Inst_SetMoveID(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  const size_t move_id = (size_t)state.GetLocal(inst.args[0]);
  hw.SetTrait(TRAIT_ID__MOVE, move_id);
}
// SGP__Inst_GetMoveXY
void EnsembleExp::SGP__Inst_GetMoveXY(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  const othello_idx_t move = GetOthelloIndex((size_t)hw.GetTrait(TRAIT_ID__MOVE));
  state.SetLocal(inst.args[0], move.x());
  state.SetLocal(inst.args[1], move.y());
}
// SGP__Inst_GetMoveID
void EnsembleExp::SGP__Inst_GetMoveID(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  const size_t move_id = hw.GetTrait(TRAIT_ID__MOVE);
  state.SetLocal(inst.args[0], move_id);
}
// SGP__Inst_IsValidXY
void EnsembleExp::SGP__Inst_IsValidXY_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const player_t playerID = othello_dreamware->GetPlayerID();
  const size_t move_x = state.GetLocal(inst.args[0]);
  const size_t move_y = state.GetLocal(inst.args[1]);
  const int valid = (int)dreamboard.IsValidMove(playerID, {move_x, move_y});
  state.SetLocal(inst.args[2], valid);
}
// SGP__Inst_IsValidID_HW
void EnsembleExp::SGP__Inst_IsValidID_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const player_t playerID = othello_dreamware->GetPlayerID();
  const size_t move_id = state.GetLocal(inst.args[0]);
  const int valid = (int)dreamboard.IsValidMove(playerID, GetOthelloIndex(move_id));
  state.SetLocal(inst.args[1], valid);
}
// SGP__Inst_IsValidOppXY
void EnsembleExp::SGP__Inst_IsValidOppXY_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const player_t playerID = othello_dreamware->GetPlayerID();
  const player_t oppID = dreamboard.GetOpponent(playerID);
  const size_t move_x = state.GetLocal(inst.args[0]);
  const size_t move_y = state.GetLocal(inst.args[1]);
  const int valid = (int)dreamboard.IsValidMove(oppID, {move_x, move_y});
  state.SetLocal(inst.args[2], valid);
}
// SGP__Inst_IsValidOppID
void EnsembleExp::SGP__Inst_IsValidOppID_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const player_t playerID = othello_dreamware->GetPlayerID();
  const player_t oppID = dreamboard.GetOpponent(playerID);
  const size_t move_id = state.GetLocal(inst.args[0]);
  const int valid = (int)dreamboard.IsValidMove(oppID, GetOthelloIndex(move_id));
  state.SetLocal(inst.args[1], valid);
}
// SGP__Inst_AdjacentXY
void EnsembleExp::SGP__Inst_AdjacentXY(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const size_t move_x = (size_t)state.GetLocal(inst.args[0]);
  const size_t move_y = (size_t)state.GetLocal(inst.args[1]);
  const facing_t dir  = IntToFacing(state.GetLocal(inst.args[2]));
  const othello_idx_t neighbor = dreamboard.GetNeighbor({move_x, move_y}, dir);
  if (!neighbor.IsValid()) {
    state.SetLocal(inst.args[0], AGENT_VIEW__ILLEGAL_ID);
    state.SetLocal(inst.args[1], AGENT_VIEW__ILLEGAL_ID);
  } else {
    state.SetLocal(inst.args[0], neighbor.x());
    state.SetLocal(inst.args[1], neighbor.y());
  }
}
// SGP__Inst_AdjacentID
void EnsembleExp::SGP__Inst_AdjacentID(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const size_t move_id = (size_t)state.GetLocal(inst.args[0]);
  const facing_t dir = IntToFacing(state.GetLocal(inst.args[1]));
  const othello_idx_t neighbor = dreamboard.GetNeighbor(GetOthelloIndex(move_id), dir);
  state.SetLocal(inst.args[0], (!neighbor.IsValid()) ? AGENT_VIEW__ILLEGAL_ID : neighbor.pos);
}
// SGP_Inst_ValidMoveCnt_HW
void EnsembleExp::SGP__Inst_ValidMoveCnt_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const player_t playerID = othello_dreamware->GetPlayerID();
  state.SetLocal(inst.args[0], dreamboard.GetMoveOptions(playerID).size());
}
// SGP_Inst_ValidOppMoveCnt_HW
void EnsembleExp::SGP__Inst_ValidOppMoveCnt_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const player_t playerID = othello_dreamware->GetPlayerID();
  const player_t oppID = dreamboard.GetOpponent(playerID);
  state.SetLocal(inst.args[0], dreamboard.GetMoveOptions(oppID).size());
}
// SGP_Inst_GetBoardValueXY_HW
void EnsembleExp::SGP__Inst_GetBoardValueXY_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const size_t move_x = state.GetLocal(inst.args[0]);
  const size_t move_y = state.GetLocal(inst.args[1]);
  const othello_idx_t move(move_x, move_y);
  const player_t playerID = othello_dreamware->GetPlayerID();
  const player_t oppID = dreamboard.GetOpponent(playerID);
  // If inputs are garbage, let the caller know.
  if (move.IsValid()) {
    const player_t owner = dreamboard.GetPosOwner({move_x, move_y});
    if (owner == playerID) { state.SetLocal(inst.args[2], AGENT_VIEW__SELF_ID); }
    else if (owner == oppID) { state.SetLocal(inst.args[2], AGENT_VIEW__OPP_ID); }
    else { state.SetLocal(inst.args[2], AGENT_VIEW__OPEN_ID); }
  } else {
    state.SetLocal(inst.args[2], AGENT_VIEW__ILLEGAL_ID);
  }
}
// SGP_Inst_GetBoardValueID_HW
void EnsembleExp::SGP__Inst_GetBoardValueID_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const size_t move_id = state.GetLocal(inst.args[0]);
  const othello_idx_t move(GetOthelloIndex(move_id));
  const player_t playerID = othello_dreamware->GetPlayerID();
  const player_t oppID = dreamboard.GetOpponent(playerID);
  // If inputs are garbage, let the caller know.
  if (move.IsValid()) {
    const player_t owner = dreamboard.GetPosOwner(move_id);
    if (owner == playerID) { state.SetLocal(inst.args[1], AGENT_VIEW__SELF_ID); }
    else if (owner == oppID) { state.SetLocal(inst.args[1], AGENT_VIEW__OPP_ID); }
    else { state.SetLocal(inst.args[1], AGENT_VIEW__OPEN_ID); }
  } else {
    state.SetLocal(inst.args[1], AGENT_VIEW__ILLEGAL_ID);
  }
}
// SGP_Inst_PlaceDiskXY_HW
void EnsembleExp::SGP__Inst_PlaceDiskXY_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const size_t move_x = (size_t)state.GetLocal(inst.args[0]);
  const size_t move_y = (size_t)state.GetLocal(inst.args[1]);
  const othello_idx_t move(move_x, move_y);
  const player_t playerID = othello_dreamware->GetPlayerID();
  if (dreamboard.IsValidMove(playerID, move)) {
    dreamboard.DoMove(playerID, move);
    state.SetLocal(inst.args[2], 1);
  } else {
    state.SetLocal(inst.args[2], 0);
  }
}
// SGP_Inst_PlaceDiskID_HW
void EnsembleExp::SGP__Inst_PlaceDiskID_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const othello_idx_t move = GetOthelloIndex(state.GetLocal(inst.args[0]));
  const player_t playerID = othello_dreamware->GetPlayerID();
  if (dreamboard.IsValidMove(playerID, move)) {
    dreamboard.DoMove(playerID, move);
    state.SetLocal(inst.args[1], 1);
  } else {
    state.SetLocal(inst.args[1], 0);
  }
}
// SGP_Inst_PlaceOppDiskXY_HW
void EnsembleExp::SGP__Inst_PlaceOppDiskXY_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const size_t move_x = (size_t)state.GetLocal(inst.args[0]);
  const size_t move_y = (size_t)state.GetLocal(inst.args[1]);
  const othello_idx_t move(move_x, move_y);
  const player_t playerID = othello_dreamware->GetPlayerID();
  const player_t oppID = dreamboard.GetOpponent(playerID);
  if (dreamboard.IsValidMove(oppID, move)) {
    dreamboard.DoMove(oppID, move);
    state.SetLocal(inst.args[2], 1);
  } else {
    state.SetLocal(inst.args[2], 0);
  }
}
// SGP_Inst_PlaceOppDiskID_HW
void EnsembleExp::SGP__Inst_PlaceOppDiskID_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const othello_idx_t move = GetOthelloIndex(state.GetLocal(inst.args[0]));
  const player_t playerID = othello_dreamware->GetPlayerID();
  const player_t oppID = dreamboard.GetOpponent(playerID);
  if (dreamboard.IsValidMove(oppID, move)) {
    dreamboard.DoMove(oppID, move);
    state.SetLocal(inst.args[1], 1);
  } else {
    state.SetLocal(inst.args[1], 0);
  }
}
// SGP_Inst_FlipCntXY_HW
void EnsembleExp::SGP__Inst_FlipCntXY_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const size_t move_x = (size_t)state.GetLocal(inst.args[0]);
  const size_t move_y = (size_t)state.GetLocal(inst.args[1]);
  const othello_idx_t move(move_x, move_y);
  const player_t playerID = othello_dreamware->GetPlayerID();
  if (dreamboard.IsValidMove(playerID, move)) {
    state.SetLocal(inst.args[2], dreamboard.GetFlipCount(playerID, move));
  } else {
    state.SetLocal(inst.args[2], 0);
  }
}
// SGP_Inst_FlipCntID_HW
void EnsembleExp::SGP__Inst_FlipCntID_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const othello_idx_t move = GetOthelloIndex((size_t)state.GetLocal(inst.args[0]));
  const player_t playerID = othello_dreamware->GetPlayerID();
  if (dreamboard.IsValidMove(playerID, move)) {
    state.SetLocal(inst.args[1], dreamboard.GetFlipCount(playerID, move));
  } else {
    state.SetLocal(inst.args[1], 0);
  }
}
// SGP_Inst_OppFlipCntXY_HW
void EnsembleExp::SGP__Inst_OppFlipCntXY_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const size_t move_x = (size_t)state.GetLocal(inst.args[0]);
  const size_t move_y = (size_t)state.GetLocal(inst.args[1]);
  const othello_idx_t move(move_x, move_y);
  const player_t playerID = othello_dreamware->GetPlayerID();
  const player_t oppID = dreamboard.GetOpponent(playerID);
  if (dreamboard.IsValidMove(oppID, move)) {
    state.SetLocal(inst.args[2], dreamboard.GetFlipCount(oppID, move));
  } else {
    state.SetLocal(inst.args[2], 0);
  }
}
// SGP_Inst_OppFlipCntID_HW
void EnsembleExp::SGP__Inst_OppFlipCntID_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const othello_idx_t move = GetOthelloIndex((size_t)state.GetLocal(inst.args[0]));
  const player_t playerID = othello_dreamware->GetPlayerID();
  const player_t oppID = dreamboard.GetOpponent(playerID);
  if (dreamboard.IsValidMove(oppID, move)) {
    state.SetLocal(inst.args[1], dreamboard.GetFlipCount(oppID, move));
  } else {
    state.SetLocal(inst.args[1], 0);
  }
}
// SGP_Inst_FrontierCnt_HW
void EnsembleExp::SGP__Inst_FrontierCnt_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  const player_t playerID = othello_dreamware->GetPlayerID();
  state.SetLocal(inst.args[0], dreamboard.CountFrontierPos(playerID));
}
// // SGP_Inst_ResetBoard_HW
void EnsembleExp::SGP__Inst_ResetBoard_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  othello_dreamware->ResetActive(*game_hw);
}
// SGP_Inst_IsOver_HW
void EnsembleExp::SGP__Inst_IsOver_HW(SGP__hardware_t & hw, const SGP__inst_t & inst) {
  SGP__state_t & state = hw.GetCurState();
  othello_t & dreamboard = othello_dreamware->GetActiveDreamOthello();
  state.SetLocal(inst.args[0], (int)dreamboard.IsOver());
}


#endif