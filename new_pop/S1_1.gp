Fn-1011101101000100:
  Div[0110000001011010](6,0,10)
  TestLess[1000011101101111](2,12,12)
  Call[0000011000100111](1,10,5)
  ResetBoard-HW[1010101110011000](6,6,12)
  Nop[1100000111001011](5,9,8)
  TestEqu[0001111101101011](15,5,5)
  IsValidXY-HW[1100101101101111](0,12,10)
  ResetBoard-HW[1000101110101101](14,3,7)
  Call[0000010011010100](13,13,4)
  PlaceDiskXY-HW[1110011110101100](14,4,0)
  SetMoveXY[0110001100100001](0,3,1)
  Inc[1111000011111111](3,12,8)
  PlaceOppDiskID-HW[1011001111001000](10,10,6)
  Break[0001011001011100](1,10,6)
  Nop[1101010100111110](9,5,1)
  GetMoveXY[0011011000100010](9,13,6)
  GetBoardValueXY-HW[1101100111110100](4,0,5)
  Fork[1100101110110111](12,5,2)
  GetBoardValueID-HW[0100001011111000](14,3,15)
  AdjacentXY[1100000111100011](7,1,0)
  Break[1011101010000101](10,10,14)
  TestEqu[0110111010100100](9,6,14)
  TestNEqu[0110111011111100](9,13,6)
  OppFlipCntXY-HW[1010101110000111](14,8,10)
  Not[0000110111000101](7,7,9)
  SetMoveXY[1101000110000001](5,11,10)
  OppFlipCntXY-HW[1010111101111001](5,13,9)
  While[1000100101001101](1,3,11)
    Dec[0101101000001011](9,3,9)
    IsValidOppXY-HW[1011100100010010](6,0,2)
    FlipCntXY-HW[0111010111100000](6,2,0)
    Dec[0111101011000100](2,14,9)
    While[1110000111011101](2,12,0)
      FlipCntXY-HW[1110010100101010](5,11,8)
      While[0001000100111001](2,2,8)
        Inc[0111000011100001](0,13,0)
        FrontierCnt-HW[0011010000000011](9,7,10)
        IsValidID-HW[0110110000001010](0,2,8)
        Not[1010010100000011](2,12,8)
        Return[1111000000101001](2,6,7)
        Close[0110000001011100](6,14,0)
      Output[0010100000111111](0,15,1)
      AdjacentXY[0011110001100101](4,2,4)
      While[1001110110001010](2,0,9)
        Return[0110010011011010](15,6,12)
        IsValidOppID-HW[0111011101111101](13,5,2)
        Not[1000001011110110](2,2,0)
        SetMoveID[1110011000100110](0,13,2)

Fn-0010001010111101:
  Pull[1111100001000101](6,12,14)
  Call[0000111101111100](12,2,12)
  SetMoveID[1100000100100111](15,7,1)
  AdjacentID[0100001001011101](4,6,13)
  TestNEqu[0010011101111111](4,0,10)
  FlipCntXY-HW[1110100101101101](4,7,8)
  Call[0101010011110000](11,8,5)
  ValidMoveCnt-HW[1110101111011000](5,12,9)
  OppFlipCntXY-HW[0011100010000100](14,5,4)
  Close[1101110101010110](14,0,2)
  AdjacentXY[0010000010111010](15,3,4)
  ValidMoveCnt-HW[1111100101000001](1,8,14)
  ValidOppMoveCnt-HW[0001111010101100](2,5,5)
  Output[1010010000000101](9,12,3)
  Fork[0110000000111010](4,10,4)
  PlaceOppDiskID-HW[1010111010010011](12,7,10)
  Pull[0100000100100110](6,6,1)
  GetBoardValueID-HW[0100000001000011](2,0,6)
  Mult[1011100010001010](14,10,5)
  While[1000000001000111](1,5,6)
    ResetBoard-HW[1011001100100101](11,7,4)
    GetBoardWidth[0111001111000011](2,11,4)
    While[0101011001110111](2,6,0)
      CopyMem[1011111011011100](15,7,12)
      SetMem[1000010001111001](0,13,11)
      While[1111011000011001](2,9,6)
        Inc[0111110001100000](0,9,13)
        PlaceOppDiskXY-HW[1001011111011001](10,9,13)
        IsValidID-HW[0111011001001011](0,2,13)
        Not[1100100110001110](2,13,2)
        Close[0000011010101011](8,14,11)
      SetMoveID[1010110110011101](0,7,15)
      AdjacentXY[1101010011001111](7,2,5)
      While[0010010110110001](2,0,3)
        Inc[0101000010111110](0,0,3)
        IsValidID-HW[0000001110110110](0,2,9)
        Not[0011101111100101](2,11,8)
        Close[1011110101100110](0,15,8)
      SetMoveID[1110011011111110](0,6,8)

