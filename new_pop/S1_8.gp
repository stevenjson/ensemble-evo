Fn-1000111110010101:
  TestNEqu[1000000000001001](5,0,10)
  Fork[1101110101001111](2,1,15)
  SetMem[1001001010001010](2,12,13)
  While[0011111000100111](2,3,12)
    TestLess[1111001110100111](3,6,11)
    Inc[0000011001101011](0,5,10)
    IsValidID-HW[0110011101100101](0,2,8)
    PlaceOppDiskID-HW[0101101101001111](13,11,11)
    Not[1010101110000010](2,5,2)
    Close[1100001011111110](0,11,12)
  ValidOppMoveCnt-HW[0110111000111000](8,6,14)
  GetBoardValueXY-HW[1100001010010101](3,5,14)
  SetMoveID[0110011111001011](0,8,11)
  TestLess[0110100011010010](10,14,2)
  Inc[0101010000000100](5,10,9)
  ValidMoveCnt-HW[0001010101100010](6,10,6)
  Fork[1000000000100000](2,15,2)
  AdjacentID[1100001100101111](0,9,7)
  GetMoveID[1101101011101101](2,6,0)
  While[0110000011101111](2,9,15)
    Call[1101001001101011](0,3,7)
    SetMoveID[0000101110101010](7,5,0)
    IsValidID-HW[1001110011110001](0,2,9)
    PlaceDiskXY-HW[1001011011011100](4,5,1)
    Input[0010111000001001](3,0,1)
    ResetBoard-HW[0011111110101011](3,11,8)
    SetMoveID[1101000111010111](1,8,0)
    SetMem[1011100010111000](15,0,8)
    While[1100111001010100](10,11,8)
      While[0101100011110111](4,10,1)
        OppFlipCntXY-HW[1000111000101100](2,14,13)
        ResetBoard-HW[1000110011100101](5,15,9)
        OppFlipCntID-HW[0111100100101000](12,4,3)
        SetMoveXY[0100010101101010](10,12,10)
        SwapMem[1111100110001101](9,10,3)
        Fork[0011111111010111](10,12,14)
        Nop[0111110100100111](14,7,3)
        GetBoardValueID-HW[0010010111001010](0,8,12)
        Return[0000100011000111](7,14,10)

Fn-1010000101100101:
  SetMem[0100010000000011](0,14,7)
  Not[0011111010010000](3,15,4)
  Close[1110111110101100](9,6,2)
  ResetBoard-HW[0001000100110101](14,13,7)
  SetMem[0001010100001110](2,11,1)
  While[0111101010111101](2,9,9)
    IsValidID-HW[0001101010000110](1,11,9)
    Inc[0010011010001100](0,15,3)
    IsValidID-HW[0111111001101010](0,2,0)
    Not[1110001110111001](2,13,2)
    Close[1110011110110010](12,4,15)
  SetMoveID[1100001101110101](0,6,1)
  Break[1111111000000110](6,9,14)
  SetMoveID[1001110101100101](0,10,11)
  Not[1101011111111100](2,9,0)
  While[0100100100110101](2,0,13)
    GetMoveID[0000000011110000](3,6,2)
    Inc[1101000100011011](0,9,15)
    Fork[1101000000100101](2,6,0)
    IsValidID-HW[1001000001110011](0,2,5)
    Not[0101110101110000](2,11,13)
    Close[1111110110000001](6,8,7)
  SetMoveID[1111111011010110](0,13,8)

Fn-0001111011101101:
  Pull[0001011010000011](5,10,6)
  Fork[1001101100110111](5,10,10)
  PlaceOppDiskXY-HW[1110110100010111](10,14,12)
  Call[1110001101011001](13,11,12)
  Not[1010010111101010](5,1,13)
  IsValidID-HW[1000111100001111](12,14,15)
  PlaceOppDiskID-HW[1001101010101101](6,3,2)
  ValidMoveCnt-HW[1100011001100100](3,8,11)
  Nop[0000111011011011](13,12,3)
  GetMoveXY[0001010010001010](1,1,12)

