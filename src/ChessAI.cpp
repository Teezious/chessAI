//
// Created by Florian on 30.03.2021.
//

#include "ChessAI.h"
#include "Evaluation.h"
#include <assert.h>
#include <bits/stdc++.h>
#include <chrono>
#include <tuple>

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