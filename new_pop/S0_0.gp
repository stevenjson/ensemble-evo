Fn-1111100011100111:
  SetMem[0010011000110101](0,-1,5)
  Inc[0101100110010101](14,1,12)
  GetBoardWidth[1000001101111011](2,9,11)
  GetBoardValueXY-HW[1010011000001001](7,6,13)
  While[1101101101010010](2,0,7)
    Pull[1010100001011010](10,10,14)
    Inc[1000001011010101](0,2,8)
    IsValidID-HW[0001111100010100](0,2,2)
    Not[1100110011010010](2,9,15)
    Close[0101001000000010](10,8,8)
  SetMoveID[0110111101011010](0,10,10)
  GetBoardWidth[1100011110100100](1,5,9)
  GetBoardWidth[0111111000001011](8,10,12)
  ValidOppMoveCnt-HW[1001001101110111](2,3,4)
  GetBoardWidth[0010100111101100](9,3,12)
  While[1101010110101010](2,1,1)
    IsValidOppXY-HW[1010111010011101](12,13,2)
    Inc[0110010111000011](0,5,8)
    IsValidID-HW[1111100101111111](0,2,13)
    Not[1011110011110010](2,14,0)
    Close[1100110100000010](10,13,11)
  FlipCntID-HW[0101011010000111](1,2,10)
  SetMoveID[1110110000001110](0,7,13)
  Fork[1000000100011010](0,1,7)
  While[1001101100100000](6,9,8)
    Call[0010100110111101](11,14,8)
    ValidMoveCnt-HW[0101100100011000](11,14,11)
    TestEqu[1000001110100100](6,9,14)
    OppFlipCntXY-HW[0011101000100011](3,9,4)
    Countdown[1100010110101000](2,4,3)
      Fork[1010000010010000](7,1,9)
      If[1111100100000000](13,3,7)
        GetMoveID[0011010111000001](13,11,1)
        Break[0100111100100010](11,15,8)
        FrontierCnt-HW[0000101110001111](10,6,8)
        FlipCntID-HW[1010111100000000](13,5,9)
        Mult[1011010010000001](9,1,5)
        ResetBoard-HW[0010000011000000](14,7,8)
        PlaceDiskID-HW[0011110000101110](0,15,7)
        OppFlipCntID-HW[1111100100100101](11,0,6)
        Output[0001010110100110](7,3,8)
        AdjacentID[0001001111011111](15,15,5)
        IsValidOppID-HW[0011000000001100](15,4,9)
        IsValidOppID-HW[0111000101000001](1,8,1)
        Dec[1110011101011110](7,0,5)
        Close[1010001101101001](1,7,7)
      Nop[0111011101111010](6,8,12)
      Call[0001001110000011](5,11,6)
      Dec[0110000100100011](1,4,0)

$
Fn-1100001100000101:
  GetBoardValueXY-HW[1001000110001011](0,1,0)
  Not[0010100111110100](1,2,1)
  Not[1001101101111110](2,5,0)
  GetBoardWidth[0001100101101110](15,8,15)
  While[0100011010111110](2,7,5)
    Inc[0110110111100110](0,3,11)
    IsValidID-HW[1101001110110001](0,2,9)
    Not[1101001011001000](2,13,5)
    Close[0100110011000101](11,6,2)
  IsValidXY-HW[1001111110000000](12,2,2)
  FrontierCnt-HW[1101111100111001](2,1,14)
  SetMoveID[0011001110111011](0,12,0)
  ValidMoveCnt-HW[1110001101010000](6,3,8)
  SetMem[1111110010011011](11,4,14)
  While[1101011010001001](2,6,10)
    Inc[0001000111110100](0,10,4)
    IsValidID-HW[0001000001110011](0,2,7)
    Not[0010100100100111](2,7,4)
    TestEqu[1111110111000001](7,15,13)
    Inc[1101111011000100](14,3,1)
    Close[1101110100001010](7,10,5)
  SetMoveID[0000000100100001](0,1,11)

