#ifndef BOARD_H
#define BOARD_H

#include "move.h"

class Board
{
    private:
        char _curMove;
        unsigned long _white;
        unsigned long _black;
        
        bool kingPositionIsValid(unsigned long l, int pos);
        bool squareIndexOutOfRange(short square);
        bool squareIsOwnedByColor(short square, char color);
        bool onlyManOrKingOccupiesSquare(short index, unsigned long *board);
        bool squareIsClear(short square);
        void moveTowardBit(short from, short to, unsigned long *board);
        void turnOffManAndKingBits(short target, unsigned long *board);
        void makeKingIfValid(std::shared_ptr<Move> move);

    public:
        Board();

        bool blackIsSquareOwner(short square);
        bool opponentIsSquareOwner(short square);
        bool squareIsFree(short square);
        bool whiteIsSquareOwner(short square);
        bool boardStateIsValid();
        char getCurMove();
        unsigned long getBlack();
        unsigned long getFreePos();
        unsigned long getWhite();
        void makeBlackKing(short pos);
        void makeWhiteKing(short pos);
        void setCurMove(char c);
        void setBlack(unsigned long b);
        void setWhite(unsigned long w);
        void performMove(std::shared_ptr<Move> move);
};

#endif
