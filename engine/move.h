#ifndef MOVE_H
#define MOVE_H

#include <vector>

class Move
{
    protected:
        char _side;
        std::vector<Move*> _children;
        bool _isJump;
        short _from, _to, _land;
        void computeLand(short from, short to);
    public:
        Move(short from, short to, char side, bool isJump);
        ~Move();
        bool hasChildren();
        void addChild(Move *j);
        char getSide();
        std::vector<Move*> getChildren();
        short getFrom();
        short getTo();
        short getLand();
};

#endif
