//
// Created by tpztp on 12/05/2021.
//

#include "../lib/thc/src/thc.h"
#include "../test/TestHeader.h"
#include "ChessGame.h"
#include <PerformanceTester.h>

TEST(Unittest,Board_test){
    ChessGame gametest;
    gametest.printBoardState();

}

TEST(unittest,move_test){
    ChessGame gametest;
    gametest.printBoardState();
    string move="a2a4";
    thc::Move mv;
    mv.TerseIn(&gametest.getBoard(),move.c_str());
    gametest.getBoard().PlayMove(mv);
    gametest.printBoardState();
}

TEST(unittest,EndgamesSet_test){
    ChessGame gametest;
    gametest.Customboard(gametest.Endgames2);
    gametest.printBoardState();
}


TEST(unittest,AI_test){
    PerformanceTester tester;
    tester.runPerformanceTest();
}