Fn-0101101101100001:
  AdjacentXY[1111001100111011](8,1,0)
  PlaceDiskID-HW[1010101101011000](5,10,8)
  Close[0011100100000100](7,8,5)
  Not[1010110010111000](2,14,0)
  GetBoardWidth[1010100110101111](15,3,15)
  Inc[0110101011011100](0,7,11)
  IsValidID-HW[0100111101101101](0,2,4)
  Close[0111011111011111](11,6,9)
  SetMoveXY[1011001000111000](3,15,12)
  Input[0111010110100010](12,2,10)
  FrontierCnt-HW[0100111110100111](2,2,1)
  ValidMoveCnt-HW[0010000001110000](2,2,2)
  PlaceDiskID-HW[1110111001010000](13,11,14)
  TestLess[0011000100001010](6,13,0)
  TestLess[0111101010111000](10,5,15)
  Input[0101110110001110](14,2,12)
  Countdown[0010110001111001](9,11,4)
    Close[0010011010101110](14,10,5)
  SetMoveID[1001001010110000](0,1,11)

Fn-1100010001010011:
  ValidMoveCnt-HW[1011100010101001](12,14,0)
  ResetBoard-HW[0111000110001101](15,10,14)
  TestNEqu[1110001000111100](2,14,14)
  PlaceOppDiskXY-HW[1101011001010110](7,0,9)
  Fork[1011011111010110](2,12,3)
  Inc[0100010011110000](1,3,15)
  Call[1000011110010011](0,2,7)
  TestLess[0011000000011110](6,6,4)
  SetMoveID[1000100101111000](0,5,11)

Fn-1101001101000001:
  TestLess[1010010100101011](0,1,10)
  PlaceDiskID-HW[1010001001111001](5,10,8)
  Close[0111100110001000](7,8,5)
  FlipCntID-HW[1011011110110000](2,8,0)
  GetBoardWidth[1001100111101111](15,3,15)
  While[0100010110010100](2,15,10)
    TestLess[1010010100101011](0,1,10)
    PlaceDiskID-HW[1000001001111001](5,10,8)
    Close[0111100100101000](7,8,5)
  Not[1011011110110000](2,8,0)
  GetBoardWidth[1001100111101111](15,3,15)
  While[0100010110010100](2,15,10)
    Inc[0111101011011000](9,7,7)
    IsValidID-HW[0100100101111101](15,2,4)
    Close[0111111111010111](11,6,9)
  Input[0110011110101010](12,2,10)
  FrontierCnt-HW[0100101001111100](2,2,1)
  ValidMoveCnt-HW[0010000010010100](2,2,2)
  PlaceDiskID-HW[1110111001010100](0,11,14)
  TestLess[0011000001010110](6,1,0)
  TestLess[0111010110111011](10,13,15)
  Countdown[0010110011111001](9,11,4)
    Close[0011001010101110](14,10,15)
  SetMoveID[0000001100110010](0,1,11)

$
Fn-1011001110011101:
  SetMem[1110000100010111](0,-1,7)
  AdjacentID[1111101001100110](2,6,2)
  SetMem[1101111010110111](2,3,2)
  While[1111000101010011](2,0,10)
    Inc[1010111001110000](0,8,15)
    GetMoveID[0011011110010110](13,15,13)
    IsValidID-HW[0000001000111001](0,2,15)
    Not[0000110100000010](2,0,10)
    Close[1011100110000110](13,2,9)
  SetMoveID[0110101111110101](0,8,7)
  GetBoardValueID-HW[0010000101010101](3,7,8)
  AdjacentID[1010100101111010](2,15,5)
  While[1011111111011001](2,5,15)
    While[0000101110111100](2,9,5)
      Inc[0100111000101000](0,10,12)
      FlipCntID-HW[1001111101101010](13,5,0)
      IsValidID-HW[0100101010001111](0,2,12)
      Not[0011100101100100](2,9,5)
      Close[1001001001010101](14,10,9)
    SetMoveID[0000011000100000](0,11,1)
    FlipCntXY-HW[1101001100000001](1,6,6)
    IsValidXY-HW[1011011010101100](2,11,2)

