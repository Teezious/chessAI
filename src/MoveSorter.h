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
    static void insertionSort(std::vector<thc::Move> &moves, int *moveScores);
    static void quickSort(std::vector<thc::Move> &moves,int *moveScores, int low, int high);
    static int partition(std::vector<thc::Move> &moves, int *moveScores, int low, int high);
    static void swap(std::vector<thc::Move> &moves, int *moveScores, int i, int j);
};

#endif // CHESSAI_MOVESORTER_H
