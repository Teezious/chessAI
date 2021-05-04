//
// Created by Florian on 30.03.2021.
//
#ifndef CHESSAI_HUMANCHESSPLAYER_H
#define CHESSAI_HUMANCHESSPLAYER_H
#include "AbstractChessPlayer.h"
#include <iostream>
#include "../lib/thc/src/thc.h"

class HumanChessPlayer final : public AbstractChessPlayer
{
  public:
    HumanChessPlayer(bool isWhite) : AbstractChessPlayer(isWhite){};

    std::string chooseMove(thc::ChessRules board, bool printResults = true);
};

#endif // CHESSAI_HUMANCHESSPLAYER_H
