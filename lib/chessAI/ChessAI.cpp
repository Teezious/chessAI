#include "ChessAI.h"
#include "Evaluation.h"
#include "MoveSorter.h"
#include "thread_pool.h"
#include <assert.h>
#include "MoveSorter.h"
#include "Opening.h"
#include <chrono>
#include <iostream>
#include <limits>
#include <memory>
#include <thread>
#include <tuple>

using namespace std;
using namespace std::chrono;
typedef std::shared_ptr<std::thread> ThreadPtr;
int ChessAI::lastEval = 0;
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
    bool OpeningFound = false;
    string move;
    if(board.history_idx <= OPENING_MOVE_LIMIT * 2)
    {
        move = Opening::getOpeningMove(board);
        if(move != "")
        {
            OpeningFound = true;
            cout << "Move: " << move << endl;
        }
    }
    if(!OpeningFound)
    {
        auto start = high_resolution_clock::now();
        static std::atomic<unsigned int> nodesSearched = 0;
        int eval;
        move = multiThreadedSearch(board, nodesSearched, eval);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        if(printResults)
        {
            cout << "Minmax finished in " << duration.count() / 1000.0 << " seconds" << endl;
            cout << "eval: " << eval << " move: " << move << endl;
            cout << "Nodes searched: " << nodesSearched << endl;
        }
    }
    return move;
}

