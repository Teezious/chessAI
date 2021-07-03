#include "../thc/src/thc.h"

#ifndef CHESSAI_ABSTRACTCHESSPLAYER_H
#define CHESSAI_ABSTRACTCHESSPLAYER_H

#include <string>

class AbstractChessPlayer
{
  public:
    AbstractChessPlayer(bool isWhite)
    {
        this->isWhite = isWhite;
    }
    AbstractChessPlayer()
    {
        this->isWhite = true;
    }

    bool getIsWhite()
    {
        return isWhite;
    }
    void setIsWhite(bool isWhite)
    {
        this->isWhite = isWhite;
    }

    virtual std::string chooseMove(thc::ChessRules board, bool printResults = true) = 0;

  private:
    bool isWhite;
};

#endif // CHESSAI_ABSTRACTCHESSPLAYER_H
