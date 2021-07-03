#ifndef CHESSAI_CHESSGAME_H
#define CHESSAI_CHESSGAME_H

#include "thc.h"
#include <iostream>

class ChessGame final
{
  public:
    thc::ChessRules& getBoard()
    {
        return board;
    }
    void setBoard(thc::ChessRules& board)
    {
        this->board = board;
    }

    void gameHandler();
    void printBoardState();

  private:
    thc::ChessRules board;
};

#endif // CHESSAI_CHESSGAME_H
