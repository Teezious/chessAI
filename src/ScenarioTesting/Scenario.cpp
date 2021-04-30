//
// Created by Florian on 29.04.2021.
//

#include "Scenario.h"
void Scenario::createNewBoard(std::string boardString) {
    strcpy(board.squares, boardString.c_str());
}
void Scenario::aiTest() {
    this->chessAI.chooseMove(board);
}