Fn-0000010001110100:
  Sub[1110111100101001](7,6,4)

Fn-1011101100001100:
  TestLess[0000010000101001](2,12,12)
  Nop[1000000111011011](5,9,2)
  Pull[0001011101001100](15,5,5)
  ResetBoard-HW[1010111110111111](13,15,14)
  SetMoveXY[1000000001101100](11,2,2)
  GetBoardValueID-HW[0001101001001111](12,15,4)
  Call[0100010011010100](13,13,4)
  PlaceDiskXY-HW[1110011110011110](14,4,0)
  SetMoveXY[0110001000101001](0,5,7)
  PlaceOppDiskID-HW[0010011101001000](13,12,6)
  IsValidXY-HW[1111110110001111](14,4,3)
  Break[0001110101010110](1,10,12)
  ValidMoveCnt-HW[0101010001100100](10,2,8)
  GetMoveXY[0011001010010000](9,13,6)
  GetBoardValueXY-HW[1101101011100010](14,0,13)
  Fork[1100011110111101](3,5,2)
  GetBoardValueXY-HW[0100011101100101](3,9,0)
  Dec[0011101001000010](2,14,13)
  While[1110001110110100](2,3,0)
    While[1001000011101001](2,2,8)
      Inc[0111011011111000](0,13,3)
      FrontierCnt-HW[0011101100011101](5,7,10)
      IsValidID-HW[0111100010001011](13,2,8)
      Not[1010000001110011](2,12,8)
      SetMem[1100000110000010](6,1,3)
      Close[0001010000011000](11,14,10)
    Output[1110001000111001](2,4,7)
    Close[0111010101101111](6,14,6)
  SetMoveID[0000100000110111](0,15,1)
  PlaceDiskID-HW[1011100100001101](10,5,2)
  Return[1111000111111011](8,13,12)
  Not[0100000001101110](2,2,8)
  SetMoveID[1010011010001101](0,1,2)

