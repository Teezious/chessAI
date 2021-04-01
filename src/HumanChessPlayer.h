//
// Created by Florian on 30.03.2021.
//
#ifndef CHESSAI_HUMANCHESSPLAYER_H
#define CHESSAI_HUMANCHESSPLAYER_H
#include "AbstractChessPlayer.h"
#include <iostream>

class HumanChessPlayer : public AbstractChessPlayer
{
  public:
    HumanChessPlayer(bool isWhite) : AbstractChessPlayer(isWhite){};

    std::string chooseMove(thc::ChessRules board);
};

#endif // CHESSAI_HUMANCHESSPLAYER_H
