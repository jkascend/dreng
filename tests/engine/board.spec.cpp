#include "../../catch.hpp"
#include "../../engine/board.h"
#include <iostream>
TEST_CASE("Initialization", "[Board]") {
    Board::Board b;
    REQUIRE(b.getWhite() == 4095);
    REQUIRE(b.getBlack() == 4293918720);
    REQUIRE(b.getCurMove() == 'b');
}

TEST_CASE("Make white king", "[Board]") {
    Board::Board b;
    b.makeWhiteKing(0);
    REQUIRE(b.getWhite() == 4294971390);
}

TEST_CASE("Make black king", "[Board]") {
    Board::Board b;
    // position of king
    unsigned long expected = (unsigned long)1 << 63;
    // remaining initial pieces
    for (size_t i = 20; i < 31; i++)
    {
        expected |= (unsigned long)1 << i;
    }
    b.makeBlackKing(31);
    REQUIRE(b.getBlack() == expected);
}

TEST_CASE("White king checks bits before swapping", "[Board]") {
    Board::Board b;
    REQUIRE_THROWS(b.makeWhiteKing(15));
}

TEST_CASE("White king checks position is 32-bit", "[Board]") {
    Board::Board b;
    REQUIRE_THROWS(b.makeWhiteKing(33));
}

TEST_CASE("Black king checks bits before swapping", "[Board]") {
    Board::Board b;
    REQUIRE_THROWS(b.makeBlackKing(0));
}

TEST_CASE("Black king checks position is 32-bit", "[Board]") {
    Board::Board b;
    REQUIRE_THROWS(b.makeBlackKing(33));
}

TEST_CASE("Square is free", "[Board]") {
    Board::Board b;
    // bits 3, 21, 41
    b.setWhite(2199025352712);
    // bits 5, 8, 32
    b.setBlack(4294967584);
    REQUIRE(!b.squareIsFree(0));
    REQUIRE(!b.squareIsFree(5));
    REQUIRE(!b.squareIsFree(8));
    REQUIRE(!b.squareIsFree(3));
    REQUIRE(!b.squareIsFree(21));
    REQUIRE(b.squareIsFree(1));
    REQUIRE(b.squareIsFree(24));
    REQUIRE(b.squareIsFree(31));
}

TEST_CASE("Square is owned by white", "[Board]") {
    Board::Board b;
    // set bits 1 and 34
    unsigned long white = 1;
    white |= (unsigned long)1 << 34;
    b.setWhite(white);
    REQUIRE(b.whiteIsSquareOwner(0));
    REQUIRE(b.whiteIsSquareOwner(2));
    REQUIRE(!b.whiteIsSquareOwner(1));
}

TEST_CASE("Square is owned by black", "[Board]") {
    Board::Board b;
    unsigned long black = 1 << 27;
    black |= (unsigned long)1 << 46;
    b.setBlack(black);
    REQUIRE(b.blackIsSquareOwner(27));
    REQUIRE(b.blackIsSquareOwner(14));
    REQUIRE(!b.blackIsSquareOwner(31));
}

TEST_CASE("Square is owned by opponent", "[Board]") {
    Board::Board b;
    REQUIRE(b.opponentIsSquareOwner(0));
    REQUIRE(!b.opponentIsSquareOwner(15));
    b.setCurMove('w');
    REQUIRE(b.opponentIsSquareOwner(28));
    REQUIRE(!b.opponentIsSquareOwner(1));
}
