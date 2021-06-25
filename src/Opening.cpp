//
// Created by Florian on 23.06.2021.
//

using namespace std;

#include "Opening.h"
#include <fstream>
#include <sstream>
#include <iostream>

string Opening::getOpeningMove(thc::ChessRules& board)
{
    fstream inFile;
    string line;
    inFile.open("../resources/GrandMasterGames/GMGames.txt");
    string bestOpeningMove;
    int moveNumber = board.history_idx;


    //else find a game where the same opening moves are played as in the current one
    while(getline(inFile, line)) {
        bool sameMoves = true;
        string openingMovesNatural[moveNumber + 1];
        string openingMovesTerse[moveNumber + 1];

        //split the string
        stringstream ssin(line);
        int i = 0;
        while (ssin.good() && i < moveNumber + 1){
            ssin >> openingMovesNatural[i];
            ++i;
        }
        thc::ChessRules tmpBoard;
        //if its the first move to be played, choose one
        if(moveNumber == 1) {
            thc::Move mv;
            mv.NaturalIn(&tmpBoard, openingMovesNatural[0].c_str());
            inFile.close();
            return mv.TerseOut();
        }

        //convert from Natural to Terse

        for(int j = 0; j < moveNumber + 1; j++) {
            thc::Move mv;
            mv.NaturalIn(&tmpBoard, openingMovesNatural[j].c_str());
            openingMovesTerse[j] = mv.TerseOut();
            tmpBoard.PlayMove(mv);
        }

        //compare history with the current game
        for(int j = 0; j < moveNumber - 1; j++) {
            if(openingMovesTerse[j] != board.history[j+1].TerseOut()) {
                sameMoves = false;
                break;
            }
        }
        if(sameMoves)
        {
            inFile.close();
            return openingMovesTerse[moveNumber - 1];
        }
    }
    //if no opening found return null and run a search
    return "";
}
