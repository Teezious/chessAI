#include "Evaluation.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
using namespace std;

int Evaluation::evaluateBoardState(thc::ChessRules board)
{
    int pos = 0;
    int wmat = 0; // white material
    int bmat = 0; // black material
    int bpawn = 0; // black pawn material
    int wpawn = 0; // white pawn material
    int wkingval = 0;
    int bkingval = 0;
    int weval = 0; // white eval
    int beval = 0; // black eval

    int wKingRank = 0; // white King Rank
    int wKingFile = 0; // white King File
    int wKingPos = 0; // White King Pos
    int bKingRank = 0; // black  King Rank
    int bKingFile = 0; // black  King File
    int bKingPos = 0; // black  King Pos

    int rank = 7; // rank starts at 7 as we iterate from top to bottom
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
                bkingval += whitePieceValues.at('K');
                bKingRank = rank;
                bKingFile = file;
                bKingPos = pos;
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
                wkingval += whitePieceValues.at(piece);
                wKingRank = rank;
                wKingFile = file;
                wKingPos = pos;
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
    float wEndGame = endgameWeight(wmat); // calculate endgame weight
    float bEndGame = endgameWeight(bmat);

    // when in endgame use different table for King
    if(wEndGame > 0.3)
    {
        weval += positionalWhiteValues.at('L').at(mirrorSquare(wKingPos));
    }
    else
    {
        beval += positionalWhiteValues.at('K').at(mirrorSquare(wKingPos));
    }
    if(bEndGame > 0.3)
    {
        beval += positionalWhiteValues.at('L').at(bKingPos);
    }
    else
    {
        beval += positionalWhiteValues.at('K').at(bKingPos);
    }

    weval += endgameEval(wKingRank, wKingFile, wmat + wpawn, bKingRank, bKingFile, bmat + bpawn,
                         bEndGame);
    beval += endgameEval(bKingRank, bKingFile, bmat + bpawn, wKingRank, wKingFile, wmat + wpawn,
                         wEndGame);

    return (weval + wmat + wpawn) - (beval + bmat + bpawn);
}

// Calculates Special Evaluation for endgame
int Evaluation::endgameEval(int mrank, int mfile, int mmaterial, int ematerial, int erank,
                            int efile, float endgameWeight)
{
    if(mmaterial > ematerial + 2 * 100 && endgameWeight > 0)
    {
        int eval = 0;
        int eDstCtrFile = max(3 - efile, efile - 4);
        int eDstCtrRank = max(3 - erank, erank - 4);
        int eCtrDst = eDstCtrRank + eDstCtrFile;

        eval += (10 *
                 eCtrDst); // reward when enemy king is in corner (bigger center distance -> corner)

        int fileDst = abs(mfile - efile);
        int rankDst = abs(mrank - erank);
        int kingDst = fileDst + rankDst; // orthogonal distance

        eval -= kingDst * 4; // reward getting the king closer to enemy king

        return (int)(eval * endgameWeight);
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

// calculated endgame weight
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
           -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30}},
    {'L', {-50, -40, -30, -20, -20, -30, -40, -50, -30, -20, -10, 0,   0,   -10, -20, -30,
           -30, -10, 20,  30,  30,  20,  -10, -30, -30, -10, 30,  40,  40,  30,  -10, -30,
           -30, -10, 30,  40,  40,  30,  -10, -30, -30, -10, 20,  30,  30,  20,  -10, -30,
           -30, -30, 0,   0,   0,   0,   -30, -30, -50, -30, -30, -30, -30, -30, -30, -50}}};
