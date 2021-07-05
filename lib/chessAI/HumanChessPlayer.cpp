#include "HumanChessPlayer.h"
#include <algorithm>
#include <iostream>

std::string HumanChessPlayer::chooseMove(thc::ChessRules, bool printResults)
{
    std::string move;
    std::cout << "Enter your move:";
    std::cin >> move;
    return move;
}
