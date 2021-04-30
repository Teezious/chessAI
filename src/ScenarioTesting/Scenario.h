//
// Created by Florian on 29.04.2021.
//

#ifndef CHESSAI_SCENARIO_H
#define CHESSAI_SCENARIO_H

#include "../lib/thc/src/thc.h"
#include <ChessAI.h>

class Scenario
{
  public:
    thc::ChessRules board;
    ChessAI chessAI;
    void createNewBoard(std::string boardString);
    void aiTest();
};

#endif // CHESSAI_SCENARIO_H
