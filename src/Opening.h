//
// Created by Florian on 23.06.2021.
//

#ifndef CHESSAI_OPENING_H
#define CHESSAI_OPENING_H

#include <ChessRules.h>
#include <string>

class Opening
{
  public:
    static std::string getOpeningMove(thc::ChessRules& board);

};

#endif // CHESSAI_OPENING_H
