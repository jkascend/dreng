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

bool Board::boardStateIsValid()
{
    return !((Board::_black & Board::_black >> 32)
        | (Board::_white & Board::_white >> 32));
}

bool Board::squareIsClear(short square)
{
    return (Board::_black | Board::_black >> 32 | Board::_white | Board::_white >> 32)
        & (1 << square);
}

bool Board::opponentIsSquareOwner(short square)
{
    return Board::_curMove == 'b' ? Board::whiteIsSquareOwner(square) : Board::blackIsSquareOwner(square);
}

// TODO: this function needs to be renamed, Board::squareIsClear is better
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

void Board::moveTowardBit(short from, short to, unsigned long *board)
{
    // move piece
    if (*board & 1 << from)
    {
        // is man
        *board |= (1 << to);
    }
    else
    {
        // is king
        *board |= ((unsigned long)1 << (to + 32));
    }
}

void Board::turnOffManAndKingBits(short target, unsigned long *board)
{
    *board &= ~(1 << target);
    *board &= ~((unsigned long)1 << (target + 32));
}

bool Board::onlyManOrKingOccupiesSquare(short index, unsigned long *board)
{
    unsigned long man = *board & 1 << index;
    unsigned long king = *board & (unsigned long)1 << (index + 32);

    return !(man ^ (king >> 32));
}

void Board::performMove(std::shared_ptr<Move> move)
{
    if (!Board::boardStateIsValid())
    {
        throw 28;
    }

    // make sure move corresponds to current turn
    if (Board::_curMove != move->getSide())
    {
        throw 27;
    }
    unsigned long *curSide = move->getSide() == 'b' ? &(Board::_black) : &(Board::_white);
    unsigned long *opponent = move->getSide() == 'b' ? &(Board::_white) : &(Board::_black);

    // check only a man or king occupies from pos
    // fail for pos 1: 010
    //                 110
    // pass for pos 1: 001
    //                 010
    if (Board::onlyManOrKingOccupiesSquare(move->getFrom(), curSide))
    {
        throw 25;
    }

    // if regular move, check man bit and king bit are xor, flip whichever is set to
    // 0, check dest man/king bits are 0 and turn on whichever should be set
    if (move->isJump())
    {
        // check destination is occupied by enemy
        unsigned long fromMan = *opponent & (unsigned long)1 << move->getTo();
        unsigned long fromKing = *opponent & (unsigned long)1 << (move->getTo() + 32);
        if (!(fromMan ^ (fromKing >> 32)))
        {
            throw 29;
        }

        // check landing pos is clear
        if (Board::squareIsClear(move->getLand()))
        {
            throw 30;
        }

        // move to appropriate pos
        Board::moveTowardBit(move->getFrom(), move->getLand(), curSide);

        // turn off jumped bit
        Board::turnOffManAndKingBits(move->getLand(), opponent);

        // turn off "from"" bit
        Board::turnOffManAndKingBits(move->getFrom(), curSide);

        if (move->getChildren().size() > 1)
        {
            throw 31;
        }
    }
    else
    {
        // check destination is unoccupied
        // fail for pos 1: 010
        //                 001
        // pass for pos 1: 001
        //                 100
        if ((*curSide & ((unsigned long)1 << move->getTo())) +
            (*curSide & ((unsigned long)1 << (move->getTo() + 32))))
            {
                throw 26;
            }
        
        // move to appropriate pos
        Board::moveTowardBit(move->getFrom(), move->getTo(), curSide);

        // clear from bits, should be 0 no matter what
        Board::turnOffManAndKingBits(move->getFrom(), curSide);
    }

    Board::_curMove = move->getSide() == 'b' ? 'w' : 'b';
}
