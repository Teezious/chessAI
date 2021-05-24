//
// Created by Florian on 03.05.2021.
//

#include "MoveSorter.h"
#include "Evaluation.h"
#include <iostream>

using namespace std;

void MoveSorter::sortMoves(vector<thc::Move>& moves, thc::ChessRules &board) {
    if(moves.empty())
        return;

    int moveScores[moves.size()];
    for(int i = 0; i < moves.size(); i++) {
        int moveScore = 0;
        char movePiece = board.squares[moves[i].src];
        char capturePiece = board.squares[moves[i].dst];

        //reward capturing enemy pieces
        if(capturePiece != '.' && capturePiece != ' ')
            moveScore += 10 * Evaluation::getWhitePieceValue(capturePiece) - Evaluation::getWhitePieceValue(movePiece);

        //check if there is a possible Promotion
        switch(moves[i].special) {
            case 6: //Queen Promotion
                moveScore += Evaluation::getWhitePieceValue('Q');
                break;
            case 7: //Rook Promotion
                moveScore += Evaluation::getWhitePieceValue('R');
                break;
            case 8: //Bishop Promotion
                moveScore += Evaluation::getWhitePieceValue('B');
                break;
            case 9: //Knight Promotion
                moveScore += Evaluation::getWhitePieceValue('N');
                break;
            default:
                break;
        }
        //punish moving on a attacked square
        if(board.AttackedSquare(moves[i].dst, !board.white))
            moveScore -= Evaluation::getWhitePieceValue(movePiece);
        //reward moving out of attacked Square
        if(board.AttackedSquare(moves[i].src, !board.white))
            moveScore += Evaluation::getWhitePieceValue(movePiece);

        moveScores[i] = moveScore;
    }
    //sort moves, maybe try out a few other sorting algorithms?
    //insertionSort(moves, moveScores);
    quickSort(moves,moveScores, 0, moves.size() - 1);
}

void MoveSorter::insertionSort(vector<thc::Move> &moves, int *moveScores) {
    for(int i = 0; i < moves.size() - 1; i++) {
        int j = i + 1;
        int tmpScore = moveScores[j];
        thc::Move tmpMove = moves.at(j);
        while(j > 0 && tmpScore > moveScores[j-1]) {
            int swap = moveScores[j];
            moveScores[j] = moveScores[j-1];
            moveScores[j-1] = swap;
            iter_swap(moves.begin() + j, moves.begin() + j - 1);
            j--;
        }
        moveScores[j] = tmpScore;
        moves.at(j) = tmpMove;
    }
}

void MoveSorter::quickSort(vector<thc::Move> &moves, int *moveScores, int low, int high) {
    if(low < high) {
        int p = partition(moves, moveScores, low, high);
        quickSort(moves, moveScores, low, p - 1);
        quickSort(moves, moveScores, p + 1, high);
    }
}
int MoveSorter::partition(std::vector<thc::Move>& moves, int *moveScores, int low, int high)
{
    int score = moveScores[high];
    int i = low - 1;

    for(int j = low; j <= high - 1; j++) {
        if(moveScores[j] >= score) {
            i++;
            swap(moves, moveScores, i, j);
        }
    }
    swap(moves, moveScores, i+1, high);
    return i+1;
}

void MoveSorter::swap(std::vector<thc::Move> &moves, int *moveScores, int i, int j) {
    int tmpScore = moveScores[i];
    moveScores[i] = moveScores[j];
    moveScores[j] = tmpScore;
    iter_swap(moves.begin() + i, moves.begin() + j);
}
