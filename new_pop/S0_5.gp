Fn-0100110011001111:
  GetBoardValueXY-HW[0111010001011100](1,8,8)
  Add[1000000011111001](7,7,9)
  IsValidOppXY-HW[0110111000010100](0,6,0)
  AdjacentXY[1010010011110011](2,0,8)
  GetMoveID[0110111001111101](2,1,6)
  While[1100000111110000](2,1,10)
    Inc[0010011101111111](0,6,13)
    IsValidID-HW[0001011011100010](0,2,5)
    Not[0111001101100000](2,15,0)
    Close[1111110101100110](6,9,5)
  SetMoveID[0111100001011010](0,10,7)
  GetMoveXY[0000100010000101](2,9,13)
  Close[1001010010101110](7,1,15)
  ValidOppMoveCnt-HW[1011100000011001](2,1,5)
  Inc[1011001110010000](1,2,7)
  While[0101010011101100](2,0,12)
    Div[0010011011010011](2,9,15)
    Inc[0101110111100011](0,5,11)
    IsValidID-HW[1011011110111101](0,2,0)
    Not[1111001101110101](2,13,6)
    SetMem[0100111111010010](15,1,5)
    Close[0111001011101010](9,3,3)
  SetMoveID[0001110001100101](0,5,13)

Fn-1001101010100101:
  GetBoardValueXY-HW[0011000001101001](1,8,9)
  TestLess[1100011011110100](7,5,12)
  IsValidOppXY-HW[0010011001000110](0,8,2)
  Pull[1011101000001000](2,2,6)
  GetMoveID[1011011101101111](2,1,6)
  GetBoardValueID-HW[0101010011011011](7,2,0)
  IsValidID-HW[1011001111010110](0,15,5)
  SetMem[0111111010001110](6,1,5)
  While[0000010010100001](2,0,12)
    Inc[1111110111111111](0,5,11)
    IsValidID-HW[1111011111101010](0,2,10)
    Not[1111001100101001](2,14,6)
    Close[0101101000001111](9,8,3)
  SetMoveID[0101110110010100](9,5,13)

Fn-1110000111001101:
  While[1010110100011011](12,12,13)
    GetBoardValueID-HW[0110010100010101](10,5,2)
    GetMoveID[0011101110111111](11,5,9)
    Break[0010111000011111](5,12,7)
    While[0100010011110000](8,1,10)
      OppFlipCntID-HW[0100010000111011](0,2,13)
      FlipCntXY-HW[0010110000010100](5,2,14)
      Fork[1110111110100010](4,8,13)
      Pull[0001010010110011](0,8,5)
      Not[0110011101110000](5,15,0)
      IsValidID-HW[0011001000101011](0,2,5)
      Dec[1011011111111010](4,11,1)
      Not[1100111100100000](0,15,0)
      Close[0011111011001110](6,9,3)
    PlaceOppDiskXY-HW[1000111000010110](7,8,13)
    SetMoveID[1001011101011010](6,5,7)
    FlipCntXY-HW[1111110000101111](5,5,10)
    Not[0110001110010010](2,15,0)
    Output[0101010001011110](2,15,0)
    Close[0100101010011111](15,9,3)
  PlaceOppDiskXY-HW[1000011001000010](7,8,13)
  SetMoveID[1011110111001000](6,0,7)
  Not[1100010001001000](2,15,0)
  Add[0110011001000001](12,8,5)
  SetMoveXY[0101000111011111](6,9,3)
  Dec[1100000111001010](6,11,4)
  FrontierCnt-HW[1010101010001000](1,13,5)
  Dec[1110000010010110](7,12,7)
  Pull[1101011111111000](0,2,3)
  IsValidID-HW[0011001001111110](1,2,14)
  ValidOppMoveCnt-HW[0010101000100100](2,1,5)
  Inc[0001101000001110](15,15,3)
  ResetBoard-HW[0110011111110111](6,13,0)
  Fork[0011011101111011](2,9,14)
  ResetBoard-HW[1110011101100011](5,4,9)
  GetBoardValueID-HW[1110101000001001](7,14,10)
  Inc[0011110111000000](0,5,15)
  IsValidID-HW[1100001111001010](0,2,0)
  Input[1010011101110011](0,2,0)
  Not[0001001101110111](9,13,6)
  GetMoveXY[0010010011000101](8,9,2)
  SetMem[0010101111010111](15,1,5)

