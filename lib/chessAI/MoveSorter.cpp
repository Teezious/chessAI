#include "MoveSorter.h"
#include "Evaluation.h"
#include <assert.h>
#include <iostream>
#include <numeric>

using namespace std;
// returns vector with sorted indeces
std::vector<unsigned int> MoveSorter::sortMoves(const vector<thc::Move>& moves,
                                                const thc::ChessRules& board,const  bool insertion)
{
    if(moves.empty())
        return {};

    std::vector<int> moveScores;
    for(const thc::Move& mv : moves)
    {
        int moveScore = 0;
        char movePiece = board.squares[mv.src];
        char capturePiece = board.squares[mv.dst];

        // reward capturing enemy pieces
        if(capturePiece != '.' && capturePiece != ' ')
            moveScore += 10 * Evaluation::getWhitePieceValue(capturePiece) -
                         Evaluation::getWhitePieceValue(movePiece);

        // check if there is a possible Promotion
        switch(mv.special)
        {
            case 6: // Queen Promotion
                moveScore += Evaluation::getWhitePieceValue('Q');
                break;
            case 7: // Rook Promotion
                moveScore += Evaluation::getWhitePieceValue('R');
                break;
            case 8: // Bishop Promotion
                moveScore += Evaluation::getWhitePieceValue('B');
                break;
            case 9: // Knight Promotion
                moveScore += Evaluation::getWhitePieceValue('N');
                break;
            default:
                break;
        }
        // punish moving on a attacked square
        if(board.AttackedSquare(mv.dst, !board.white))
            moveScore -= Evaluation::getWhitePieceValue(movePiece);
        // reward moving out of attacked Square
        if(board.AttackedSquare(mv.src, !board.white))
            moveScore += Evaluation::getWhitePieceValue(movePiece);

        moveScores.push_back(moveScore);
    }
    if(insertion){
            return insertionSort(moves, moveScores);
    }else{
            std::vector<unsigned int> indices ( moves.size() );
            std::iota(indices.begin(), indices.end(), 0);
            quickSort(indices, moveScores, 0, moves.size() - 1);
            return indices;
    }
    return {};
}

std::vector<unsigned int> MoveSorter::insertionSort(const vector<thc::Move>& moves,
                                                    std::vector<int> moveScores)
{
    std::vector<unsigned int> indices ( moves.size() );
    std::iota(indices.begin(), indices.end(), 0);

    if(moves.empty() || moveScores.empty())
    {
        return indices;
    }

    for(int i = 0; i < moves.size() - 1; ++i)
    {
        int j = i + 1;
        int tmpScore = moveScores[j];
        thc::Move tmpMove = moves[j];
        while(j > 0 && tmpScore > moveScores[j - 1])
        {
            std::swap(moveScores[j], moveScores[j - 1]);
            std::swap(indices[j], indices[j - 1]);
            j--;
        }
        moveScores[j] = tmpScore;
    }
    return indices;
}

void MoveSorter::quickSort(std::vector<unsigned int>& indices,
                                                std::vector<int> moveScores, int low, int high)
{

    if(low < high)
    {
        int p = partition(indices, moveScores, low, high);
        quickSort(indices, moveScores, low, p - 1);
        quickSort(indices, moveScores, p + 1, high);
    }
}

inline int MoveSorter::partition(std::vector<unsigned int>& indices, std::vector<int>& moveScores,
                          int low, int high)
{
    // assert(0 <= high && high < moveScores.size());
    // assert(0 <= low && low < moveScores.size());
    const int& score = moveScores[high];
    int i = low - 1;

    for(int j = low; j <= high - 1; ++j)
    {
        if(moveScores[j] >= score)
        {
            i++;
          std::swap(moveScores[j], moveScores[i]);
          std::swap(indices[j], indices[i]);
        }
    }
    std::swap(moveScores[i+1], moveScores[high]);
    std::swap(indices[i+1], indices[high]);
    return i + 1;
}

