//
// Created by Florian on 30.03.2021.
//
#include "ChessGame.h"
#include "AbstractChessPlayer.h"
#include "ChessAI.h"
#include "HumanChessPlayer.h"

#include <algorithm>
#include <memory>

using namespace std;
using ChessPlayerPtr = std::shared_ptr<AbstractChessPlayer>;

void ChessGame::gameHandler()
{
    ChessPlayerPtr whitePlayer = nullptr;
    ChessPlayerPtr blackPlayer = nullptr;
    while(true)
    {
        string userColorChoice = "white";
        cout << "Enter your color(white/black): ";
        getline(cin, userColorChoice);
        transform(userColorChoice.begin(), userColorChoice.end(), userColorChoice.begin(),
                  ::tolower);
        if(userColorChoice.compare("black") == 0)
        {
            whitePlayer = ChessPlayerPtr(new ChessAI(true));
            blackPlayer = ChessPlayerPtr(new HumanChessPlayer(false));
            break;
        }
        else if(userColorChoice.compare("white") == 0)
        {
            blackPlayer = ChessPlayerPtr(new ChessAI(false));
            whitePlayer = ChessPlayerPtr(new HumanChessPlayer(true));
            break;
        }
        else
            cout << "Invalid Input! Try again" << endl;
    }

    bool whitesMove = true;
    string move;
    thc::Move mv;

    string endgame1 = "4k3/8/8/8/8/8/5R2/R3K3 w - - 0 1";
    string mateIn3 = "3r3k/4r3/8/8/8/8/8/2K5 w - - 0 1";
    string mateIn1 = "1r5k/2r5/8/8/8/8/8/K7 w - - 0 1";
    string endgame = "4k3/8/8/8/8/8/8/R3K3 w - - 0 1";
    board.Forsyth(endgame.c_str());
    while(1)
    {
        printBoardState();
        if(whitesMove)
        {
            do
            {
                move = whitePlayer->chooseMove(board);
            } while(!mv.TerseIn(&board, move.c_str()));
        }
        else
        {
            do
            {
                move = blackPlayer->chooseMove(board);
            } while(!mv.TerseIn(&board, move.c_str()));
        }
        board.PlayMove(mv);

        thc::TERMINAL evalPosition;
        board.Evaluate(evalPosition);
        thc::DRAWTYPE eval_draw;
        board.IsDraw(whitesMove, eval_draw);
        if(eval_draw == thc::DRAWTYPE_50MOVE || eval_draw == thc::DRAWTYPE_REPITITION ||
           eval_draw == thc::DRAWTYPE_INSUFFICIENT_AUTO)
        {
            printBoardState();
            cout << "Final Move: " << move << endl;
            cout << " Draw" << endl;
            break;
        }
        else if(evalPosition == 2 || evalPosition == -2)
        {
            printBoardState();
            cout << "Final Move: " << move << endl;
            cout << "Draw" << endl;
            break;
        }
        else if(evalPosition == 1)
        {
            printBoardState();
            cout << "Final Move: " << move << endl;
            cout << "White won!" << endl;
            break;
        }
        else if(evalPosition == -1)
        {
            printBoardState();
            cout << "Final Move: " << move << endl;
            cout << "Black won!" << endl;
            break;
        }

        whitesMove = !whitesMove;
    }
}

// temporary function
void ChessGame::printBoardState()
{
    int i = 0;
    for(char c : board.squares)
    {
        if(i % 8 == 0 && i != 64)
            cout << "|" << endl
                 << "   +---+---+---+---+---+---+---+---+" << endl
                 << " " << ((64 - i) / 8) << " ";
        cout << "| " << c << " ";
        i++;
    }
    cout << endl << "   +---+---+---+---+---+---+---+---+" << endl;
    cout << "     a   b   c   d   e   f   g   h " << endl;
}
