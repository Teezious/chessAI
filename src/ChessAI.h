//
// Created by Florian on 30.03.2021.
//

#ifndef CHESSAI_CHESSAI_H
#define CHESSAI_CHESSAI_H
#include "AbstractChessPlayer.h"
#include <iostream>
#include <limits>

#define SEARCH_DEPTH 5 // reccomended 5 as 6 currently takes 30second + per move

class ChessAI final : public AbstractChessPlayer
{
  public:
    ChessAI(bool isWhite) : AbstractChessPlayer(isWhite){};
    std::string chooseMove(thc::ChessRules board, bool printResults = true);
    std::string multiThreadedSearch(thc::ChessRules board, unsigned int *nodesSearched, int *bestEval);

  protected:
  private:
    static int minMax(thc::ChessRules& board, const int depth, bool maximize, int alpha, int beta, unsigned int *nodesSearched);
    static int searchCaptures(thc::ChessRules& board, int alpha, int beta);
};

#endif // CHESSAI_CHESSAI_H
