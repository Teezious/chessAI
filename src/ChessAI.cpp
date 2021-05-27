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
#include "Opening.h"

using namespace std;
using namespace std::chrono;
typedef std::shared_ptr<std::thread> ThreadPtr;
int Rounds=0;
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
    void setBestEval(const int newBestEval, const thc::Move& newBestMove)
    {
        mBestEval = newBestEval;
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

string  ChessAI::Blackopen(int i,int method)
{
        return open->BlackTIG[i];
}

string ChessAI::chooseMove(thc::ChessRules board, bool printResults)
{
    if(Rounds<=1 && getIsWhite()==0){
        string move=Blackopen(Rounds,0);
        Rounds++;
        return move;
    }
    auto start = high_resolution_clock::now();
    unsigned int nodesSearched = 1;
    int eval;
    string move = multiThreadedSearch(board, &nodesSearched, &eval);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    if(printResults)
    {
        cout << "Minmax finished in " << duration.count() / 1000 << " seconds" << endl;
        cout << "eval: " << eval << " move: " << move << endl;
        cout << "Nodes searched: " << nodesSearched << endl;
    }
    Rounds++;
    return move;
}

std::string ChessAI::multiThreadedSearch(thc::ChessRules board, unsigned int* nodesSearched,
                                         int* bestEval)
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

    cb::ThreadPool pool(cb::ThreadPool::GetNumLogicalCores()); // generates thread pool
    if(isWhite)
    {
        result = std::make_shared<ResultFinder>(
            std::numeric_limits<int>::min()); // min initialisze for maximizer
        for(thc::Move& move : legalMoves)
        {
            pool.Schedule([&]() {
                thc::ChessRules b = board;
                b.PlayMove(move); // push every possible move
                int moveEval = minMax(b, SEARCH_DEPTH - 1, !isWhite, result->getBestEval(),
                                      std::numeric_limits<int>::max(), nodesSearched); // maybe mutex here
                {
                    std::lock_guard<std::mutex> lock{result->getMutex()};
                    {
                        if(moveEval > result->getBestEval()) // check if new best
                        {
                            result->setBestEval(moveEval, move);
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
        for(thc::Move& move : legalMoves)
        {
            pool.Schedule([&]() {
                thc::ChessRules b = board;
                b.PlayMove(move);
                int moveEval =
                    minMax(b, SEARCH_DEPTH - 1, !isWhite, std::numeric_limits<int>::min(),
                           result->getBestEval(), nodesSearched); // maybe mutex here
                {
                    std::lock_guard<std::mutex> lock{result->getMutex()};
                    {
                        if(moveEval < result->getBestEval()) // check if new best
                        {
                            result->setBestEval(moveEval, move);
                        }
                        result->moveIncrement();
                    }
                } // Release mutex
            });
        }
    }

    pool.Wait(); // wait for all threds to finish
    // ugly solution that makes sure threads actually are finished; pool.Wait() doesnt work for some reason
    while(result->getMoveCount() != expectedMoveCount){
        this_thread::sleep_for(200ms);
    }
    *bestEval = result->getBestEval();
    return result->getBestMove().TerseOut();
}

int ChessAI::minMax(thc::ChessRules& board, const int depth, bool maximize, int alpha, int beta,
                    unsigned int* nodesSearched)
{
    (*nodesSearched)++;
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
    MoveSorter::sortMoves(legalMoves, board);
    for(const thc::Move& move : legalMoves)
    {
        thc::ChessRules b = board;
        b.PlayMove(move);
        int val;
        val = minMax(b, depth - 1, !maximize, alpha, beta, nodesSearched);

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
