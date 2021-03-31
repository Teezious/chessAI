//
// Created by Florian on 30.03.2021.
//
#include "../lib/thc-chess-library/src/thc.h"

#ifndef CHESSAI_ABSTRACTCHESSPLAYER_H
#define CHESSAI_ABSTRACTCHESSPLAYER_H

#include <string>

class AbstractChessPlayer {
  public:
    AbstractChessPlayer(bool isWhite) {this->isWhite = isWhite; }

    bool getIsWhite() { return isWhite; }
    void setIsWhite(bool isWhite) { this->isWhite = isWhite; }

    virtual std::string chooseMove(thc::ChessRules *board) = 0;

  private:
    bool isWhite;
};

#endif // CHESSAI_ABSTRACTCHESSPLAYER_H
