//
// Created by Florian on 30.03.2021.
//

#ifndef CHESSAI_CHESSAI_H
#define CHESSAI_CHESSAI_H
#include "AbstractChessPlayer.h"
#include <atomic>
#include <iostream>
#include <limits>

#define SEARCH_DEPTH 5// recommended 5
#define OPENING_MOVE_LIMIT 4
#define QUIESCENT_SEARCH_LIMIT 10 // recommended 10
#define MAXEVAL 100000

class ChessAI final : public AbstractChessPlayer
{
  public:
    ChessAI(bool isWhite) : AbstractChessPlayer(isWhite){};
    std::string chooseMove(thc::ChessRules board, bool printResults = true);
    std::string multiThreadedSearch(thc::ChessRules board, std::atomic<unsigned int>& nodesSearched,
                                    int& bestEval);

  protected:
  private:
    static int lastEval;

    static int minMax(thc::ChessRules& board, const int depth, int alpha, int beta,
                      std::atomic<unsigned int>& nodesSearched);
    static int quiescentSearch(thc::ChessRules& board, int alpha, int beta,
                               std::atomic<unsigned int>& nodesSearched, int depth);
};

#endif // CHESSAI_CHESSAI_H
