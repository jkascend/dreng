#include "game.h"

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

vector<short> Game::getMovesForPos(short pos)
{
    vector<short> toCheck;
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
        if (Game::_board.opponentIsSquareOwner(*it) && Game::canJump(pos, *it))
        {
            if (!isKing && ((color == 'b' && *it > pos) || (color == 'w' && *it < pos)))
            {
                continue;
            }
            jumpPositions.push_back(*it);
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

vector<shared_ptr<Move::Move> > Game::getJumpsForPos(short pos, short lastJumpPos, unsigned long opponentPieces, char color, bool isKing)
{
    vector<shared_ptr<Move::Move> > retVal;
    vector<short> jumpMoves = Game::getJumpMoves(pos, opponentPieces, color, isKing);
    for (int i = 0; i < jumpMoves.size(); i++)
    {
        if (jumpMoves[i] == lastJumpPos) continue;
        shared_ptr<Move::Move> jump(new Move(pos, jumpMoves[i], color, true));
        vector<shared_ptr<Move::Move> > childMoves = Game::getJumpsForPos(jump->getLand(), jump->getTo(), opponentPieces, color, isKing);
        for (int j = 0; j < childMoves.size(); j++)
        {
            jump->addChild(childMoves[j]);
        }
        retVal.push_back(jump);
    }
    return retVal;
}

vector<shared_ptr<Move::Move> > Game::getCurrentMoves()
{
    vector<shared_ptr<Move::Move> > allMoves;
    char curTurn = Game::_board.getCurMove();
    unsigned long curPieces = curTurn == 'b' ? Game::_board.getBlack() : Game::_board.getWhite();
    unsigned long opponentPieces = curTurn == 'b' ? Game::_board.getWhite() : Game::_board.getBlack();
    unsigned long allPos = Game::_board.getFreePos();
    for (short i = 0; i < 64; i++)
    {
        if (curPieces & ((unsigned long)1 << i))
        {
            short absolutePosition = Game::getAbsolutePosition(i);
            std::vector<std::shared_ptr<Move::Move> > jumpsForPos = Game::getJumpsForPos(absolutePosition, -1, opponentPieces, curTurn, i > 31);
            allMoves.insert(std::end(allMoves), std::begin(jumpsForPos), std::end(jumpsForPos));
        }
    }

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
                    std::shared_ptr<Move::Move> move(new Move(absolutePosition, *simpleIt, curTurn, false));
                    allMoves.push_back(move);
                }
            }
        }
    }

    return allMoves;
}

void Game::updateBoardState(char curTurn)
{
    Game::_board.setCurMove(curTurn);
}

void Game::updateBoardState(unsigned long black, unsigned long white)
{
    Game::_board.setWhite(white);
    Game::_board.setBlack(black);
}

void Game::updateBoardState(unsigned long black, unsigned long white, char curTurn)
{
    Game::_board.setWhite(white);
    Game::_board.setBlack(black);
    Game::_board.setCurMove(curTurn);
}