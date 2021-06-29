//
// Created by Florian on 03.05.2021.
//

#include "MoveSorter.h"
#include "Evaluation.h"
#include <iostream>

using namespace std;

void MoveSorter::sortMoves(vector<thc::Move>& moves, thc::ChessRules& board)
{
    if(moves.empty())
        return;
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
    // sort moves, maybe try out a few other sorting algorithms?
    // quickSort(moves, moveScores, 0, moves.size() - 1);
    insertionSort(moves, moveScores);
}

void MoveSorter::insertionSort(vector<thc::Move>& moves, std::vector<int> moveScores)
{
    if(moves.empty() || moveScores.empty())
    {
        return;
    }

    for(int i = 0; i < moves.size() - 1; i++)
    {
        int j = i + 1;
        int tmpScore = moveScores.at(j);
        thc::Move tmpMove = moves.at(j);
        while(j > 0 && tmpScore > moveScores.at(j - 1))
        {
            int swap = moveScores.at(j);
            moveScores.at(j) = moveScores.at(j - 1);
            moveScores.at(j - 1) = swap;
            iter_swap(moves.begin() + j, moves.begin() + j - 1);
            j--;
        }
        moveScores.at(j) = tmpScore;
        moves.at(j) = tmpMove;
    }
}

void MoveSorter::quickSort(vector<thc::Move>& moves, std::vector<int> moveScores, int low, int high)
{
    if(low < high)
    {
        int p = partition(moves, moveScores, low, high);
        quickSort(moves, moveScores, low, p - 1);
        quickSort(moves, moveScores, p + 1, high);
    }
}
int MoveSorter::partition(std::vector<thc::Move>& moves, std::vector<int> moveScores, int low,
                          int high)
{
    int score = moveScores.at(high);
    int i = low - 1;

    for(int j = low; j <= high - 1; j++)
    {
        if(moveScores.at(j) >= score)
        {
            i++;
            swap(moves, moveScores, i, j);
        }
    }
    swap(moves, moveScores, i + 1, high);
    return i + 1;
}

void MoveSorter::swap(std::vector<thc::Move>& moves, std::vector<int> moveScores, int i, int j)
{
    int tmpScore = moveScores.at(i);
    moveScores.at(i) = moveScores.at(j);
    moveScores.at(j) = tmpScore;
    iter_swap(moves.begin() + i, moves.begin() + j);
}
