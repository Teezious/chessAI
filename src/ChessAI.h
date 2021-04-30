//
// Created by Florian on 30.03.2021.
//

#ifndef CHESSAI_CHESSAI_H
#define CHESSAI_CHESSAI_H
#include "AbstractChessPlayer.h"
#include <iostream>
#include <limits>

#define SEARCH_DEPTH 5

class ChessAI : public AbstractChessPlayer
{
  public:
    ChessAI(bool isWhite) : AbstractChessPlayer(isWhite){};
    ChessAI() :AbstractChessPlayer(){};
    std::string chooseMove(thc::ChessRules board);
    void orderMoves(std::vector<thc::Move> &moves, thc::ChessRules &board);
    bool enableOrderMoves = true;
  protected:
  private:
    int minMax(thc::ChessRules& board, const int depth, bool maximize, int alpha, int beta);
    void insertionSort(std::vector<thc::Move> &moves, std::vector<int> &moveScores);
};

#endif // CHESSAI_CHESSAI_H