$
Fn-1110101110101111:
  Mult[1000010100100010](6,9,15)
  TestEqu[0000011110100111](0,3,15)
  AdjacentXY[0110011010111010](2,15,8)
  PlaceDiskID-HW[1111110011001010](13,3,10)
  TestLess[1011001111010111](0,14,0)
  IsValidID-HW[1001011101110100](8,1,5)
  AdjacentID[0101100101001110](4,0,5)
  While[1101000011011111](2,1,1)
    FlipCntXY-HW[0000100000001110](11,2,10)
    Fork[1001111010000110](12,8,2)
    Inc[1000010101100101](0,1,1)
    Pull[1110000011001101](6,15,0)
    ResetBoard-HW[1101101100111001](15,10,12)
    Inc[0000000110101011](0,14,1)
    IsValidID-HW[1100011010101000](0,2,3)
    Not[0010000111111010](2,3,13)
    Close[1000000100110111](7,0,11)
  SetMoveXY[0100000011011000](5,2,2)
  SetMoveID[1001011101111101](0,14,0)
  SetMem[0111000000110110](2,5,7)
  If[1010011010010111](14,3,3)
    Input[0011110101110111](6,8,14)
    Mod[0010101010010110](5,4,11)
    Close[1000001101000010](1,8,2)
  Inc[0000010000100010](0,4,3)
  While[0100111011001001](2,13,1)
    OppFlipCntXY-HW[1000111000111001](8,3,13)
    IsValidOppID-HW[0100001101101100](4,14,10)
    Inc[1001101011100000](0,4,1)
    IsValidID-HW[0100100100010101](0,2,11)
    Not[0110100101010000](2,15,9)
    Not[0100001111100001](9,13,5)
    Close[0111000011010000](9,5,11)
  SetMoveID[1000100000110100](0,12,14)
  GetBoardWidth[0010011000111111](2,1,14)
  TestLess[1101000000110011](6,14,7)
  Inc[1001011111100000](0,3,11)
  Div[1100001011010100](8,10,4)
  SwapMem[0011100101101011](3,12,9)
  IsOver-HW[1110111110101010](5,2,5)
  Commit[1110101000010100](8,7,7)
  While[1101100011101000](2,2,8)
    Inc[0000110101101100](0,9,10)
    IsValidID-HW[0001011100100000](0,2,4)

Fn-1000001111000010:
  Add[0000100110000011](12,0,10)
  PlaceDiskID-HW[0101110101011000](13,13,11)
  PlaceOppDiskID-HW[0101011110111101](1,5,9)
  ValidMoveCnt-HW[0110111000000001](12,10,3)

Fn-1100001101100111:
  Mult[1001010100100010](6,9,14)
  TestNEqu[1011100101101010](1,1,11)
  PlaceDiskID-HW[1011110010001010](13,3,4)
  TestLess[1000000110010110](3,14,0)
  IsValidID-HW[1001011010110000](8,1,5)
  AdjacentID[0001100101011000](4,12,1)
  Mod[0010011011000100](5,4,11)
  Dec[0111010010001001](9,0,0)
  GetBoardWidth[1110011110011000](5,15,10)
  TestNEqu[0101100110110011](6,14,14)
  Inc[1101011111001000](0,3,11)
  Div[1100011110010100](8,14,4)
  SwapMem[1011100101101011](3,0,3)
  Return[1110111100100010](5,2,5)
  Commit[1110101000010100](8,10,7)
  While[1111110001101010](2,2,6)
    Inc[0100110001000000](0,9,10)
    IsValidID-HW[0000001100100000](0,2,4)

$
Fn-0110011110000001:
  OppFlipCntXY-HW[1001100111010101](14,2,12)
  Input[1110011100101110](9,7,12)
  ValidMoveCnt-HW[0100001000001011](14,13,14)
  IsValidOppXY-HW[0110100101110100](12,4,5)
  Dec[0100010011110100](14,11,5)
  Call[0100001000010111](13,4,5)
  OppFlipCntXY-HW[0101001010010100](10,15,12)
  Return[0100111000011000](8,5,13)
  CopyMem[1011000011001011](2,4,14)
  PlaceOppDiskXY-HW[1010110100000000](2,12,4)
  Fork[1101110111111101](6,7,14)
  ValidOppMoveCnt-HW[0110000011011110](12,15,2)
  TestEqu[0111100110010000](7,4,4)
  CopyMem[1111110010111010](0,15,6)
  AdjacentXY[0011101011110011](0,13,0)
  Inc[1011101010001001](1,12,11)
  ValidOppMoveCnt-HW[0111010100101010](12,15,7)
  While[0000111110101111](4,7,7)
    Nop[1011011110101100](0,9,9)
    Mult[1111000011100000](12,15,4)
    SetMem[0111101101010010](2,15,5)
    IsValidID-HW[0011110011101100](10,15,14)
    Add[1000001000110010](1,14,6)
    Mod[1010010110001000](14,11,1)
    While[1100001110100101](2,13,9)
      Inc[0011000101101000](0,13,11)
      IsValidID-HW[0111010110101001](0,2,6)
      Mod[0111011100010101](12,14,10)
      Call[0000001000110010](6,7,8)
      TestEqu[1000010110001011](2,0,6)
      Not[1111010101011101](2,12,2)
      Commit[0000111011111011](0,3,6)
      Pull[0000000011000100](3,8,12)
      Close[1110000111010111](10,12,1)
    SetMoveID[1100110111110000](0,5,9)

