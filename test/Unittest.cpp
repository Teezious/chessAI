//
// Created by tpztp on 12/05/2021.
//

#include "../lib/thc/src/thc.h"
#include "../test/TestHeader.h"
#include "ChessGame.h"
#include <ChessAI.h>
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
TEST(unittest,AImove_test){
    ChessGame gametest;
    gametest.printBoardState();
    ChessAI AI=new ChessAI(true);
    string move=AI.chooseMove(gametest.getBoard());
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


TEST(unittest,PerformanceTester){
    PerformanceTester tester;
    tester.runPerformanceTest();
}