Fn-1110000100010111:
  Call[1001111110101110](1,2,12)
  Call[1011111110101100](1,10,12)
  IsValidXY-HW[0010110001000011](2,11,1)

Fn-1011001110011101:
  SetMem[1110010110000111](0,-1,7)
  AdjacentID[1011001101100110](2,6,2)
  Div[1111111010110011](2,3,2)
  While[1001000101010011](2,0,6)
    IsOver-HW[1100101110110000](1,8,0)
    Inc[1010100011010000](9,8,15)
    GetMoveID[0101011011110110](13,15,13)
    IsValidID-HW[0000001000101001](0,2,2)
    Not[0011110100000011](2,0,10)
    Close[1001100110000110](13,2,9)
  ValidOppMoveCnt-HW[0111111111100101](0,8,7)
  GetBoardValueID-HW[1010010101010111](3,7,8)
  While[1010101101111110](2,5,5)
    While[1001111011001010](2,5,15)
      While[0000100100111100](2,9,5)
        Inc[0100101001101000](0,10,12)
        FlipCntID-HW[1000111101101010](13,5,0)
        Not[0011100100100100](2,9,5)
        Close[1001101000001101](14,10,9)
      TestEqu[1001110100001000](14,1,15)
      FrontierCnt-HW[0100001100000010](1,6,6)
      IsValidXY-HW[0010111101101101](2,11,2)

Fn-1011001110011101:
  SetMem[0110000100010111](0,-1,7)
  Div[1101111010110011](2,3,2)
  While[1001000101010111](2,7,7)
    GetMoveID[1010100011010000](9,8,15)
    GetMoveID[0101011011110110](13,15,13)
    IsValidID-HW[0000001000101001](0,2,2)
    Close[1011100110001110](13,2,9)
  SetMoveID[0110111110110101](0,8,7)
  GetBoardValueID-HW[1010010111010111](3,7,8)
  While[1010101100111110](2,5,5)
    While[1001101011011010](2,5,15)
      While[0000110110111100](2,9,5)
        FrontierCnt-HW[0100101001101000](0,10,12)
        Break[1001101101111010](13,5,0)
        Not[0011100100100110](2,9,5)
        Close[1001101000011101](14,10,13)
      SetMoveID[0000011000100000](0,11,1)
      FrontierCnt-HW[0100001100000010](1,6,6)
      IsValidXY-HW[0010111001111101](2,11,2)

$
Fn-1011100101110111:
  GetBoardValueXY-HW[1001100010001011](0,12,0)
  Dec[1101000000000100](2,10,9)
  FlipCntXY-HW[0001000110110011](0,12,1)
  Countdown[0101101110001011](2,4,1)
    Inc[1101100111100100](0,13,1)
    IsValidID-HW[1101011010001011](0,2,0)
    Not[1000001001101101](2,0,4)
    Close[1101100101000000](6,7,10)
  IsValidOppID-HW[0100000000110001](4,15,13)
  OppFlipCntID-HW[1010011100001011](13,9,7)
  Not[1000110101010101](12,9,6)
  Pull[0001111100101101](2,1,11)
  SetMoveID[1011111100001000](0,7,1)
  SetMem[1100101100010101](2,6,3)
  Countdown[0011111110110000](2,10,10)
    Inc[0100110100110000](0,2,11)
    Commit[0110101000100100](11,7,14)
    IsValidID-HW[1111011011101111](0,2,10)
    IsOver-HW[1111101110100001](6,0,12)
    Not[1100110000110011](2,6,11)
    Close[0100001010110010](7,14,12)
  SetMoveID[0100100100100010](0,4,14)

Fn-0001110001011001:
  GetBoardValueID-HW[0011101000101011](1,12,10)