$
Fn-1011100001001001:
  SetMem[0011111001110010](0,-1,14)
  IsValidID-HW[0111010000111010](4,7,6)
  SetMem[1110110010011110](13,14,3)
  While[0100010111001101](13,10,12)
    AdjacentID[1101110001100001](2,9,8)
    IsValidOppID-HW[0101011100011001](12,4,8)
    SetMem[1001011010100100](2,9,13)
    While[1001001001101010](2,4,7)
      Inc[0101101010001001](0,6,5)
      IsValidID-HW[0110011110001101](0,2,9)
      Not[1110111110000111](2,4,6)
      Output[1011110101100100](3,1,8)
      Return[1010010111010100](13,4,0)
      Close[0000101000000011](12,1,9)
    AdjacentID[0101010110101000](15,1,0)
    SetMoveID[1000010110101000](0,2,10)
    Nop[0011110000010000](0,0,5)
    GetBoardValueID-HW[1010011011100111](14,2,13)
    Not[1101011101010110](2,13,9)
    FlipCntXY-HW[1010101001110010](2,14,13)
    ValidMoveCnt-HW[1000010000100100](9,9,11)
    Dec[1110000100011011](14,8,0)
    AdjacentID[1001001010110000](11,13,0)
    Pull[0010000000011111](10,2,3)
    Commit[1000110111010011](7,9,1)
    IsValidXY-HW[1100011001110011](15,7,1)
    GetMoveID[0110000010001110](14,4,0)
    Close[1100110001001100](11,6,3)
  Commit[1110110011101000](11,11,12)
  EndTurn[0011001100011001](3,8,12)
  IsValidOppXY-HW[0011010001111000](5,4,10)
  EndTurn[0011000111100001](0,0,2)
  PlaceOppDiskXY-HW[1100100010111111](13,2,5)
  GetBoardValueID-HW[1111100110101101](10,0,8)
  Dec[0001110100111111](15,2,2)
  ValidMoveCnt-HW[0110000001110111](14,11,4)
  TestNEqu[0111101101111000](6,14,9)
  PlaceDiskID-HW[0010001001110100](0,5,10)
  GetBoardWidth[1010111100001001](2,4,12)
  Output[0000101011101011](6,5,11)
  Dec[0100001100110111](15,14,5)
  GetMoveID[0101001100000000](6,4,3)
  TestEqu[1011101000011101](8,12,11)
  GetMoveXY[0010100010111100](12,8,15)
  AdjacentID[1111001011101101](15,1,7)
  GetBoardWidth[1100111100010110](12,11,12)
  SetMoveID[0101001100110011](4,2,7)
  Nop[1001110011111100](2,14,9)
  GetMoveID[1001100110010101](11,12,4)

Fn-1001110001001011:
  PlaceOppDiskXY-HW[1111011100010011](9,10,5)
  SetMem[1011110100110000](0,-1,14)
  IsValidID-HW[0111011100101010](4,7,5)
  SetMem[1110110011001111](13,3,13)
  While[0000010011011101](11,9,14)
    Nop[0110110111101111](12,0,15)
    AdjacentID[0010111100100000](2,2,2)
    While[1011101011111011](2,4,7)
      Sub[1101011110101011](0,6,5)
      IsValidID-HW[1000001010001101](8,2,12)
      Not[0111110100000111](2,7,14)
      IsValidID-HW[0011110011100010](8,7,4)
      Mod[1010101101110010](11,12,8)
      Output[1010111101000000](3,1,8)
      Return[0010011111110100](13,2,0)
      Nop[1011100011111100](2,14,9)
      If[0111100011100110](3,1,0)
        GetMoveID[1101101010000000](11,12,13)

Fn-1001010011001101:
  SetMem[0011110110110010](2,-1,14)
  IsValidID-HW[0111010100001110](4,2,0)
  SetMem[1011010011100111](2,9,13)
  While[1001101011111010](2,4,7)
    Inc[1111001100001001](0,6,5)
    IsValidID-HW[1000000010001101](8,2,12)
    Not[0111110100000111](2,12,6)
    TestNEqu[0011111011100011](8,7,4)
    Commit[0110010110111010](6,6,14)
    Output[1010100101000000](3,1,8)
    Inc[1101001110101001](0,6,5)
    IsValidID-HW[1000001010001101](8,2,12)
    Not[1110110100000111](2,7,6)
    IsValidID-HW[0011110011100010](8,7,4)
    Output[1000110101000000](3,1,8)
    FlipCntID-HW[0110010111110100](13,2,0)
    Nop[1001110010111110](2,14,9)
    If[0011100001011010](3,1,0)
      ValidMoveCnt-HW[1110100101000000](1,2,0)
      GetBoardWidth[1101100011011001](5,11,0)
      GetMoveID[1000100010101001](11,12,13)