Fn-1110110110101101:
  Nop[0001100101110100](10,4,15)
  GetBoardWidth[0010101000001010](0,5,13)
  ResetBoard-HW[1001101001111111](0,11,13)
  Commit[0100111100010000](2,13,11)
  SetMem[1110011010110101](7,9,7)
  IsValidXY-HW[1000110010000011](0,9,13)
  TestLess[0011011100011100](7,11,9)
  FrontierCnt-HW[1110011011110100](3,0,15)
  TestLess[1001101000010111](0,11,13)
  IsValidOppXY-HW[0000001100111010](6,11,15)
  PlaceOppDiskXY-HW[0111100111001000](2,13,11)
  IsValidOppID-HW[0001010101110110](8,14,1)
  IsValidXY-HW[0111110001001011](13,9,13)
  IsValidXY-HW[0110010010111111](10,1,4)
  Add[0000101100011010](7,7,12)
  GetBoardValueID-HW[1011001110001100](10,5,15)
  AdjacentXY[1010011100111000](6,3,10)
  Commit[1110111001001001](6,12,5)
  IsValidOppID-HW[0000111110100111](12,12,1)
  Sub[1001100011010010](3,0,11)
  Inc[0101010110001111](14,5,0)
  IsValidXY-HW[1001101000110111](0,9,13)
  GetBoardValueID-HW[1000000010010001](3,15,6)
  ValidOppMoveCnt-HW[1101010110001100](13,2,9)
  Sub[0011110000010111](15,6,1)
  Dec[1001101010110011](9,15,5)

$
Fn-1101101000000000:
  SetMem[0001001001110100](0,14,8)
  Commit[0000010010110001](4,6,11)
  GetBoardValueID-HW[1110111000000100](0,6,2)
  Fork[0000010101100110](15,4,0)
  Close[0111100001000001](1,8,8)
  SetMem[0101010000000011](2,12,12)
  Countdown[0110000011001000](2,1,13)
    Inc[1101110101010111](0,9,15)
    FlipCntID-HW[0110100011101011](0,2,14)
    Not[0011010111100100](2,0,2)
    IsValidXY-HW[0001001100101110](14,2,12)
    Close[0110100000101001](7,1,7)
  SetMoveID[0010001101110101](0,10,9)
  Dec[1111101110100000](2,11,11)
  Nop[0111100110100000](5,0,1)
  OppFlipCntID-HW[0001001000000010](12,12,1)
  SetMem[0101100010101001](2,9,12)
  GetMoveXY[0100110000001100](6,2,11)
  While[1001010000111001](2,12,15)
    AdjacentXY[1011101010111111](6,8,14)
    ValidOppMoveCnt-HW[1111011111001100](6,14,3)
    OppFlipCntID-HW[1011100111110100](8,15,5)
    While[1110001011010101](2,0,13)
      Inc[0111001010011001](0,1,15)
      IsValidID-HW[1001010000010101](0,2,2)
      Not[1111001110001010](2,12,7)
      Close[0011011111111001](2,2,1)
    SetMoveID[1000110110010010](0,6,6)
    SwapMem[0100101111101000](8,7,8)
    SetMem[0010010001000101](2,15,13)
    Call[0110010000101101](5,5,10)
    IsValidXY-HW[0101100110101001](7,0,10)
    ResetBoard-HW[0001000001010111](7,1,12)
    GetMoveID[1101000101101010](0,4,2)
    AdjacentXY[1011101000100010](1,10,9)
    Not[0011110011111010](2,13,5)

Fn-1111111000111111:
  Sub[1010011100100000](11,10,4)

Fn-1000100010001111:
  Mod[0101110000111111](0,5,13)
  Break[0101100000001001](2,9,5)
  GetMoveID[0000010100001100](15,5,3)
  ResetBoard-HW[1110010011100101](13,10,7)
  Div[0111101000110000](6,15,9)
  FrontierCnt-HW[1111101000011010](15,11,13)
  Fork[1111001100110000](9,9,12)
  SetMem[0000111011110011](1,8,5)
  Fork[0011001001000001](9,6,4)
  PlaceDiskID-HW[0000110000110011](11,11,11)
  ValidOppMoveCnt-HW[0100010000010011](7,3,5)
  SetMem[1011011100111111](2,11,0)
  Commit[0110111100101001](6,11,3)
  Sub[0011101011100000](12,1,14)
  PlaceDiskXY-HW[1110100100110001](1,3,3)
  While[1000011110110010](2,14,9)
    PlaceOppDiskID-HW[0100101001101001](2,6,4)
    Fork[1010001111101010](13,15,14)
    ResetBoard-HW[0100001100101110](8,9,12)
    Mult[1000110101100101](2,11,12)
    ResetBoard-HW[0111110100100000](13,9,9)
    Inc[0100111000110110](0,3,3)
    IsValidID-HW[0110110001000010](0,2,3)
    Not[1101000100100000](2,12,12)
    Close[0000111010111010](0,10,6)
  SetMoveID[1110010000110110](0,1,5)

