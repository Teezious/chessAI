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
        cout << move << endl;
        board.PlayMove(mv);
        thc::TERMINAL evalPosition; // what does this code do?
        bool legal = board.Evaluate(evalPosition);
        bool mateWhite = (evalPosition == thc::TERMINAL_WCHECKMATE);
        bool mateBlack = (evalPosition == thc::TERMINAL_BCHECKMATE);

        // TODO check three fold repetition
        // TODO draws
        // check mate, stalemate
        if(mateBlack || mateWhite)
            break;

        whitesMove = !whitesMove;
    }
    string winner = whitesMove ? "White" : "Black";
    cout << winner << " won" << endl;
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
