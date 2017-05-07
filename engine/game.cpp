#include "game.h"
#include "jump.h"

Game::Game()
{
    Game::_board = Board::Board();
}

Board Game::getBoard()
{
    return Game::_board;
}

bool Game::isOnBoard(short pos)
{
    return pos >= 0 && pos < 32;
}

std::vector<short> Game::getMovesForPos(short pos)
{
    std::vector<short> toCheck;
    if ((pos % 8 == 0) || ((pos - 7) % 8 == 0))
    {
        toCheck.push_back(pos + 4);
        toCheck.push_back(pos - 4);
    }
    else if (pos % 8 < 4)
    {
        toCheck.push_back(pos + 3);
        toCheck.push_back(pos + 4);
        toCheck.push_back(pos - 5);
        toCheck.push_back(pos - 4);
    }
    else
    {
        toCheck.push_back(pos + 4);
        toCheck.push_back(pos + 5);
        toCheck.push_back(pos - 3);
        toCheck.push_back(pos - 4);
    }

    return toCheck;
}

bool Game::canJump(short startPos, short enemyPos)
{
    Jump j(startPos, enemyPos);
    return Game::_board.squareIsFree(j.computeLanding());
}

std::vector<short> Game::getJumpMoves(short pos, unsigned long opponentPieces, char color, bool isKing)
{
    std::vector<short>jumpPositions;
    std::vector<short>toCheck = Game::getMovesForPos(pos);
    std::vector<short>::iterator it;
    for (it = toCheck.begin(); it < toCheck.end(); it++)
    {
        if (Game::_board.opponentIsSquareOwner(*it))
        {
            if (Game::canJump(pos, *it))
            {
                if (!isKing && ((color == 'b' && *it > pos) || (color == 'w' && *it < pos)))
                {
                    continue;
                }
                jumpPositions.push_back(*it);
            }
        }
    }

    return jumpPositions;
}

short Game::getAbsolutePosition(short pos)
{
    if (pos < 32) return pos;
    return pos - 32;
}

std::vector<short> Game::getSimpleMoves(short pos, unsigned long allPos, char color, bool isKing)
{
    std::vector<short> retVal;
    std::vector<short> toCheck = Game::getMovesForPos(pos);
    
    std::vector<short>::iterator it;
    for (it = toCheck.begin(); it < toCheck.end(); it++)
    {
        if (Game::isOnBoard(*it))
        {
            if ((allPos & (1 << *it)) == 0)
            {
                bool isGreater = *it > pos;
                if (isKing)
                {
                    retVal.push_back(*it);
                }
                else
                {
                    if (color == 'b')
                    {
                        if (*it < pos)
                        {
                            retVal.push_back(*it);
                        }
                    }
                    else if (color == 'w')
                    {
                        if (*it > pos)
                        {
                            retVal.push_back(*it);
                        }
                    }
                }
            }
        }
    }

    return retVal;
}

// Move::Move Game::getJump(short start, short jump, char color, unsigned long opponentPieces)
// {
//     Move::Move ret(start, jump, color, true);

// }

std::vector<Move> Game::getCurrentMoves()
{
    std::vector<Move> allMoves;
    char curTurn = Game::_board.getCurMove();
    unsigned long curPieces = curTurn == 'b' ? Game::_board.getBlack() : Game::_board.getWhite();
    unsigned long opponentPieces = curTurn == 'b' ? Game::_board.getWhite() : Game::_board.getBlack();
    unsigned long allPos = Game::_board.getFreePos();
    for (short i = 0; i < 64; i++)
    {
        if (curPieces & ((unsigned long)1 << i))
        {
            short absolutePosition = Game::getAbsolutePosition(i);
            std::vector<short> jumpMoves = Game::getJumpMoves(absolutePosition, opponentPieces, curTurn, i > 31);
            std::vector<short>::iterator jIter;
            for (jIter = jumpMoves.begin(); jIter < jumpMoves.end(); jIter++)
            {
                allMoves.push_back(Move(absolutePosition, *jIter, curTurn, true));
            }
        }
    }
    //TODO: write more tests for moves!!

    if (!allMoves.size())
    {
        for (short i = 0; i < 64; i++)
        {
            if (curPieces & ((unsigned long)1 << i))
            {
                short absolutePosition = Game::getAbsolutePosition(i);
                std::vector<short> simpleMovePositions = Game::getSimpleMoves(absolutePosition, allPos, curTurn, i > 31);

                std::vector<short>::iterator simpleIt;
                for (simpleIt = simpleMovePositions.begin(); simpleIt < simpleMovePositions.end(); simpleIt++)
                {
                    allMoves.push_back(Move(i, *simpleIt, curTurn, false));
                }
            }
        }
    }

    return allMoves;
}

void Game::updateBoardState(unsigned long black, unsigned long white)
{
    Game::_board.setWhite(white);
    Game::_board.setBlack(black);
}