Fn-1010101000011010:
  Output[1010100110101101](1,15,6)

$
Fn-1111010010011110:
  Div[1100011110101100](15,14,14)
  GetBoardValueID-HW[0111111111100111](2,11,6)
  Return[0000100101000100](6,6,8)
  IsValidID-HW[1110110001111010](7,12,15)
  IsValidXY-HW[1000101000001010](9,14,12)
  CopyMem[1101011011011100](7,12,4)
  PlaceOppDiskXY-HW[1001111011001100](7,4,13)
  Fork[1000000011110011](0,7,15)
  Not[1011101110101000](2,0,2)
  OppFlipCntXY-HW[1010010001110000](4,0,10)
  ValidMoveCnt-HW[0001111110110000](4,1,2)
  Mult[1011110010101001](0,15,8)
  FrontierCnt-HW[0010101001111000](0,13,15)
  AdjacentXY[0101100000110110](8,10,14)
  Inc[0000000001011100](2,11,15)
  While[1001000101101100](2,13,13)
    ValidOppMoveCnt-HW[0010011110100001](15,11,5)
    AdjacentXY[0010000100001010](3,10,0)
    Inc[1100101001110101](14,0,3)
    GetBoardWidth[0110010101001111](7,9,13)
    SwapMem[0000111100010101](14,0,13)
    IsValidID-HW[0001011011100011](0,2,5)
    Div[1000110111100000](14,1,11)
    IsValidOppXY-HW[0010010000101111](3,11,6)
    Not[1111110110111010](2,3,4)
    Close[1000100001100000](1,10,4)
  GetMoveXY[1011001010101001](3,15,0)
  PlaceDiskID-HW[0111101001010100](9,12,10)
  IsValidOppID-HW[0110010011011111](12,6,8)
  Return[1111110000111110](13,6,10)
  Sub[1100111111101101](1,8,7)
  GetBoardWidth[1101101010011011](14,13,14)
  AdjacentXY[0011101011100000](2,15,2)
  Close[0010011110101111](9,3,2)
  GetBoardValueID-HW[1010110000010001](12,7,6)
  SetMoveID[0101110001010101](0,12,12)
  CopyMem[0000001010010010](1,6,6)
  OppFlipCntXY-HW[1111111010000000](0,12,10)

Fn-1001110001101101:
  PlaceDiskID-HW[0111101110110000](14,10,5)
  FrontierCnt-HW[1011000001010110](0,14,2)
  OppFlipCntID-HW[1110011010001010](2,7,3)
  FrontierCnt-HW[1010001111110111](12,10,12)
  PlaceOppDiskID-HW[0110001101010100](0,8,4)
  CopyMem[1100101110011000](3,1,7)
  PlaceOppDiskID-HW[1100001101001010](7,8,6)
  ResetBoard-HW[1011010110101000](14,7,5)
  Dec[0111111000100011](0,10,1)
  Commit[0100010010100000](0,4,9)
  Dec[1000101000110100](5,3,10)
  ValidOppMoveCnt-HW[1010010100001100](9,12,8)
  GetMoveXY[0101100011010111](9,7,12)
  ValidOppMoveCnt-HW[0101011111111001](15,6,2)
  Not[0101101101111000](2,0,9)
  Add[0111100001100100](0,0,0)
  GetMoveID[1110010001000100](4,15,4)
  Break[1101100011111011](1,11,14)
  Sub[1111011100111000](9,8,13)
  Pull[1000010010111011](1,5,15)
  IsValidID-HW[1011100101110101](7,3,1)
  While[0111001001001001](2,6,13)
    Inc[1001101001011111](0,9,8)
    ValidMoveCnt-HW[1100010101101011](7,4,12)
    TestEqu[0001001101010001](15,8,12)
    IsValidID-HW[0001100001101100](1,4,12)
    ValidOppMoveCnt-HW[0011011000000001](7,13,5)
    IsValidID-HW[0110010101000101](0,2,4)
    Not[1011000100011010](2,4,4)
    AdjacentXY[0010010101100100](11,4,10)
    Mult[0101101111100110](13,8,4)
    Close[0110111110110101](0,9,15)
  FlipCntXY-HW[0001111101011110](12,11,8)
  IsValidID-HW[0010101010010010](15,12,13)
  Sub[1001000000001101](8,2,12)
  TestLess[0100110000000101](9,14,4)
  IsOver-HW[1101011001101001](15,12,0)
  SetMoveID[0101001000101100](0,1,1)

