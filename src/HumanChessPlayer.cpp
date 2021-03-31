//
// Created by Florian on 30.03.2021.
//

#include "HumanChessPlayer.h"
#include <iostream>

std::string HumanChessPlayer::chooseMove(thc::ChessRules *board) {
    std::string move;
    std::cout << "Enter your move:";
    std::cin >> move;
    return move;
}

