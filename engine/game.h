#ifndef GAME_H
#define GAME_H

#include <vector>
#include "board.h"
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
        // Move::Move getJump(short start, short jump, char color, unsigned long opponentPieces);
    public:
        Game();
        Board getBoard();
        void updateBoardState(unsigned long black, unsigned long white);
        std::vector<Move> getCurrentMoves();
};

#endif