Fn-0011110101100100:
  SetMoveID[0100101001100111](7,4,0)

Fn-1011110001010100:
  Div[0111011111000001](15,14,12)
  SetMoveID[0100111011001111](0,5,6)
  FlipCntXY-HW[0011011101000110](6,13,0)
  Close[0110011110100100](2,3,15)
  FlipCntID-HW[1001100101101010](6,6,1)
  IsValidXY-HW[0000001011101000](4,13,11)
  CopyMem[1100000110011110](7,3,4)
  PlaceOppDiskXY-HW[1000010011101111](7,4,13)
  PlaceOppDiskID-HW[1011111111100011](2,0,2)
  PlaceOppDiskXY-HW[1100010011011001](7,0,10)
  ValidMoveCnt-HW[0101011111011000](5,1,2)
  Pull[1110010010110001](13,15,9)
  PlaceDiskXY-HW[1110111000101010](0,15,8)
  FrontierCnt-HW[0110000000110001](0,13,15)
  Mult[1101000111100101](1,2,12)
  AdjacentID[0001100000100000](8,9,14)
  Inc[0000001101111001](2,11,15)
  GetMoveID[0111011010111010](8,2,3)
  GetBoardValueXY-HW[0000001101101101](2,13,13)
  ValidOppMoveCnt-HW[0010010110100001](13,11,10)
  SetMem[0110001100101100](3,10,8)
  Nop[0100010100001101](7,9,4)
  Mod[0000001011010101](7,14,7)
  IsValidID-HW[0011111001111110](0,2,5)
  IsValidOppXY-HW[1101110001111010](6,14,6)
  Not[1011010011111110](2,3,4)
  Nop[1110110011000011](5,10,14)
  SetMoveXY[1111100100010000](3,10,4)
  GetMoveXY[1010000110011001](3,15,0)
  Dec[1011111011100000](3,6,1)
  GetMoveXY[1100101011011000](15,12,10)
  SetMoveID[0110001010001101](15,6,8)
  Sub[0110001101011100](11,11,7)
  TestEqu[0100111010100000](6,10,9)
  GetBoardWidth[1100101011011110](14,11,14)
  Mod[0011000100000101](3,11,12)
  PlaceDiskXY-HW[0110110011010111](10,11,1)
  GetBoardValueID-HW[0111001001110100](1,8,9)
  Close[0110101101110011](9,6,13)
  TestEqu[0011101001000001](12,8,9)
  SetMoveID[1100011101011100](0,12,12)
  CopyMem[0100001010010111](14,6,5)
  ValidOppMoveCnt-HW[1101011001010100](3,1,6)
  OppFlipCntXY-HW[0001111010000000](12,9,10)