Fn-0001010011001101:
  SetMem[0011010100010010](2,-1,14)
  SetMem[0110110011001111](13,3,3)
  While[0000110110010001](13,9,14)
    SetMem[0110110001001111](13,13,3)
    While[0000010110010101](13,11,14)
      SetMem[1011001011100111](2,9,13)
      While[1001011001111010](13,4,7)
        Inc[0111000100001001](0,6,5)
        IsValidID-HW[1000001010001101](8,2,12)
        Input[1001010010011110](2,14,9)
        If[0111100011011010](3,1,0)
          GetMoveID[1101000110101001](11,12,13)

$
Fn-1101001011010100:
  AdjacentID[0010010100001001](5,3,11)
  Mult[1100001110111001](4,0,2)
  SetMoveID[1100101100011000](0,4,7)
  AdjacentID[0101110110111010](5,1,14)
  FrontierCnt-HW[0001100111000111](3,7,1)
  GetMoveXY[0100001110000001](7,14,9)
  Output[1101011000001101](5,5,3)
  FlipCntXY-HW[0000011110111000](6,10,11)
  Pull[1110010011100100](2,7,9)
  Mod[1101100101011011](6,5,11)
  GetBoardWidth[1000010110110001](14,4,7)
  Mod[1101100101011011](6,5,11)
  GetBoardWidth[1000010110110001](14,4,7)
  TestNEqu[1100100111110010](8,0,2)
  FrontierCnt-HW[1001011001111011](12,10,6)
  Sub[0001010011011110](5,6,6)
  Output[1011000100110101](6,6,15)
  Input[1111010100100000](2,9,3)
  AdjacentXY[1000111000011010](13,0,9)
  Inc[0011101000011000](3,9,6)
  Mod[0100000111110111](4,4,0)
  IsValidID-HW[1100010101001111](15,2,9)
  PlaceOppDiskXY-HW[1100000110110111](3,7,8)
  Not[0011101101101100](2,14,1)
  FlipCntID-HW[1000001101111000](13,0,7)
  IsValidOppID-HW[1110001100111001](9,6,6)
  IsValidID-HW[1101100101010011](11,11,7)
  IsValidOppXY-HW[1100110101000010](12,7,13)
  FlipCntID-HW[0101010000010001](4,3,11)
  Output[1100100000111010](3,2,15)
  Mod[1100100001010101](10,12,0)
  IsValidID-HW[0011101100110111](1,11,8)
  GetMoveID[0110111111100011](14,7,13)
  PlaceDiskXY-HW[1100100010100001](14,5,8)
  IsValidOppXY-HW[1100110000001010](7,9,7)
  Div[0100110011000011](0,11,12)
  ResetBoard-HW[1100011111111110](12,11,10)
  AdjacentXY[0100000101011111](0,2,1)
  SwapMem[1110101101101010](3,6,4)
  GetMoveXY[1101001111010100](4,15,2)
  OppFlipCntXY-HW[0100011110110100](7,14,14)
  GetMoveID[0100010011100010](10,6,11)
  FrontierCnt-HW[0011000100111001](1,12,8)
  AdjacentID[1110000100001001](2,8,2)
  While[0101111111010000](2,3,14)
    Inc[1010100101100011](0,12,12)
    IsValidID-HW[1000111001010011](0,2,5)
    Not[1111101100000001](2,4,14)
    Return[1111101110110110](9,2,1)

