#include "../../catch.hpp"
#include "../../engine/game.h"
#include "../../engine/board.h"

#include <vector>
#include <memory>

TEST_CASE("Game Initialization", "[Game]") {
    Game::Game g;
    Board gameBoard = g.getBoard();
    Board expectedBoard = Board::Board();
    REQUIRE(expectedBoard.getFreePos() == gameBoard.getFreePos());
    REQUIRE(expectedBoard.getCurMove() == gameBoard.getCurMove());
}

TEST_CASE("Gets expected moves for white man", "[Game]") {
    Game::Game g;
    g.updateBoardState((unsigned long)1 << 17, (unsigned long)1 << 6, 'w');

    Move::Move upLeft(6, 10, 'w', false);
    Move::Move upRight(6, 11, 'w', false);

    std::vector<std::shared_ptr<Move> > gMoves = g.getCurrentMoves();

    REQUIRE(gMoves.size() == 2);
    REQUIRE(upLeft == *(gMoves[0]));
    REQUIRE(upRight == *(gMoves[1]));
}

TEST_CASE("Gets expected moves for black man", "[Game]") {
    Game::Game g;
    g.updateBoardState((unsigned long)1 << 17, (unsigned long)1 << 6, 'b');

    Move::Move downLeft(17, 12, 'b', false);
    Move::Move downRight(17, 13, 'b', false);

    std::vector<std::shared_ptr<Move> > gMoves = g.getCurrentMoves();

    REQUIRE(gMoves.size() == 2);
    REQUIRE(downLeft == *(gMoves[0]));
    REQUIRE(downRight == *(gMoves[1]));
}

TEST_CASE("Gets expected moves for white king", "[Game]") {
    Game::Game g;
    g.updateBoardState(1 << 25, (unsigned long)1 << 45, 'w');

    Move::Move upLeft(13, 17, 'w', false);
    Move::Move upRight(13, 18, 'w', false);
    Move::Move downLeft(13, 9, 'w', false);
    Move::Move downRight(13, 10, 'w', false);

    std::vector<std::shared_ptr<Move> > gMoves = g.getCurrentMoves();

    REQUIRE(gMoves.size() == 4);
    REQUIRE(upLeft == *(gMoves[0]));
    REQUIRE(upRight == *(gMoves[1]));
    REQUIRE(downRight == *(gMoves[2]));
    REQUIRE(downLeft == *(gMoves[3]));
}

TEST_CASE("Gets expected moves for black king", "[Game]") {
    Game::Game g;
    g.updateBoardState((unsigned long)1 << 58, 7);

    Move::Move upLeft(26, 29, 'b', false);
    Move::Move upRight(26, 30, 'b', false);
    Move::Move downLeft(26, 21, 'b', false);
    Move::Move downRight(26, 22, 'b', false);

    std::vector<std::shared_ptr<Move> > gMoves = g.getCurrentMoves();

    REQUIRE(gMoves.size() == 4);
    REQUIRE(upLeft == *(gMoves[0]));
    REQUIRE(upRight == *(gMoves[1]));
    REQUIRE(downLeft == *(gMoves[2]));
    REQUIRE(downRight == *(gMoves[3]));
}

TEST_CASE("Generates expected jumps", "[Game]") {
    Game::Game g;
    g.updateBoardState((unsigned long)1 << 49, 11263);
    
    Move::Move expected(Move(17, 13, 'b', true));
    
    std::vector<std::shared_ptr<Move> > generatedMoves = g.getCurrentMoves();

    REQUIRE(generatedMoves.size() == 1);
    
    std::shared_ptr<Move> m = generatedMoves.front();
    REQUIRE(*m == expected);
}

TEST_CASE("Chained jumps", "[Game]") {
    Game::Game g;
    unsigned long black = 17592186044416;
    unsigned long white = 1536;
    g.updateBoardState(black, white);

    Move::Move expected(12, 9, 'b', true);
    Move::Move expectedChild(5, 10, 'b', true);

    std::vector<std::shared_ptr<Move> > generatedMoves = g.getCurrentMoves();
    std::shared_ptr<Move> m = generatedMoves.front();
    std::shared_ptr<Move> child = m->getChildren()[0];

    REQUIRE(generatedMoves.size() == 1);
    REQUIRE(*m == expected);
    REQUIRE(*child == expectedChild);
}

TEST_CASE("Multiple chained jumps", "[Game]") {
    Game::Game g;
    g.updateBoardState((unsigned long)1 << 56, 1060880);

    Move::Move parent(24, 20, 'b', true);
    Move::Move childA(17, 12, 'b', true);
    Move::Move childB(17, 13, 'b', true);
    Move::Move grandChild(8, 4, 'b', true);

    std::vector<std::shared_ptr<Move> > generatedMoves = g.getCurrentMoves();

    REQUIRE(generatedMoves.size() == 1);
    std::shared_ptr<Move> gParent = generatedMoves[0];

    REQUIRE(gParent->getChildren().size() == 2);
    std::shared_ptr<Move> gChildA = gParent->getChildren()[0];
    std::shared_ptr<Move> gChildB = gParent->getChildren()[1];

    REQUIRE(gChildA->getChildren().size() == 1);
    REQUIRE(gChildB->getChildren().size() == 0);
    std::shared_ptr<Move> gGrandChild = gChildA->getChildren()[0];

    REQUIRE(parent == *gParent);
    REQUIRE(childA == *gChildA);
    REQUIRE(childB == *gChildB);
    REQUIRE(grandChild == *gGrandChild);
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
    std::vector<std::shared_ptr<Move> > generatedMoves = g.getCurrentMoves();

    std::vector<Move>::iterator it;
    
    for (it = moves.begin(); it < moves.end(); it++)
    {
        bool found = false;
        for (size_t i = 0; i < generatedMoves.size(); i++)
        {
            if (it->getFrom() == generatedMoves[i]->getFrom()
                && it->getTo() == generatedMoves[i]->getTo()
                && it->getSide() == generatedMoves[i]->getSide())
                {
                    found = true;
                    break;
                }
        }
        REQUIRE(found);
    }
}
