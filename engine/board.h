#ifndef BOARD_H
#define BOARD_H

class Board
{
    private:
        unsigned long _white;
        unsigned long _black;
        char _curMove;
        bool kingPositionIsValid(unsigned long l, int pos);
        bool squareIndexOutOfRange(short square);
        bool squareIsOwnedByColor(short square, char color);
    public:
        Board();
        void setWhite(unsigned long w);
        void setBlack(unsigned long b);
        void setCurMove(char c);
        unsigned long getWhite();
        unsigned long getBlack();
        char getCurMove();
        unsigned long getFreePos();
        void makeWhiteKing(int pos);
        void makeBlackKing(int pos);
        bool squareIsFree(short square);
        bool blackIsSquareOwner(short square);
        bool whiteIsSquareOwner(short square);
        bool opponentIsSquareOwner(short square);
};

#endif
