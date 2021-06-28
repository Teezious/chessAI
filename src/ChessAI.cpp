//
// Created by Florian on 30.03.2021.
//

#include "ChessAI.h"
#include "Evaluation.h"
#include "thread_pool.h"
#include <assert.h>
//#include <bits/stdc++.h>
#include "MoveSorter.h"
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

    ResultFinder(int init) : mBestEval(init), moveCount(0), resultMtx() {}

    ~ResultFinder() {}

    // Acquire lock before calling this function
    int getBestEval()
    {
        return mBestEval;
    }

    thc::Move getBestMove()
    {
        return mBestMove;
    }

    // Acquire lock before calling this function
    void setBestEval(const int newBestEval)
    {
        mBestEval = newBestEval;
    }

    void setBestMove(const thc::Move& newBestMove)
    {
        mBestMove = newBestMove;
    }

    void moveIncrement()
    {
        ++moveCount;
    }
    int getMoveCount()
    {
        return moveCount;
    }
    std::mutex& getMutex()
    {
        return resultMtx;
    }

  private:
    int mBestEval; // best evaluation
    int moveCount;
    std::mutex resultMtx; // saves best move
    thc::Move mBestMove; // get this mutex before getting or setting new best move
};

string ChessAI::chooseMove(thc::ChessRules board, bool printResults)
{
    auto start = high_resolution_clock::now();
    static std::atomic<unsigned int> nodesSearched = 0;
    int eval;
    string move = multiThreadedSearch(board, &nodesSearched, &eval);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    if(printResults)
    {
        cout << "Minmax finished in " << duration.count() / 1000.0 << " seconds" << endl;
        cout << "eval: " << eval << " move: " << move << endl;
        cout << "Nodes searched: " << nodesSearched << endl;
    }
    return move;
}

