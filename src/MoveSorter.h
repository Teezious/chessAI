//
// Created by Florian on 03.05.2021.
//

#ifndef CHESSAI_MOVESORTER_H
#define CHESSAI_MOVESORTER_H

#include <thc.h>
class MoveSorter final
{
  public:
    static void sortMoves(std::vector<thc::Move> &moves, thc::ChessRules &board);
  private:
    static void insertionSort(std::vector<thc::Move> &moves,int *moveScores);
};

#endif // CHESSAI_MOVESORTER_H
