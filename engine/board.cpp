#include "board.h"

Board::Board()
{
    Board::_white = 4095;
    Board::_black = 4293918720;
    Board::_curMove = 'b';
}

// private:
bool Board::kingPositionIsValid(unsigned long l, int pos)
{
    // TODO: Make error codes standardized
    if (pos > 31)
    {
        throw 20;
    }
    // check current pos is not empty
    if ((l & (unsigned long)(1 << pos)) == 0)
    {
        throw 21;
    }
    // check king pos is empty
    if ((l & ((unsigned long)1 << (pos + 32))) != 0)
    {
        throw 22;
    }
    return true;
}

bool Board::squareIndexOutOfRange(short square)
{
    return square < 0 || square > 31;
}

bool Board::squareIsOwnedByColor(short square, char color)
{
    if (Board::squareIndexOutOfRange(square))
    {
        return false;
    }
    unsigned long target;
    if (color == 'b')
    {
        target = Board::_black;
    }
    else if (color == 'w')
    {
        target = Board::_white;
    }

    return ((target & (unsigned long)1 << square) != 0) || ((target >> 32 & (unsigned long)1 << square) != 0);
}

// public:
bool Board::blackIsSquareOwner(short square)
{
    return Board::squareIsOwnedByColor(square, 'b');
}

bool Board::opponentIsSquareOwner(short square)
{
    return Board::_curMove == 'b' ? Board::whiteIsSquareOwner(square) : Board::blackIsSquareOwner(square);
}

bool Board::squareIsFree(short square)
{
    if (Board::squareIndexOutOfRange(square))
    {
        return false;
    }
    return (Board::getFreePos() & (1 << square)) == 0;
}

bool Board::whiteIsSquareOwner(short square)
{
    return Board::squareIsOwnedByColor(square, 'w');
}

char Board::getCurMove()
{
    return Board::_curMove;
}

unsigned long Board::getBlack()
{
    return Board::_black;
}

unsigned long Board::getFreePos()
{
    return (Board::_white & 4294967295)
        | (Board::_black & 4294967295)
        | (Board::_white >> 32)
        | (Board::_black >> 32);
}

unsigned long Board::getWhite()
{
    return Board::_white;
}

void Board::makeBlackKing(int pos)
{
    if (Board::kingPositionIsValid(Board::_black, pos))
    {
        Board::_black ^= (unsigned long)1 << pos;
        Board::_black ^= (unsigned long)1 << (pos + 32);
    }
}

void Board::makeWhiteKing(int pos)
{
    if (Board::kingPositionIsValid(Board::_white, pos))
    {
        Board::_white ^= (unsigned long)1 << pos;
        Board::_white ^= (unsigned long)1 << (pos + 32);
    }
}

void Board::setCurMove(char c)
{
    Board::_curMove = c;
}

void Board::setBlack(unsigned long b)
{
    Board::_black = b;
}

void Board::setWhite(unsigned long w)
{
    Board::_white = w;
}
