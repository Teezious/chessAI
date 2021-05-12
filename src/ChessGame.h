//
// Created by Florian on 30.03.2021.
//

#ifndef CHESSAI_CHESSGAME_H
#define CHESSAI_CHESSGAME_H

#include "../lib/thc/src/thc.h"
#include <iostream>

class ChessGame final
{
  public:
    thc::ChessRules& getBoard()
    {
        return board;
    }

    ChessGame();
    char *Endgames1;
    char *Endgames2;
    ChessGame(char* endgames2);

    void setBoard(thc::ChessRules& board)
    {
        this->board = board;
    }

    void gameHandler();
    void printBoardState();
    void Customboard(char *Customboard);

  private:
    thc::ChessRules board;
};

#endif // CHESSAI_CHESSGAME_H
