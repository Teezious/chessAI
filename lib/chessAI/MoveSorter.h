#ifndef CHESSAI_MOVESORTER_H
#define CHESSAI_MOVESORTER_H

#include <thc.h>

class MoveSorter final
{
  public:
    static std::vector<unsigned int> sortMoves(const std::vector<thc::Move>& moves,
                                               const thc::ChessRules& board,
                                               const bool insertion = false);
    // private:
    static std::vector<unsigned int> insertionSort(const std::vector<thc::Move>& moves,
                                                   std::vector<int> moveScores);
    static void quickSort(std::vector<unsigned int>& indices, std::vector<int> moveScores, int low,
                          int high);
    static inline int partition(std::vector<unsigned int>& indices, std::vector<int>& moveScores,
                                int low, int high);
};

#endif // CHESSAI_MOVESORTER_H