$
Fn-0001101100111000:
  SetMem[0000101010101001](0,-1,4)
  GetMoveXY[0100110110101001](14,2,15)
  Break[1100010000101101](5,2,1)
  PlaceOppDiskXY-HW[0101010010000111](11,12,4)
  ResetBoard-HW[1010001000010001](13,1,12)
  ValidMoveCnt-HW[0111000111110010](9,15,14)
  While[0001011011011000](2,10,5)
    Inc[1001010111110010](0,11,2)
    IsValidID-HW[0011100110000101](0,2,5)
    Not[0011011111111100](2,1,14)
    Close[0001001110100110](13,14,0)
  Return[0010011100111110](15,9,2)
  Input[0100100001000001](7,14,7)
  IsValidXY-HW[1000101110111011](7,11,8)
  IsValidOppXY-HW[0101000001000110](5,10,11)
  SetMoveID[0001101111001000](0,15,3)
  SetMem[0000011010011110](3,15,10)
  Call[0110011101100001](14,0,3)
  SetMem[1111110000110011](0,-1,11)
  SetMem[0011110100000101](2,6,0)
  While[1110011111011111](2,14,2)
    Countdown[1000101111010001](2,0,9)
      Inc[0011011000001100](0,5,3)
      IsValidID-HW[0000110011101111](6,15,4)
      IsValidID-HW[0010100110111010](0,2,0)
      Not[1010011111101100](2,15,13)
      SwapMem[0001110110010101](14,1,7)
      Close[1110101100100111](4,1,12)
    Commit[1010010010010110](4,2,0)
    SetMoveID[0011111101000011](0,2,1)
    Return[0110010010101101](11,9,8)
    ValidOppMoveCnt-HW[1001000111010011](11,14,15)
    Div[0100101101100010](1,4,14)
    AdjacentID[1000000111000110](0,0,6)
    Call[0000100010001101](5,6,14)
    IsValidID-HW[0100010110101101](0,2,10)
    GetMoveID[0000101100001000](10,9,7)
    Not[0110100001000110](2,5,13)
    Add[1110011101110000](2,0,0)
    IsValidXY-HW[0001001001001000](12,7,15)
    GetBoardWidth[0011011100011110](3,1,6)
    Call[0111111110001111](14,14,10)
    Close[1001001001110100](15,9,13)
  GetMoveID[0110111100000010](11,7,1)
  SetMoveID[1001000010110001](0,0,2)

Fn-0100001010000000:
  GetMoveID[1011001011001000](2,2,5)
  Add[1010111011100100](2,6,12)
  FrontierCnt-HW[1111101010011011](1,2,2)

Fn-0100101000010011:
  TestNEqu[1011011100111000](7,9,4)
  GetBoardValueID-HW[0011011110110011](11,0,5)
  IsValidOppXY-HW[1100011010110110](6,7,8)
  TestLess[1110000100110001](5,10,7)
  ValidMoveCnt-HW[1010010111000110](15,0,6)
  FlipCntID-HW[1001111011101010](11,1,10)
  PlaceDiskID-HW[0011001000010000](9,2,13)
  TestEqu[1101001001010100](8,10,8)

Fn-1101100001011110:
  GetBoardWidth[0110001110110111](3,13,13)

$
Fn-1110000000010101:
  AdjacentXY[1101110110010011](8,11,11)
  SetMem[1001110111011010](0,-1,4)
  FrontierCnt-HW[1000101100100111](9,5,0)
  Add[1001110011010100](14,8,10)
  Nop[0100111000001101](12,0,15)
  SetMem[1011000010001001](2,10,12)
  While[1001101111010011](2,12,4)
    Inc[0001111011000001](0,14,12)
    IsValidID-HW[0010010010101011](0,2,6)
    Sub[1110100000101111](1,1,11)
    Not[0001110111100011](2,1,5)
    Close[1000110001110011](9,7,1)
  SetMoveID[1011011001100011](0,2,14)
  GetMoveXY[0010001010010111](12,2,13)
  While[0001011000011111](2,12,11)
    TestEqu[1001111000011000](9,6,12)
    Inc[0111010010010011](0,4,3)
    IsValidID-HW[0010001101100001](0,2,10)
    Not[1100011111010010](2,4,0)
    Close[1111010111100000](9,1,4)
  SetMoveID[1001001101001110](0,14,7)
  TestLess[0001111010010000](14,8,2)
  Inc[1100111010100011](0,13,3)
  ValidMoveCnt-HW[0101100111100010](13,7,5)
  SetMem[1000001011100001](2,4,10)
  While[1110110110110000](2,15,9)
    Inc[0100010110000110](0,11,10)
    Mod[1110011101010100](7,0,13)
    IsValidID-HW[1101011100110110](0,2,7)
    Not[1011001011000000](2,12,14)
    Close[0100110110011010](13,8,11)
  Inc[1000110001100011](0,6,3)
  PlaceDiskXY-HW[1101001010111110](5,15,8)
  PlaceOppDiskXY-HW[1011010011101100](12,12,5)
  Pull[0010010001100010](8,2,0)
  OppFlipCntXY-HW[1101000001100011](7,6,10)
  GetBoardValueXY-HW[0110111010011111](9,0,8)
  Nop[1001110111000011](14,3,4)
  GetMoveXY[0011010100110011](8,12,9)
  SetMem[0000011000010000](2,8,6)
  While[0011111111100111](2,5,2)
    Inc[0010100011011010](0,15,14)
    SwapMem[0110011000001000](14,7,15)
    IsValidID-HW[1010111001110000](0,2,14)
    Not[1101001110110100](2,1,5)
    Close[1000011100111111](2,6,12)
  SetMoveID[0100101110110100](0,13,4)