Fn-1101001011010100:
  AdjacentID[0010010100001001](5,3,11)
  Mult[1100001110111001](4,0,2)
  SetMoveID[1100101100011000](0,4,7)
  AdjacentID[0111000110111010](5,1,14)
  FrontierCnt-HW[0001100111000111](3,7,1)
  GetMoveXY[0100001110000001](7,14,9)
  Output[1101111000001101](5,5,3)
  FlipCntXY-HW[0000011110111000](12,10,11)
  Pull[1110010011100100](2,7,9)
  Mod[1101100101011011](6,5,11)
  GetBoardWidth[1000010110110001](14,4,7)
  Mod[1101100101011011](6,5,11)
  GetBoardWidth[1000010110110001](14,4,7)
  TestNEqu[1100100111110010](8,0,2)
  FrontierCnt-HW[1001011001111011](12,10,6)
  Sub[0001010011011110](5,6,6)
  Output[1011000100110101](6,6,15)
  Input[1111010100100000](2,9,3)
  AdjacentXY[1000111000011010](13,0,9)
  Inc[0011101100011000](3,9,6)
  Mod[0100000111110111](4,4,0)
  IsValidID-HW[1100110101001111](15,2,9)
  PlaceOppDiskXY-HW[1100000110110111](3,7,8)
  Not[0101101101101100](2,14,1)
  FlipCntID-HW[1000001101111000](13,0,7)
  IsValidOppID-HW[1110001100111001](1,6,6)
  IsValidID-HW[1101100101010011](11,11,7)
  IsValidOppXY-HW[1100110101000010](12,7,13)
  FlipCntID-HW[0101010000010001](4,3,11)
  Output[1100000000111010](3,2,15)
  Mod[1100100001010101](10,12,0)
  IsValidID-HW[0011101100110111](1,11,8)
  GetMoveID[0110111110100011](14,7,13)
  PlaceDiskXY-HW[1100100010100001](14,5,8)
  IsValidOppXY-HW[1100110000001010](7,9,7)
  Div[0100110011000011](0,11,12)
  ResetBoard-HW[1100011111111110](12,11,10)
  SwapMem[1110101101101010](3,6,4)
  GetMoveXY[1101001011010100](4,15,2)
  OppFlipCntXY-HW[0100011110110100](7,14,14)
  GetBoardValueID-HW[1110111101100000](3,1,12)
  GetMoveID[0100010011100010](10,6,11)
  FrontierCnt-HW[0011000100111001](1,12,8)
  AdjacentID[1110000100001001](2,8,2)
  While[0101111111010000](2,3,14)
    Inc[1010100101000011](0,12,12)
    IsValidID-HW[1000111001010011](0,2,5)
    Not[1111101100000001](2,4,14)
    Return[1111101110110110](9,2,1)

$
Fn-0001100101110101:
  Mod[1000100101001001](11,9,7)
  SetMem[1110010101010010](2,2,14)
  While[0100101011101010](2,9,3)
    Inc[0101100110001000](0,15,3)
    IsValidID-HW[1111011110101001](0,2,7)
    Not[0001000001101011](2,10,4)
    Close[1110100000000101](0,13,0)
  SetMoveID[0011110001111111](0,11,14)
  SetMem[1000110100000011](4,3,2)
  Nop[1010101111100000](10,3,1)
  AdjacentXY[0011111010000101](2,12,12)
  While[0100001111100100](2,13,7)
    ValidOppMoveCnt-HW[0100000110110111](7,5,13)
    FrontierCnt-HW[1000110110111111](13,9,3)
    While[1000101011110100](2,8,3)
      Div[1001011101101011](3,14,13)
      AdjacentXY[0011111110011010](0,4,0)
      SetMem[1110100100111111](5,14,9)
      If[1010010000010100](4,6,10)
        OppFlipCntXY-HW[1101110100100010](13,15,8)
        While[0010010110110001](2,3,4)
          Inc[0001101010110100](0,6,15)
          FlipCntID-HW[0110101001001100](0,2,4)
          Not[0010010010000110](2,7,5)
          Close[0110111000101100](9,14,8)
        SetMoveID[1000110000001111](0,2,12)

$
Fn-0000111110111001:
  GetMoveXY[0010101100010001](15,12,15)
  PlaceOppDiskID-HW[1100000100011001](14,7,3)
  SetMem[0111001011001100](0,-1,3)
  Input[0001110101010011](15,3,9)
  AdjacentXY[1000011100100000](12,15,13)
  While[1111101001101110](2,15,11)
    ResetBoard-HW[0000101000011101](11,4,3)
    While[0110010100010000](2,5,2)
      Inc[0110010000001110](0,5,11)
      IsValidID-HW[1110111001000011](0,2,1)
      Dec[1110010111111111](2,5,9)
      TestLess[1101100000010011](3,9,8)
      IsValidOppXY-HW[0010110111110010](8,12,9)
      Close[0100000010100101](7,0,14)
    SetMoveID[1010011010110100](0,3,1)
    Inc[0110111010010000](0,8,0)
    IsValidID-HW[0100001110001011](0,2,6)
    Not[1010110000011011](2,14,1)
    ResetBoard-HW[1001101111100100](12,10,7)
    Close[0000000010001111](7,4,3)
  PlaceOppDiskID-HW[1000101001011111](2,11,4)
  GetMoveID[1110010111001011](2,8,10)
  SetMoveID[0011010000100110](0,9,0)

$
