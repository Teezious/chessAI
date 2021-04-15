//
// Created by Florian on 30.03.2021.
//

#include "ChessAI.h"
#include "Evaluation.h"
#include "thread_pool.h"
#include <assert.h>
#include <bits/stdc++.h>
#include <chrono>
#include <iostream>
#include <limits>
#include <memory>
#include <thread>
#include <tuple>

using namespace std;
using namespace std::chrono;
typedef std::shared_ptr<std::thread> ThreadPtr;
class ResultFinder
{
  public:
    using Ptr = std::shared_ptr<ResultFinder>;

    ResultFinder(int init) : mBest(init), resultMtx() {}

    ~ResultFinder() {}

    // Acquire lock before calling this function
    int getBestEval()
    {
        return mBest;
    }

    thc::Move getBestMove()
    {
        return mBestMove;
    }

    // Acquire lock before calling this function
    void setBestEval(const int newBestEval, const thc::Move& newBestMove)
    {
        mBest = newBestEval;
        mBestMove = newBestMove;
    }

    std::mutex& getMutex()
    {
        return resultMtx;
    }

  private:
    int mBest;
    std::mutex resultMtx;
    thc::Move mBestMove;
};

string ChessAI::chooseMove(thc::ChessRules board)
{
    auto start = high_resolution_clock::now();
    std::vector<bool> check;
    std::vector<bool> mate;
    std::vector<bool> stalemate;
    std::vector<int> eval;
    std::vector<string> move;
    std::vector<thc::Move> legalMoves;
    bool isWhite = this->getIsWhite();
    board.GenLegalMoveList(legalMoves, check, mate, stalemate);
    ResultFinder::Ptr result;

    cb::ThreadPool pool(cb::ThreadPool::GetNumLogicalCores());
    if(isWhite)
    {
        result = std::make_shared<ResultFinder>(std::numeric_limits<int>::min());
        for(const thc::Move& move : legalMoves)
        {
            pool.Schedule([&]() {
                thc::ChessRules b = board;
                b.PlayMove(move);
                thc::Move tmp = move;
                int moveEval = minMax(b, SEARCH_DEPTH - 1, !isWhite, result->getBestEval(),
                                      std::numeric_limits<int>::max()); // maybe mutex here
                {
                    std::lock_guard<std::mutex> lock{result->getMutex()};
                    {
                        if(moveEval > result->getBestEval())
                        {
                            result->setBestEval(moveEval, move);
                        }
                    }

                } // Release mutex
            });
        }
    }
    else
    {
        result = std::make_shared<ResultFinder>(std::numeric_limits<int>::max());
        for(const thc::Move& move : legalMoves)
        {
            pool.Schedule([&]() {
                thc::ChessRules b = board;
                b.PlayMove(move);
                thc::Move tmp = move;
                int moveEval = minMax(b, SEARCH_DEPTH - 1, !isWhite,
                                      std::numeric_limits<int>::min(), result->getBestEval()); //maybe mutex here
                {
                    std::lock_guard<std::mutex> lock{result->getMutex()};
                    {
                        if(moveEval < result->getBestEval())
                        {
                            result->setBestEval(moveEval, move);
                        }
                    }
                } // Release mutex
            });
        }
    }

    pool.Wait();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);

    std::cout << "FinalEval: " << result->getBestEval()
              << " FinalMove: " << result->getBestMove().TerseOut() << std::endl;
    cout << "Minmax finished in " << duration.count() << " seconds" << endl;
    return result->getBestMove().TerseOut();
}

int ChessAI::minMax(thc::ChessRules board, const int depth, bool maximize, int alpha, int beta)
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
    for(const thc::Move& move : legalMoves)
    {
        thc::ChessRules b = board;
        b.PlayMove(move);
        int val;
        val = minMax(b, depth - 1, !maximize, alpha, beta);

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
