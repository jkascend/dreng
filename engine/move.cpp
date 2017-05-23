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

// private
void Move::computeLand(short from, short to)
{
    Jump::Jump j(from, to);
    Move::_land = j.computeLanding();
}

// public
bool Move::hasChildren()
{
    return Move::_children.size() > 0;
}

bool Move::isJump()
{
    return Move::_isJump;
}

bool Move::operator == (const Move &other) const {
    if (Move::_isJump && Move::_land != other._land)
    {
        return false;
    }
    return Move::_to == other._to
        && Move::_from == other._from
        && Move::_isJump == other._isJump
        && Move::_side == other._side;
}

bool Move::operator != (const Move &other) const {
    return !(*this == other);
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

short Move::getLand()
{
    return Move::_land;
}

std::vector<std::shared_ptr<Move::Move> > Move::getChildren()
{
    return std::move(Move::_children);
}

std::shared_ptr<Move::Move> Move::getChildMove()
{
    return Move::_chosenChild;
}

void Move::addChild(std::shared_ptr<Move::Move> m)
{
    Move::_children.push_back(std::move(m));
}

void Move::setChildMove(std::shared_ptr<Move::Move> m)
{
    Move::_chosenChild = m;
}
