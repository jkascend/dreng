#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include "board.h"
#include "jump.h"
#include "move.h"

class Game
{
    private:
        Board _board;
        std::vector<short> getSimpleMoves(short pos, unsigned long allPos, char color, bool isKing);
        std::vector<short> getJumpMoves(short pos, unsigned long opponentPieces, char color, bool isKing);
        std::vector<short> getMovesForPos(short pos);
        bool isOnBoard(short pos);
        bool canJump(short startPos, short enemyPos);
        short getAbsolutePosition(short pos);
        std::vector<std::shared_ptr<Move::Move> > getJumpsForPos(short pos, short from, unsigned long opponentPieces, char color, bool isKing);
    public:
        Game();
        Board getBoard();
        bool isGameOver();
        void updateBoardState(char curTurn);
        void updateBoardState(unsigned long black, unsigned long white);
        void updateBoardState(unsigned long black, unsigned long white, char curTurn);
        std::vector<std::shared_ptr<Move::Move> > getCurrentMoves();
};

#endif
