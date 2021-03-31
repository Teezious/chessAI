//
// Created by Florian on 30.03.2021.
//
#include <algorithm>
#include "ChessGame.h"
#include "AbstractChessPlayer.h"
#include "ChessAI.h"
#include "HumanChessPlayer.h"

using namespace std;

void ChessGame::gameHandler() {
    AbstractChessPlayer *whitePlayer;
    AbstractChessPlayer *blackPlayer;
    while(1) {
        string userColorChoice;
        cout << "Enter your color(white/black): ";
        getline(cin, userColorChoice);
        transform(userColorChoice.begin(), userColorChoice.end(), userColorChoice.begin(), ::tolower);
        if(userColorChoice.compare("black") == 0) {
            ChessAI ai(true);
            HumanChessPlayer humanPlayer(false);
            whitePlayer = &ai;
            blackPlayer = &humanPlayer;
            break;
        }
        else if(userColorChoice.compare("white") == 0) {
            ChessAI ai(false);
            HumanChessPlayer humanPlayer(true);
            whitePlayer = &humanPlayer;
            blackPlayer = &ai;
            break;
        }
        else
            cout << "Invalid Input! Try again" << endl;
    }

    bool whitesMove = true;
    string move;
    thc::Move mv;

    while(1) {
        printBoardState();

        if(whitesMove) {
            move = whitePlayer->chooseMove(&board);
        }
        else {
            move = blackPlayer->chooseMove(&board);
        }
        cout << move << endl;
        //TODO check legal move
        mv.TerseIn(&board, move.c_str());
        board.PlayMove(mv);
        thc::TERMINAL evalPosition;
        bool legal = board.Evaluate(evalPosition);
        bool mateWhite = (evalPosition == thc::TERMINAL_WCHECKMATE);
        bool mateBlack = (evalPosition == thc::TERMINAL_BCHECKMATE);


        //check mate, stalemate
        if(mateBlack || mateWhite)
            break;

        whitesMove = !whitesMove;
    }
    string winner = whitesMove ? "White" : "Black";
    cout << winner << " won" << endl;
}

//temporary function
void ChessGame::printBoardState() {
    string boardString = this->board.ToDebugStr();
    boardString.erase(0, 15);
    int i = 0;
    for(char c : boardString) {
        if(i % 9 == 0)
            cout << "  +---+---+---+---+---+---+---+---+" << endl << " " << ((72 - i)  / 9) << " ";
        cout << "| " << c << " ";
        i++;
    }
    cout << "  +---+---+---+---+---+---+---+---+" << endl;
    cout << "     a   b   c   d   e   f   g   h " << endl;
}