Fn-1010011010011100:
  CopyMem[0101011110110010](10,1,13)
  FrontierCnt-HW[0110100010111011](14,1,15)
  Not[1011011010010000](5,11,2)
  SetMem[1111010010000010](9,1,9)
  SetMem[1001010101100010](6,10,10)
  CopyMem[1111001111001011](0,7,0)
  SetMem[1001011000010110](2,15,6)
  Call[0011000000101111](0,0,4)
  PlaceDiskXY-HW[1001001010010110](5,13,4)
  TestNEqu[1111001011010001](12,0,2)
  Fork[1101011010101011](9,13,1)
  Pull[0011100101100011](8,9,5)
  PlaceOppDiskXY-HW[1010101001001110](15,11,8)
  FrontierCnt-HW[1110010011101011](2,12,6)
  If[0100000100111100](5,9,3)
    Div[1011001010100011](0,15,6)
    PlaceOppDiskXY-HW[0001010000101100](0,6,4)
    PlaceDiskXY-HW[0111001101110110](5,0,11)
    Pull[0111000001101001](7,2,5)
    Break[0101101110110101](0,6,7)
    ValidMoveCnt-HW[1111000001100011](0,12,7)
    PlaceDiskXY-HW[1100001101010100](5,0,15)
    PlaceOppDiskXY-HW[1110101111000001](5,0,5)
    IsOver-HW[0011100011110111](14,15,2)
    Fork[1100011010010111](3,0,5)
    PlaceDiskXY-HW[0010101010000010](15,2,7)
    FrontierCnt-HW[1110110101001100](4,12,13)
    FlipCntID-HW[1000110101100001](14,1,13)
    FlipCntID-HW[0001011100100011](2,1,2)
    SwapMem[1011011010111010](15,7,4)
    IsValidID-HW[0010101101110000](0,15,14)
    Call[0111011110000000](2,1,3)
    OppFlipCntID-HW[1110000011111010](12,10,3)
    Break[0110100110011100](13,0,5)
    SetMem[0100010111110010](6,11,0)
    IsValidOppID-HW[1001101101101110](5,8,3)
    GetBoardValueID-HW[1000001101100110](14,7,10)
    GetBoardWidth[0100101001001101](12,11,4)

Fn-1111011001011100:
  AdjacentID[0010010000101001](8,8,12)
  TestEqu[1011111011101010](12,5,3)
  AdjacentID[0010100001111010](11,9,2)
  PlaceOppDiskID-HW[0101100110010000](8,8,5)
  IsValidID-HW[1000011011110100](15,9,5)
  Break[0011111110110010](9,10,9)
  IsOver-HW[0001001010111111](12,9,12)
  IsValidID-HW[1100001001101101](0,7,0)
  AdjacentID[0000110101111101](2,5,8)
  IsValidOppXY-HW[1111001010100010](7,15,2)
  OppFlipCntID-HW[0011100110110100](2,4,6)
  Break[1011111110110001](11,6,12)
  TestNEqu[1011011000110011](8,2,5)
  Break[0011011110110011](9,10,9)
  IsOver-HW[0001001010101111](10,9,12)
  IsValidID-HW[1100001001100101](5,7,0)
  CopyMem[1111010010100011](7,15,2)
  OppFlipCntID-HW[0011100110110111](2,4,6)
  Close[1011111101110001](11,6,12)
  Pull[0011111000100011](8,6,5)
  PlaceDiskXY-HW[0011111010101101](15,2,8)
  Div[0101001001101011](12,12,2)
  Dec[0101010011000101](13,9,9)
  SetMem[0100110111011010](1,3,0)
  Close[1100000101111101](11,15,11)
  Pull[0011111000000010](8,6,5)
  PlaceDiskXY-HW[0011101010101100](15,2,8)
  GetBoardValueID-HW[0001010011000101](13,9,9)
  SetMem[0100110000011010](7,3,0)
  Close[1100000101111101](11,15,11)
  SetMoveID[1100010001000001](0,13,3)

Fn-1111110111011111:
  SetMoveID[0101110111110001](0,1,3)

$
