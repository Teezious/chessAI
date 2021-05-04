//
// Created by Florian on 31.03.2021.
//

#ifndef CHESSAI_EVALUATION_H
#define CHESSAI_EVALUATION_H

#include "../lib/thc/src/thc.h"
#include <map>

class Evaluation final
{
  public:
    static int evaluateBoardState(thc::ChessRules board);
    static int getWhitePieceValue(char piece);
  private:
    static int mirrorSquare(int pos);
    static const std::map<char, int> whitePieceValues;
    static const std::map<char, std::vector<int>> positionalWhiteValues;
};

#endif // CHESSAI_EVALUATION_H