Fn-1000111110111000:
  TestLess[1101000001101010](14,0,13)

Fn-1101011001100011:
  Mult[1011101101111101](11,4,8)
  Mult[1011101101111101](11,4,8)
  ResetBoard-HW[1110000010101010](9,12,11)

Fn-0101000000110010:
  Break[1100001000011110](6,6,15)
  ResetBoard-HW[1010100010100011](10,12,11)

$
Fn-0000010100000000:
  PlaceDiskID-HW[0000110000000101](12,3,0)
  While[1101111011110101](2,12,1)
    Inc[1100010001010100](0,7,8)
    IsValidID-HW[0001101101111001](0,2,6)
    Not[0110011000110100](2,7,6)
    Close[1111011000001010](0,3,7)
  SetMoveID[0101001001100001](0,7,6)
  ValidOppMoveCnt-HW[0100100101110100](2,7,12)
  While[1100111001001101](2,13,3)
    Add[0110001111100100](15,1,11)
    Inc[0010001101100001](0,8,11)
    GetMoveID[1001110101100100](5,1,14)
    OppFlipCntID-HW[1100001010101101](12,15,11)
    Mult[1000111011000101](2,15,11)
    SetMem[0001011001000001](14,12,0)
    TestLess[0001011110000101](5,4,3)
    GetBoardValueXY-HW[1011111111011111](9,9,12)
    IsValidOppXY-HW[0000000100011000](6,15,1)
    IsValidID-HW[1001001001100000](0,2,15)
    Not[1001000100001000](2,8,5)
    Close[0001101010100011](1,6,14)
  Break[1110111101101001](8,4,8)
  TestNEqu[0001010001101000](10,10,13)
  SetMoveID[0011110001011110](0,3,13)
  Div[0101010000000110](3,10,4)
  OppFlipCntID-HW[0110000110011100](3,15,1)
  OppFlipCntID-HW[0001101110100011](0,7,15)
  TestNEqu[0101101000001111](9,9,0)
  ValidOppMoveCnt-HW[1011100000111010](11,14,12)
  IsOver-HW[0011110001010011](2,3,5)
  FlipCntID-HW[1011111100100110](2,6,3)
  AdjacentXY[0101000000100010](11,0,11)
  IsValidXY-HW[0000111100101000](10,9,10)
  GetBoardWidth[0100111101101010](2,0,3)

Fn-0101100111010110:
  Countdown[1101101001111101](14,11,8)
    While[1000110011010101](7,15,7)
      OppFlipCntXY-HW[1001011010111111](12,7,6)
      GetMoveXY[0110111110000000](13,9,10)
      GetMoveXY[1100111110001000](13,9,1)
      EndTurn[0011111100000111](4,5,5)
      Not[0101101111010011](2,4,4)

