#include "../../catch.hpp"
#include "../../engine/game.h"
#include "../../engine/board.h"

#include <vector>
#include <iostream>

TEST_CASE("Game Initialization", "[Game]") {
    Game::Game g;
    Board gameBoard = g.getBoard();
    Board expectedBoard = Board::Board();
    REQUIRE(expectedBoard.getFreePos() == gameBoard.getFreePos());
    REQUIRE(expectedBoard.getCurMove() == gameBoard.getCurMove());
}

TEST_CASE("Generates expected jumps", "[Game]") {
    Game::Game g;
    g.updateBoardState((unsigned long)1 << 49, 11263);
    
    Move::Move expected(Move(17, 13, 'b', true));
    
    std::vector<Move> generatedMoves = g.getCurrentMoves();

    REQUIRE(generatedMoves.size() == 1);
    
    Move m = generatedMoves.front();
    REQUIRE(m.getFrom() == expected.getFrom());
    REQUIRE(m.getTo() == expected.getTo());
    REQUIRE(m.getLand() == expected.getLand());
}

TEST_CASE("Generates expected moves", "[Game]") {
    Game::Game g;
    std::vector<Move> moves;
    moves.push_back(Move(20, 16, 'b', false));
    moves.push_back(Move(20, 17, 'b', false));
    moves.push_back(Move(21, 17, 'b', false));
    moves.push_back(Move(21, 18, 'b', false));
    moves.push_back(Move(22, 18, 'b', false));
    moves.push_back(Move(22, 19, 'b', false));
    moves.push_back(Move(23, 19, 'b', false));
    std::vector<Move> generatedMoves = g.getCurrentMoves();

    std::vector<Move>::iterator it;
    std::vector<Move>::iterator gen;
    
    for (it = moves.begin(); it < moves.end(); it++)
    {
        bool found = false;
        for (gen = generatedMoves.begin(); gen < generatedMoves.end(); gen++)
        {
            if (it->getFrom() == gen->getFrom()
                && it->getTo() == gen->getTo()
                && it->getSide() == gen->getSide())
                {
                    found = true;
                    break;
                }
        }
        REQUIRE(found);
    }
}
