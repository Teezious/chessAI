//
// Created by Florian on 30.03.2021.
//

#include "ChessAI.h"
#include "Evaluation.h"
#include <chrono>
#include <tuple>

using namespace std;
using namespace std::chrono;

string ChessAI::chooseMove(thc::ChessRules *board) {
    auto start = high_resolution_clock::now();
    int eval;
    string move;
    tie(eval, move) = minMax(board, SEARCH_DEPTH, this->getIsWhite(), numeric_limits<int>::min(), numeric_limits<int>::max());

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Minmax finished in " << duration.count() << " seconds" << endl;
    return move;
}

tuple<int, string> ChessAI::minMax(thc::ChessRules *board, int depth, bool maximize, int alpha, int beta, string bestMove /*= ""*/) {
    //TODO detect check mate
    if(depth == 0) {
        return make_tuple(Evaluation::evaluateBoardState(board), bestMove);
    }
    std::vector<thc::Move> legalMoves;
    std::vector<bool> check;
    std::vector<bool> mate;
    std::vector<bool> stalemate;

    int bestEval = maximize ? numeric_limits<int>::min() : numeric_limits<int>::max();

    board->GenLegalMoveList(legalMoves, check, mate, stalemate);
    for(thc::Move mv : legalMoves) {
        thc::ChessRules tmpBoard = *board;
        tmpBoard.PlayMove(mv);
        int val;
        string tmp_move;
        tie(val, tmp_move) = minMax(&tmpBoard, depth--, !maximize, alpha, beta, bestMove);

        if(maximize) {
            if(val > bestEval) {
                bestEval = val;
                bestMove = tmp_move;
            }
            alpha = max(alpha, val);
        }
        else {
            if(val < bestEval) {
                bestEval = val;
                bestMove = tmp_move;
            }
            beta = min(beta, val);
        }
        if(beta <= alpha)
            break;
    }



    return make_tuple(bestEval, bestMove);
}