$
Fn-1000001100010110:
  GetBoardWidth[0010011110010110](11,6,4)
  ValidMoveCnt-HW[1100011101011011](2,4,13)
  SetMem[0001000001010100](5,10,6)
  While[1101111011111101](2,6,3)
    Fork[1110001100100010](1,9,14)
    CopyMem[0000100101110000](2,10,4)
    IsOver-HW[0000111010110100](9,1,2)
    Inc[0000100010010011](0,4,8)
    IsValidID-HW[1011110010000001](0,2,2)
    FrontierCnt-HW[0001000110101110](9,10,10)
    Not[0101010011100010](2,3,11)
    Close[1000101000011111](8,15,9)
  SetMoveID[0100101110001011](0,7,6)
  ValidOppMoveCnt-HW[0100011000100110](5,13,12)
  FrontierCnt-HW[1010000010101110](4,15,11)
  Commit[0011010000001011](10,3,3)
  Output[0101011011110111](15,3,2)
  GetBoardValueID-HW[0101101101011000](2,4,8)
  GetBoardValueXY-HW[0010111110011111](15,2,5)
  SetMem[1100111011111000](2,13,8)
  Break[1001101000100000](3,6,6)
  IsValidID-HW[0000001100111110](2,0,11)
  AdjacentID[1000010101001100](15,1,5)
  SwapMem[1001011000110010](15,10,10)
  While[0011000100110001](2,8,8)
    Inc[0110001011101000](0,14,9)
    Fork[1101111100100101](0,12,14)
    OppFlipCntID-HW[1001011100001110](15,6,9)
    Inc[1100010101010011](0,1,13)
    Inc[0011110100111101](0,7,5)
    ValidOppMoveCnt-HW[0010100101010100](6,3,0)
    Inc[0110100000111011](11,6,15)
    IsValidID-HW[0110110100001111](0,2,9)
    TestLess[1111111001010101](10,11,10)
    Not[1010100110100000](2,2,10)
    AdjacentID[1000110110011001](5,0,12)
    OppFlipCntID-HW[1110000000000100](11,13,13)
    IsOver-HW[0010010100100110](6,10,0)

Fn-1000101100101111:
  ValidMoveCnt-HW[0111001110100001](2,1,7)
  EndTurn[0111101101011000](7,11,7)

Fn-1101101000101100:
  Pull[0001011111010100](13,9,6)
  ValidMoveCnt-HW[0110010100111101](2,1,7)
  Not[0100010010101001](11,14,15)
  While[0011100100101000](2,5,1)
    Add[0000000001000010](7,3,10)
    IsValidID-HW[0100100101110011](15,2,11)
    SetMem[0001001110010110](14,4,7)
    FrontierCnt-HW[1101011011110000](10,9,2)
    Not[1101001011010101](2,7,0)
    GetBoardValueID-HW[0101010001100001](9,9,6)
    Nop[1000101000001001](9,6,14)
    While[0101000111111000](2,11,1)
      ResetBoard-HW[1010101010110110](15,14,1)
      Fork[0100001100000101](5,2,11)
      IsValidXY-HW[0000111111111011](4,2,1)
      PlaceOppDiskXY-HW[0000100001011011](9,10,10)
      Input[1111100101101001](6,2,11)
      Sub[0100001100011000](7,4,7)
      IsValidID-HW[1011100110101111](0,9,8)
      Not[0010011000000000](2,7,0)
      GetMoveID[0110100100010001](7,7,9)
      SwapMem[0101010100010110](11,5,0)
      ValidMoveCnt-HW[1111101001011100](11,14,4)
      TestNEqu[0001110011010111](1,7,7)
      Close[1101100100000001](8,4,8)
    SetMoveID[1011100001001100](15,13,1)
    GetMoveXY[1001101100000000](2,11,5)
    CopyMem[1100011110111110](7,15,3)
    SetMem[0100001101010000](14,1,8)
    Fork[1111011100010010](11,14,12)
    IsValidOppXY-HW[1000010000100101](7,1,12)
    SetMem[0011110011110011](12,5,1)
    EndTurn[0001011101110101](2,13,15)
    PlaceOppDiskXY-HW[0100100111010110](7,4,1)
    Pull[0001111000111010](14,4,15)
    SetMoveXY[1001010101111101](13,14,2)
    GetMoveXY[0111000100011000](9,6,8)
    Break[0010010000110110](15,15,14)
    Countdown[1100010101101100](5,3,3)
      FlipCntID-HW[1010001110011001](10,4,13)
      OppFlipCntID-HW[1000110010101110](0,10,0)

$
