#include "../../catch.hpp"
#include "../../engine/board.h"

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
    b.makeBlackKing(31);
    REQUIRE(b.getBlack() == (unsigned long)9223372039001210880);
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
    b.setWhite((unsigned long)17179869185);
    REQUIRE(b.whiteIsSquareOwner(0));
    REQUIRE(b.whiteIsSquareOwner(2));
    REQUIRE(!b.whiteIsSquareOwner(1));
}

TEST_CASE("Square is owned by black", "[Board]") {
    Board::Board b;
    b.setBlack((unsigned long)70368878395392);
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
