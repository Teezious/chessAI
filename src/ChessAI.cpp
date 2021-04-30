//
// Created by Florian on 30.03.2021.
//

#include "ChessAI.h"
#include "Evaluation.h"
#include <assert.h>
#include <bits/stdc++.h>
#include <chrono>
#include <tuple>
#include <algorithm>

using namespace std;
using namespace std::chrono;

string ChessAI::chooseMove(thc::ChessRules board)
{
    auto start = high_resolution_clock::now();
    int index;
    std::vector<bool> check;
    std::vector<bool> mate;
    std::vector<bool> stalemate;
    std::vector<int> eval;
    std::vector<string> move;
    std::vector<thc::Move> legalMoves;

    board.GenLegalMoveList(legalMoves, check, mate, stalemate);
    for(int i = 0; i < legalMoves.size(); ++i)
    {
        thc::ChessRules tmpBoard = board;
        tmpBoard.PlayMove(legalMoves[i]);
        move.push_back(legalMoves[i].TerseOut());
        eval.push_back(minMax(tmpBoard, SEARCH_DEPTH - 1, !this->getIsWhite(),
                              numeric_limits<int>::min(), numeric_limits<int>::max()));
    }
    if(this->getIsWhite())
    {
        index = distance(eval.begin(), max_element(eval.begin(), eval.end()));
    }
    else
    {
        index = distance(eval.begin(), min_element(eval.begin(), eval.end()));
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Minmax finished in " << duration.count() << " seconds" << endl;
    cout << "eval: " << eval[index] << " move: " << move[index] << endl;
    return move[index];
}

int ChessAI::minMax(thc::ChessRules& board, const int depth, bool maximize, int alpha, int beta)
{
    // TODO detect check mate
    if(depth == 0)
    {
        return Evaluation::evaluateBoardState(board);
    }
    assert(depth >= 0);
    std::vector<thc::Move> legalMoves;
    std::vector<bool> check;
    std::vector<bool> mate;
    std::vector<bool> stalemate;

    int bestEval = maximize ? numeric_limits<int>::min() : numeric_limits<int>::max();

    board.GenLegalMoveList(legalMoves, check, mate, stalemate);
    if(enableOrderMoves)
        orderMoves(legalMoves, board);
    for(int i = 0; i < legalMoves.size(); ++i)
    {
        thc::ChessRules tmpBoard = board;
        tmpBoard.PlayMove(legalMoves[i]);
        int val;
        string tmp_move = legalMoves[i].TerseOut();
        val = minMax(tmpBoard, depth - 1, !maximize, alpha, beta);

        if(maximize)
        {
            bestEval = max(bestEval, val);
            alpha = max(alpha, val);
        }
        else
        {
            bestEval = min(bestEval, val);
            beta = min(beta, val);
        }
        if(beta <= alpha)
            break;
    }

    return bestEval;
}


void ChessAI::orderMoves(vector<thc::Move> &moves, thc::ChessRules &board) {
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

void ChessAI::insertionSort(vector<thc::Move> &moves, vector<int> &moveScores) {
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
