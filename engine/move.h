#ifndef MOVE_H
#define MOVE_H

#include <vector>
#include <memory>

class Move
{
    private:
        bool _isJump;
        char _side;
        short _from, _to, _land;
        std::vector<std::shared_ptr<Move::Move> > _children;
        std::shared_ptr<Move> _chosenChild;
        void computeLand(short from, short to);
    public:
        Move(short from, short to, char side, bool isJump);

        bool hasChildren();
        bool isJump();
        bool operator == (const Move &other) const;
        bool operator != (const Move &other) const;
        char getSide();
        short getFrom();
        short getTo();
        short getLand();
        std::vector<std::shared_ptr<Move::Move> > getChildren();
        std::shared_ptr<Move::Move> getChildMove();
        void addChild(std::shared_ptr<Move::Move> m);
        void setChildMove(std::shared_ptr<Move::Move> m);
};

#endif
