//
// Created by Florian on 31.03.2021.
//

#include "Evaluation.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
using namespace std;

int Evaluation::evaluateBoardState(thc::ChessRules board)
{
    int pos = 0;
    int wmat = 0;
    int bmat = 0;
    int bpawn = 0;
    int wpawn = 0;
    int weval = 0;
    int beval = 0;

    // int wkingval = 0;
    // int bkingval = 0;

    int wKingRank = 0;
    int wKingFile = 0;
    int wKingPosEval = 0;
    int bKingRank = 0;
    int bKingFile = 0;
    int bKingPosEval = 0;

    int rank = 7;
    int file = 0;

    for(char piece : board.squares)
    {
        if(piece == 32 || piece == '\0')
        { // skip empty squares
            pos++;
            file++;
            if(file == 8)
            {
                file = 0;
                --rank;
            }
            continue;
        }
        if(islower(piece))
        {
            if(piece == 'k')
            {
                // bkingval += whitePieceValues.at('K');
                bKingRank = rank;
                bKingFile = file;
                bKingPosEval = positionalWhiteValues.at('K').at(pos);
            }
            else if(piece == 'p')
            {
                bpawn += whitePieceValues.at('P');
                beval += positionalWhiteValues.at(toupper(piece)).at(pos);
            }
            else
            {
                bmat += whitePieceValues.at(toupper(piece));
                beval += positionalWhiteValues.at(toupper(piece)).at(pos);
            }
        }
        else
        {
            if(piece == 'K')
            {
                // wkingval += whitePieceValues.at(piece);
                wKingRank = rank;
                wKingFile = file;
                wKingPosEval = positionalWhiteValues.at('K').at(mirrorSquare(pos));
            }
            else if(piece == 'P')
            {
                wpawn += whitePieceValues.at('P');
                weval += positionalWhiteValues.at(piece).at(
                    mirrorSquare(pos)); // mirror white as board is traversed in reverse order
            }
            else
            {
                wmat += whitePieceValues.at(piece);
                weval += positionalWhiteValues.at(piece).at(
                    mirrorSquare(pos)); // mirror white as board is traversed in reverse order
            }
        }
        pos++;
        file++;
        if(file == 8)
        {
            file = 0;
            --rank;
        }
    }
    float wEndGame = endgameWeight(wmat);
    float bEndGame = endgameWeight(bmat);

    weval += (int)wKingPosEval * (1 - wEndGame);
    beval += (int)bKingPosEval * (1 - bEndGame);

    weval += endgameEval(wKingRank, wKingFile, wmat + wpawn, bKingRank, bKingFile, bmat + bpawn,
                         bEndGame);
    beval += endgameEval(bKingRank, bKingFile, bmat + bpawn, wKingRank, wKingFile, wmat + wpawn,
                         wEndGame);

    return (weval + wmat + wpawn) - (beval + bmat + bpawn);
}

int Evaluation::endgameEval(int mrank, int mfile, int mmaterial, int ematerial, int erank,
                            int efile, float endgameWeight)
{
    if(mmaterial > ematerial + 2 * 100 && endgameWeight > 0)
    {
        int eval = 0;
        int eDstCtrFile = max(3 - efile, efile - 4);
        int eDstCtrRank = max(3 - erank, erank - 4);
        int ctrDst = eDstCtrRank + eDstCtrFile;

        eval += (10 * ctrDst);

        int fileDst = abs(mfile - efile);
        int rankDst = abs(mrank - erank);
        int kingDst = fileDst + rankDst; // manhattan distance

        eval += (14 - kingDst) * 4;

        return (int)(eval * 10 * endgameWeight);
    }
    return 0;
}

int Evaluation::mirrorSquare(int pos)
{
    return pos ^ 0x38;
}
int Evaluation::getWhitePieceValue(char piece)
{
    return whitePieceValues.at(toupper(piece));
}

float Evaluation::endgameWeight(int materialCountWithoutPawns)

{
    float endgameMaterialStart =
        whitePieceValues.at('R') * 2 + whitePieceValues.at('B') + whitePieceValues.at('N');
    float multiplier = 1 / endgameMaterialStart;
    float weight = 1 - std::min((float)1, materialCountWithoutPawns * multiplier);
    return weight;
}
const std::map<char, int> Evaluation::whitePieceValues = {{'P', 100}, {'N', 310}, {'B', 330},
                                                          {'R', 500}, {'Q', 900}, {'K', 20000}};
// positional values for each piece  rows = rank 1 -> 8, columns = file a -> h
const std::map<char, std::vector<int>> Evaluation::positionalWhiteValues = {
    {'P',
     {0,  0,  0,  0,  0,  0,  0,  0,  5,  10, 10, -20, -20, 10, 10, 5,  5, -5, -10, 0,  0,  -10,
      -5, 5,  0,  0,  0,  20, 20, 0,  0,  0,  5,  5,   10,  25, 25, 10, 5, 5,  10,  10, 20, 30,
      30, 20, 10, 10, 50, 50, 50, 50, 50, 50, 50, 50,  0,   0,  0,  0,  0, 0,  0,   0}},
    {'N', {-50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0,   0,   0,   0,   -20, -40,
           -30, 0,   10,  15,  15,  10,  0,   -30, -30, 5,   15,  20,  20,  15,  5,   -30,
           -30, 0,   15,  20,  20,  15,  0,   -30, -30, 5,   10,  15,  15,  10,  5,   -30,
           -40, -20, 0,   5,   5,   0,   -20, -40, -50, -40, -30, -30, -30, -30, -40, -50}},
    {'B', {-20, -10, -10, -10, -10, -10, -10, -20, -10, 5,   0,   0,   0,   0,   5,   -10,
           -10, 10,  10,  10,  10,  10,  10,  -10, -10, 0,   10,  10,  10,  10,  0,   -10,
           -10, 5,   5,   10,  10,  5,   5,   -10, -10, 0,   5,   10,  10,  5,   0,   -10,
           -10, 0,   0,   0,   0,   0,   0,   -10, -20, -10, -10, -10, -10, -10, -10, -20}},
    {'R', {0, 0,  0,  5,  5, 0,  0,  0,  -5, 0,  0,  0, 0, 0, 0, -5, -5, 0,  0,  0, 0, 0,
           0, -5, -5, 0,  0, 0,  0,  0,  0,  -5, -5, 0, 0, 0, 0, 0,  0,  -5, -5, 0, 0, 0,
           0, 0,  0,  -5, 5, 10, 10, 10, 10, 10, 10, 5, 0, 0, 0, 0,  0,  0,  0,  0}},
    {'Q', {-20, -10, -10, -5, -5, -10, -10, -20, -10, 0,   5,   0,  0,  0,   0,   -10,
           -10, 5,   5,   5,  5,  5,   0,   -10, 0,   0,   5,   5,  5,  5,   0,   -5,
           -5,  0,   5,   5,  5,  5,   0,   -5,  -10, 0,   5,   5,  5,  5,   0,   -10,
           -10, 0,   0,   0,  0,  0,   0,   -10, -20, -10, -10, -5, -5, -10, -10, -20}},
    {'K', {20,  30,  10,  0,   0,   10,  30,  20,  20,  20,  0,   0,   0,   0,   20,  20,
           -10, -20, -20, -20, -20, -20, -20, -10, -20, -30, -30, -40, -40, -30, -30, -20,
           -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30,
           -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30}}};