Fn-0001111001000011:
  GetBoardValueXY-HW[1111110110000010](0,11,15)
  PlaceDiskXY-HW[0001011010101110](2,9,8)
  Call[0001111000100010](3,1,8)
  Dec[1101010011000001](2,3,9)
  FlipCntXY-HW[0001000000010000](0,6,7)
  Countdown[0001000010100001](0,4,1)
    Close[0000000000101111](0,2,0)
  Countdown[0110100101001010](10,11,10)
    Div[0100110110111100](6,8,14)
    IsValidOppID-HW[0000100101010010](10,4,10)
    PlaceDiskXY-HW[0111010100001111](15,14,4)
    Commit[0111111010100100](8,11,14)
    TestNEqu[0100000010111010](0,7,10)
    Pull[0010011110100101](6,10,10)
    FlipCntXY-HW[1101010101000101](13,15,2)
    TestNEqu[0100100100111010](0,7,13)
    Call[1100011101001111](15,7,5)
    FlipCntXY-HW[1100010111000001](13,1,7)
    Not[1001100110110111](2,5,10)
    Nop[0101110111111010](11,4,11)
    TestLess[1001000000110001](14,9,12)
    SetMoveID[1110111001100010](0,14,7)

Fn-0011110000000011:
  Mult[1101001110011100](11,12,0)
  GetBoardValueXY-HW[1001101010011110](0,15,0)
  FlipCntXY-HW[1001000111010011](0,12,1)
  PlaceOppDiskXY-HW[1110011111001101](5,0,0)
  Countdown[1001100010101101](2,1,0)
    Inc[1100010111110100](10,13,1)
    IsValidID-HW[1001011000001011](0,2,0)
    Call[1010101011100011](5,4,11)
    Close[1011101100000000](6,7,10)
  Dec[1101110100011101](2,10,9)
  FlipCntXY-HW[0000000011100111](0,12,1)
  GetMoveID[1000101010001111](15,4,1)
  Inc[1100010101010111](0,12,1)
  If[1001001101011011](0,2,0)
    SetMoveXY[1010110101101100](14,9,15)
    FlipCntID-HW[0110010011011010](12,5,10)
    SetMoveID[0100000100101011](8,14,14)

$
Fn-1111000100111110:
  AdjacentXY[1111010110101111](0,4,11)
  SwapMem[0111111111000110](10,14,14)
  SetMem[1001010001100001](2,5,2)
  While[1000001100001011](2,4,4)
    Inc[0010111010101000](0,11,15)
    IsValidID-HW[0110010001000111](0,2,15)
    Not[1110000100000010](2,15,2)
    SwapMem[1010001100000010](14,7,7)
    Close[1011111010101011](0,11,5)
  SetMoveID[1100100110111100](0,4,15)
  SetMem[1000110011110111](2,13,3)
  Close[1101000000010100](12,1,6)
  While[0100111001001110](2,13,2)
    FrontierCnt-HW[1001111001101100](11,0,9)
    Inc[1010100000101101](0,5,11)
    IsValidID-HW[1110011110111100](0,2,7)
    Not[0101110001111011](2,8,0)
    While[0001011000000001](2,8,0)
      Inc[0001011111101010](0,13,11)
      IsValidID-HW[0101000111100111](0,2,12)
      Not[1010101010111000](2,14,7)
      While[1000110010011100](2,8,6)
        CopyMem[1000000100110110](11,14,11)
        Inc[1001111101100111](0,3,14)
        FlipCntID-HW[1000010101110110](0,2,6)
        Not[1100111111010100](2,5,3)
        Close[1101100100000100](8,4,4)
      TestLess[0000111000101001](7,4,4)
      FlipCntID-HW[1111100001101010](2,2,11)
      SetMoveID[1001001011000011](0,10,6)
      TestNEqu[1100111110011011](7,8,8)
      FlipCntID-HW[0011101111011101](15,14,5)

$