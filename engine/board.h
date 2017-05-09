#ifndef BOARD_H
#define BOARD_H

class Board
{
    private:
        char _curMove;
        unsigned long _white;
        unsigned long _black;
        
        bool kingPositionIsValid(unsigned long l, int pos);
        bool squareIndexOutOfRange(short square);
        bool squareIsOwnedByColor(short square, char color);
    public:
        Board();

        bool blackIsSquareOwner(short square);
        bool opponentIsSquareOwner(short square);
        bool squareIsFree(short square);
        bool whiteIsSquareOwner(short square);
        char getCurMove();
        unsigned long getBlack();
        unsigned long getFreePos();
        unsigned long getWhite();
        void makeBlackKing(int pos);
        void makeWhiteKing(int pos);
        void setCurMove(char c);
        void setBlack(unsigned long b);
        void setWhite(unsigned long w);
};

#endif
