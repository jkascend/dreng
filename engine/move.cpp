#include "move.h"
#include "jump.h"

Move::Move(short from, short to, char side, bool isJump)
{
    Move::_side = side;
    Move::_from = from;
    Move::_to = to;
    Move::_isJump = isJump;
    if (Move::_isJump)
    {
        Move::computeLand(from, to);
    }
}

Move::~Move()
{
    for (int i = 0; i < Move::_children.size(); i++)
    {
        delete Move::_children[i];
    }
}

void Move::computeLand(short from, short to)
{
    Jump::Jump j(from, to);
    Move::_land = j.computeLanding();
}

bool Move::hasChildren()
{
    return Move::_children.size() > 0;
}

char Move::getSide()
{
    return Move::_side;
}

short Move::getFrom()
{
    return Move::_from;
}

short Move::getTo()
{
    return Move::_to;
}

void Move::addChild(Move* m)
{
    _children.push_back(m);
}

std::vector<Move*> Move::getChildren()
{
    return Move::_children;
}

short Move::getLand()
{
    return Move::_land;
}