int ChessAI::minMax(thc::ChessRules& board, const int depth, bool maximize, int alpha, int beta,
                    std::atomic<unsigned int>* nodesSearched, int lastEval)
{
    // TODO detect check mate

    thc::DRAWTYPE eval_draw;
    board.IsDraw(maximize, eval_draw);
    if(eval_draw == thc::DRAWTYPE_50MOVE || eval_draw == thc::DRAWTYPE_REPITITION ||
       eval_draw == thc::DRAWTYPE_INSUFFICIENT_AUTO)
    {
        return 0;
    }
    // else if(board.GetRepetitionCount() > 0 && lastEval > 100)
    // {
    //     return 0;
    // }

    thc::TERMINAL eval_final_position;
    board.Evaluate(eval_final_position);
    if(eval_final_position != 0)
    {
        if(eval_final_position == 2 && eval_final_position == -2)
        {
            return 0;
        }
        else if(eval_final_position == 1)
        {
            return MAXEVAL + depth;
        }
        else
        {
            return -MAXEVAL - depth;
        }
    }
    if(depth == 0)
    {
        return quiescentSearch(board, alpha, beta, nodesSearched, -depth, maximize, lastEval);
    }

    (*nodesSearched)++;
    std::vector<thc::Move> legalMoves;
    std::vector<bool> check;
    std::vector<bool> mate;
    std::vector<bool> stalemate;

    int bestEval = maximize ? numeric_limits<int>::min() : numeric_limits<int>::max();

    board.GenLegalMoveList(legalMoves, check, mate, stalemate);
    MoveSorter::sortMoves(legalMoves, board);
    for(const thc::Move& move : legalMoves)
    {
        thc::ChessRules b = board;
        b.PlayMove(move);
        int val;
        val = minMax(b, depth - 1, !maximize, alpha, beta, nodesSearched, lastEval);

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

std::string ChessAI::multiThreadedSearch(thc::ChessRules board,
                                         std::atomic<unsigned int>* nodesSearched, int* bestEval)
{
    std::vector<bool> check;
    std::vector<bool> mate;
    std::vector<bool> stalemate;
    std::vector<int> eval;
    std::vector<string> move;
    std::vector<thc::Move> legalMoves;
    bool isWhite = this->getIsWhite();
    board.GenLegalMoveList(legalMoves, check, mate, stalemate);
    MoveSorter::sortMoves(legalMoves, board);
    ResultFinder::Ptr result; // result will be saved here
    int expectedMoveCount = legalMoves.size();
    int cores = (int)(cb::ThreadPool::GetNumLogicalCores()) * 2 / 3;

    cb::ThreadPool pool(1); // generates thread pool
    if(isWhite)
    {
        result = std::make_shared<ResultFinder>(
            std::numeric_limits<int>::min()); // min initialisze for maximizer
        for(const thc::Move& move : legalMoves)
        {
            pool.Schedule([&]() {
                thc::ChessRules b = board;
                b.PlayMove(move); // push every possible move
                int moveEval = minMax(b, SEARCH_DEPTH - 1, !isWhite, result->getBestEval(),
                                      std::numeric_limits<int>::max(), nodesSearched,
                                      lastEval); // maybe mutex here
                {
                    std::lock_guard<std::mutex> lock{result->getMutex()};
                    {
                        if(moveEval > result->getBestEval()) // check if new best
                        {
                            result->setBestEval(moveEval);
                            result->setBestMove(move);
                        }
                        result->moveIncrement();
                    }

                } // Release mutex
            });
        }
    }
    else
    {
        result = std::make_shared<ResultFinder>(
            std::numeric_limits<int>::max()); // max initialisze for minimizer
        for(const thc::Move& move : legalMoves)
        {
            pool.Schedule([&]() {
                thc::ChessRules b = board;
                b.PlayMove(move);
                int moveEval =
                    minMax(b, SEARCH_DEPTH - 1, !isWhite, std::numeric_limits<int>::min(),
                           result->getBestEval(), nodesSearched, lastEval); // maybe mutex here
                {
                    std::lock_guard<std::mutex> lock{result->getMutex()};
                    {
                        if(moveEval < result->getBestEval()) // check if new best
                        {
                            result->setBestEval(moveEval);
                            result->setBestMove(move);
                        }
                        result->moveIncrement();
                    }
                } // Release mutex
            });
        }
    }

    pool.Wait(); // wait for all threds to finish
    while(result->getMoveCount() != expectedMoveCount)
    {
        this_thread::sleep_for(5ms);
    }
    *bestEval = result->getBestEval();
    lastEval = abs(result->getBestEval());
    return result->getBestMove().TerseOut();
}

int ChessAI::quiescentSearch(thc::ChessRules& board, int alpha, int beta,
                             std::atomic<unsigned int>* nodesSearched, int depth, bool isWhite,
                             int lastEval)
{
    thc::DRAWTYPE eval_draw;
    board.IsDraw(isWhite, eval_draw);
    if(eval_draw == thc::DRAWTYPE_50MOVE || eval_draw == thc::DRAWTYPE_REPITITION ||
       eval_draw == thc::DRAWTYPE_INSUFFICIENT_AUTO)
    {
        return 0;
    }
    // else if(board.GetRepetitionCount() > 0 && lastEval > 100)
    // {
    //     return 0;
    // }

    thc::TERMINAL eval_final_position;
    board.Evaluate(eval_final_position);
    if(eval_final_position != 0)
    {
        if(eval_final_position == 2 || eval_final_position == -2)
        {
            return 0;
        }
        else if(eval_final_position == 1)
        {
            return MAXEVAL + depth;
        }
        else
        {
            return -MAXEVAL - depth;
        }
    }
    (*nodesSearched)++;

    int val;
    int bestEval = Evaluation::evaluateBoardState(board);
    // if(depth == 0)
    // {
    //     return standPat;
    // }

    // gen moves
    std::vector<thc::Move> moves;
    std::vector<bool> check;
    std::vector<bool> mate;
    std::vector<bool> stalemate;
    board.GenLegalMoveList(moves, check, mate, stalemate);

    MoveSorter::sortMoves(moves, board);
    int i = 0;
    for(const thc::Move& mv : moves)
    {
        // skip moves that have no capture
        if(mv.capture == 32 && check[i] == false)
        {
            ++i;
            continue;
        }

        thc::ChessRules b = board;
        b.PlayMove(mv);
        int val = quiescentSearch(b, beta, alpha, nodesSearched, depth - 1, !isWhite, lastEval);

        if(isWhite)
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
        {
            break;
        }
        ++i;
    }

    return bestEval;
}
