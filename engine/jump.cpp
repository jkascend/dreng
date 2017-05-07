#include "jump.h"

Jump::Jump(short from, short to)
{
    Jump::_from = from;
    Jump::_to = to;
}

short Jump::computeLanding()
{
    bool isJumpingUp = Jump::_from < Jump::_to;
    short mod8lessThan4 = Jump::_to % 8 < 4;
    
    if (Jump::_to < 4
        || Jump::_to > 27
        || Jump::_to % 8 == 0
        || (Jump::_to - 7) % 8 == 0)
        {
            return -1;
        }
    else if (isJumpingUp && mod8lessThan4)
    {
        return Jump::_to + (Jump::_to - Jump::_from - 1);
    }
    else if (isJumpingUp && !mod8lessThan4)
    {
        return Jump::_to + (Jump::_to - Jump::_from + 1);
    }
    else if (!isJumpingUp && mod8lessThan4)
    {
        return Jump::_to - (Jump::_from - Jump::_to + 1);
    }
    else if (!isJumpingUp && !mod8lessThan4)
    {
        return Jump::_to - (Jump::_from - Jump::_to - 1);
    }
    return -1;
}