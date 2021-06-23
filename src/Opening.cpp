//
// Created by Florian on 23.06.2021.
//

using namespace std;

#include "Opening.h"
#include <fstream>
#include <sstream>

thc::Move Opening::getOpeningMove(thc::ChessRules& board, int moveNumber)
{
    fstream inFile;
    string line;
    inFile.open("../resources/GrandMasterGames/GMGames.txt");
    thc::Move bestOpeningMove;
    //if its the first move to be played, choose one
    if(moveNumber == 0) {
        getline(inFile, line);
        thc::Move mv;
        mv.NaturalIn(&board, )
    }


    //else find a game where the same opening moves are played as in the current one
    while(true) {
        bool sameMoves = true;
        string openingMovesNatural[moveNumber + 1];
        string openingMovesTerse[moveNumber + 1];
        getline(inFile, line);

        //split the string
        stringstream ssin(line);
        int i = 0;
        while (ssin.good() && i < moveNumber + 1){
            ssin >> openingMovesNatural[i];
            ++i;
        }
        //convert from Natural to Terse
        thc::ChessRules tmpBoard;
        for(int j = 0; j < moveNumber + 1; j++) {
            thc::Move mv;
            mv.NaturalIn(&tmpBoard, openingMovesNatural[j].c_str());
            tmpBoard.PlayMove(mv);
            openingMovesTerse[j] = mv.TerseOut();
        }

        //compare history with the current game
        for(int j = 0; j < moveNumber; j++) {
            if(openingMovesTerse[j] != board.history[j+1].TerseOut()) {
                sameMoves = false;
                break;
            }
        }
        if(sameMoves) {
            thc::Move mv;
            mv.TerseIn(&board, openingMovesTerse[moveNumber].c_str());
            return mv;
        }




    }
    inFile.close();
}