std::string ChessAI::multiThreadedSearch(thc::ChessRules board,
                                         std::atomic<unsigned int>& nodesSearched, int& bestEval)
{
    std::vector<bool> check;
    std::vector<bool> mate;
    std::vector<bool> stalemate;
    std::vector<int> eval;
    std::vector<string> move;
    std::vector<thc::Move> legalMoves;
    bool isWhite = this->getIsWhite();
    board.GenLegalMoveList(legalMoves, check, mate, stalemate);
    std::vector<unsigned int> indices = MoveSorter::sortMoves(legalMoves, board);
    ResultFinder::Ptr result; // result will be saved here
    int expectedMoveCount = legalMoves.size();
    int cores = (int)(cb::ThreadPool::GetNumLogicalCores()) * 2 / 3;
    cb::ThreadPool pool(cores); // generates thread pool
    if(isWhite)
    {
        result = std::make_shared<ResultFinder>(
            std::numeric_limits<int>::min()); // min initialisze for maximizer
        for(const unsigned int& i : indices)
        {
            const thc::Move& move = legalMoves[i];
            pool.Schedule([&]() {
                thc::ChessRules b = board;
                b.PlayMove(move); // push every possible move
                int moveEval =
                    minMax(b, SEARCH_DEPTH - 1, result->getBestEval(),
                           std::numeric_limits<int>::max(), nodesSearched); // maybe mutex here
                {
                    std::lock_guard lock{result->getMutex()};
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
            std::numeric_limits<int>::max()); // max initialize for minimizer
        for(const unsigned int& i : indices)
        {
            const thc::Move& move = legalMoves[i];
            pool.Schedule([&]() {
                thc::ChessRules b = board;
                b.PlayMove(move);
                int moveEval = minMax(b, SEARCH_DEPTH - 1, std::numeric_limits<int>::min(),
                                      result->getBestEval(), nodesSearched); // maybe mutex here
                {
                    std::lock_guard lock{result->getMutex()};
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

    pool.Wait(); // wait for all threads to finish
    while(result->getMoveCount() != expectedMoveCount)
    {
        this_thread::sleep_for(5ms);
    }
    bestEval = result->getBestEval();
    lastEval = abs(result->getBestEval()); // save last eval
    return result->getBestMove().TerseOut();
}

// search reduced search tree by only checking for checks and captures
int ChessAI::quiescentSearch(thc::ChessRules& board, int alpha, int beta,
                             std::atomic<unsigned int>& nodesSearched, int depth)
{
    thc::DRAWTYPE eval_draw;
    board.IsDraw(board.white, eval_draw); // detect draw
    if(eval_draw == thc::DRAWTYPE_50MOVE || eval_draw == thc::DRAWTYPE_REPITITION ||
       eval_draw == thc::DRAWTYPE_INSUFFICIENT_AUTO)
    {
        return 0; // if draw return 0
    }
    else if(board.GetRepetitionCount() > 1 && lastEval > 100)
    {
        return 0; // if leading and prevent repetition
    }

    thc::TERMINAL eval_final_position;
    board.Evaluate(eval_final_position); // detect mate
    if(eval_final_position != 0)
    {
        if(eval_final_position == 2 || eval_final_position == -2) // stalemate
        {
            return 0;
        }
        else if(eval_final_position == 1) // black is checkmated
        {
            return MAXEVAL + depth; // return max value, add depth to reward faster checkmates
        }
        else
        {
            return -MAXEVAL - depth;
        }
    }

    int standPat = Evaluation::evaluateBoardState(board);
    if(standPat >= beta)
    {
        return beta;
    }
    if(depth == 0)
    {
        return standPat;
    }
    nodesSearched++;

    alpha = max(alpha, standPat);

    // gen moves
    std::vector<thc::Move> moves;
    std::vector<bool> check;
    std::vector<bool> mate;
    std::vector<bool> stalemate;
    int i = 0;
    board.GenLegalMoveList(moves, check, mate, stalemate);

    std::vector<unsigned int> indices = MoveSorter::sortMoves(moves, board);
    for(const unsigned int& i : indices)
    {
        const thc::Move& mv = moves[i];
        if(mv.capture == 32 && check[i] == false)
            continue;

        thc::ChessRules b = board;
        b.PlayMove(mv);
        int score = -quiescentSearch(b, -beta, -alpha, nodesSearched, depth - 1);

        if(score >= beta)
            return beta;
        alpha = max(alpha, score);
    }
    return alpha;
}

int ChessAI::minMax(thc::ChessRules& board, const int depth, int alpha, int beta,
                    std::atomic<unsigned int>& nodesSearched)
{
    thc::DRAWTYPE eval_draw;
    board.IsDraw(board.white, eval_draw); // detect draw
    if(eval_draw == thc::DRAWTYPE_50MOVE || eval_draw == thc::DRAWTYPE_REPITITION ||
       eval_draw == thc::DRAWTYPE_INSUFFICIENT_AUTO)
    {
        return 0; // when draw return 0
    }
    else if(board.GetRepetitionCount() > 1 && lastEval > 100)
    {
        return 0; // prevent going for draw when having a lead
    }

    thc::TERMINAL eval_final_position;
    board.Evaluate(eval_final_position); // detect mate
    if(eval_final_position != 0)
    {
        if(eval_final_position == 2 && eval_final_position == -2) // stalemate
        {
            return 0;
        }
        else if(eval_final_position == 1) // black is checkmated
        {
            return MAXEVAL + depth; // return max value, add depth to reward faster checkmates
        }
        else
        {
            return -MAXEVAL - depth;
        }
    }
    if(depth == 0)
    {
        return quiescentSearch(board, alpha, beta, nodesSearched, QUIESCENT_SEARCH_LIMIT);
    }

    nodesSearched++;
    std::vector<thc::Move> legalMoves;
    std::vector<bool> check;
    std::vector<bool> mate;
    std::vector<bool> stalemate;

    int bestEval = board.white ? numeric_limits<int>::min() : numeric_limits<int>::max();

    board.GenLegalMoveList(legalMoves, check, mate, stalemate);
    std::vector<unsigned int> indices = MoveSorter::sortMoves(legalMoves, board);
    for(const unsigned int& i : indices)
    {
        const thc::Move& move = legalMoves[i];
        thc::ChessRules b = board;
        b.PlayMove(move);
        int val;
        val = minMax(b, depth - 1, alpha, beta, nodesSearched);

        if(board.white)
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
