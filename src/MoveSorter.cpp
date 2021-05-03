//
// Created by Florian on 03.05.2021.
//

#include "MoveSorter.h"
#include "Evaluation.h"

using namespace std;

void MoveSorter::sortMoves(vector<thc::Move>& moves, thc::ChessRules &board) {
    vector<int> moveScores;
    for(thc::Move move : moves) {
        int moveScore = 0;
        char movePiece = board.squares[move.src];
        char capturePiece = board.squares[move.dst];

        //reward capturing enemy pieces
        if(capturePiece != '.' && capturePiece != ' ')
            moveScore += 10 * Evaluation::getWhitePieceValue(capturePiece) - Evaluation::getWhitePieceValue(movePiece);

        //check if there is a possible Promotion
        switch(move.special) {
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
        if(board.AttackedSquare(move.dst, !board.white))
            moveScore -= Evaluation::getWhitePieceValue(movePiece);
        //reward moving out of attacked Square
        if(board.AttackedSquare(move.src, !board.white))
            moveScore += Evaluation::getWhitePieceValue(movePiece);

        moveScores.push_back(moveScore);
    }
    //sort moves, maybe try out a few other sorting algorithms?
    insertionSort(moves,moveScores);
}

void MoveSorter::insertionSort(vector<thc::Move> &moves, vector<int> &moveScores) {
    if(moves.empty() || moveScores.empty())
        return;

    for(int i = 0; i < moveScores.size() - 1; i++) {
        int j = i + 1;
        int tmpScore = moveScores.at(j);
        thc::Move tmpMove = moves.at(j);
        while(j > 0 && tmpScore > moveScores.at(j-1)) {
            iter_swap(moveScores.begin() + j, moveScores.begin() + j - 1);
            iter_swap(moves.begin() + j, moves.begin() + j - 1);
            j--;
        }
        moveScores.at(j) = tmpScore;
        moves.at(j) = tmpMove;
    }
}
