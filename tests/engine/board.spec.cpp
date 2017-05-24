#include "../../catch.hpp"
#include "../../engine/board.h"
#include "../../engine/move.h"

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

TEST_CASE("Perform move checks current square is XOR man/king", "[Board]") {
    Board::Board b;
    b.setBlack(b.getBlack() | (unsigned long)1 << 52);
    std::shared_ptr<Move::Move> m(new Move(20, 17, 'b', false));
    REQUIRE_THROWS(b.performMove(m));
}

TEST_CASE("Perform move checks current square is XOR white man/king", "[Board]") {
    Board::Board b;
    b.setWhite(b.getWhite() | (unsigned long)1 << 40);
    std::shared_ptr<Move::Move> m(new Move(8, 12, 'w', false));
    b.setCurMove('w');
    REQUIRE_THROWS(b.performMove(m));
}

TEST_CASE("Perform move checks destination square is completely unoccupied by black man", "[Board]") {
    Board::Board b;
    std::shared_ptr<Move::Move> m(new Move(24, 20, 'b', false));
    REQUIRE_THROWS(b.performMove(m));
}

TEST_CASE("Perform move checks destination square is completely unoccupied by black king", "[Board]") {
    Board::Board b;
    b.setBlack(b.getBlack() | (unsigned long)1 << 48);
    std::shared_ptr<Move::Move> m(new Move(20, 16, 'b', false));
    REQUIRE_THROWS(b.performMove(m));
}

TEST_CASE("Perform move checks destination square is completely unoccupied by white man", "[Board]") {
    Board::Board b;
    b.setCurMove('w');
    std::shared_ptr<Move::Move> m(new Move(5, 9, 'w', false));
    REQUIRE_THROWS(b.performMove(m));
}

TEST_CASE("Perform move checks destination square is completely unoccupied by white king", "[Board]") {
    Board::Board b;
    b.setCurMove('w');
    b.setWhite(b.getWhite() | (unsigned long)1 << 44);
    std::shared_ptr<Move::Move> m(new Move(9, 12, 'w', false));
    REQUIRE_THROWS(b.performMove(m));
}

TEST_CASE("Perform move sets proper bits for black", "[Board]") {
    Board::Board b;
    unsigned long expectedBlack = b.getBlack();
    unsigned long expectedWhite = b.getWhite();
    expectedBlack ^= (1 << 20);
    expectedBlack |= (1 << 17);
    std::shared_ptr<Move::Move> m(new Move(20, 17, 'b', false));
    b.performMove(m);
    REQUIRE(expectedBlack == b.getBlack());
    REQUIRE(expectedWhite == b.getWhite());
}

TEST_CASE("Perform move sets proper bits for white", "[Board]") {
    Board::Board b;
    b.setCurMove('w');
    unsigned long expectedBlack = b.getBlack();
    unsigned long expectedWhite = b.getWhite();
    expectedWhite ^= (1 << 9);
    expectedWhite |= (1 << 12);
    std::shared_ptr<Move::Move> m(new Move(9, 12, 'w', false));
    b.performMove(m);
    REQUIRE(expectedWhite == b.getWhite());
    REQUIRE(expectedBlack == b.getBlack());
}

TEST_CASE("Perform move sets proper bits for black king", "[Board]") {
    Board::Board b;
    unsigned long expectedBlack = (unsigned long)1 << 49;
    b.setBlack((unsigned long)1 << 52);
    std::shared_ptr<Move::Move> m(new Move(20, 17, 'b', false));
    b.performMove(m);
    REQUIRE(expectedBlack == b.getBlack());
}

TEST_CASE("Perform move sets proper bits for white king", "[Board]") {
    Board::Board b;
    unsigned long expectedWhite = (unsigned long)1 << 45;
    b.setWhite((unsigned long)1 << 41);
    b.setCurMove('w');
    std::shared_ptr<Move::Move> m(new Move(9, 13, 'w', false));
    b.performMove(m);
    REQUIRE(expectedWhite == b.getWhite());
}

TEST_CASE("Perform move checks move corresponds to current turn", "[Board]") {
    Board::Board b;

    std::shared_ptr<Move::Move> m(new Move(9, 12, 'w', false));

    REQUIRE_THROWS(b.performMove(m));
}

TEST_CASE("Board updates current move after move", "[Board]") {
    Board::Board b;
    std::shared_ptr<Move::Move> m(new Move(20, 16, 'b', false));
    b.performMove(m);
    REQUIRE(b.getCurMove() == 'w');
}

TEST_CASE("Board state is valid returns true", "[Board]") {
    Board::Board b;
    REQUIRE(b.boardStateIsValid());
}

TEST_CASE("Board state is valid returns false for black man and king on at same pos", "[Board]") {
    Board::Board b;
    b.setBlack(b.getBlack() | ((unsigned long)1 << 60));
    REQUIRE(b.boardStateIsValid() == false);
}

TEST_CASE("Board state is valid returns false for white man and king on at same pos", "[Board]") {
    Board::Board b;
    b.setWhite(b.getWhite() | ((unsigned long)1 << 37));
    REQUIRE(b.boardStateIsValid() == false);
}

TEST_CASE("Perform move throws when from pos is on for black man/king", "[Board]") {
    Board::Board b;
    b.setBlack(b.getBlack() | ((unsigned long)1 << 52));
    b.setWhite(1 << 17);
    std::shared_ptr<Move> m(new Move(20, 17, 'b', true));
    REQUIRE_THROWS(b.performMove(m));
}

TEST_CASE("Perform move throws when from pos is on for white man/king", "[Board]") {
    Board::Board b;
    b.setCurMove('w');
    b.setWhite(b.getWhite() | ((unsigned long)1 << 42));
    b.setWhite(1 << 17);
    b.setBlack(1 << 13);
    std::shared_ptr<Move> m(new Move(10, 13, 'w', true));
    REQUIRE_THROWS(b.performMove(m));
}

TEST_CASE("Perform move throws if black jump landing square occupied by friendly man", "[Board]") {
    Board::Board b;
    b.setWhite(1 << 17);
    b.setBlack(b.getBlack() | 1 << 13);
    std::shared_ptr<Move> m(new Move(20, 17, 'b', true));
    REQUIRE_THROWS(b.performMove(m));
}

TEST_CASE("Perform move throws if black jump landing square occupied by friendly king", "[Board]") {
    Board::Board b;
    b.setWhite(1 << 17);
    b.setBlack(b.getBlack() | (unsigned long)1 << 45);
    std::shared_ptr<Move> m(new Move(20, 17, 'b', true));
    REQUIRE_THROWS(b.performMove(m));
}

TEST_CASE("Perform move throws if white jump landing square occupied by friendly man", "[Board]") {
    Board::Board b;
    b.setBlack(1 << 13);
    b.setWhite(b.getWhite() | 1 << 17);
    b.setCurMove('w');
    std::shared_ptr<Move> m(new Move(10, 13, 'w', true));
    REQUIRE_THROWS(b.performMove(m));
}

TEST_CASE("Perform move throws if white jump landing square occupied by friendly king", "[Board]") {
    Board::Board b;
    b.setBlack(1 << 13);
    b.setWhite(b.getWhite() | (unsigned long) 1 << 49);
    b.setCurMove('w');
    std::shared_ptr<Move> m(new Move(10, 13, 'w', true));
    REQUIRE_THROWS(b.performMove(m));
}

TEST_CASE("Perform move throws if black jump landing square occupied by enemy man", "[Board]") {
    Board::Board b;
    b.setWhite(1 << 13 | 1 << 17);
    std::shared_ptr<Move> m(new Move(20, 17, 'b', true));
    REQUIRE_THROWS(b.performMove(m));
}

TEST_CASE("Perform move throws if black jump landing square occupied by enemy king", "[Board]") {
    Board::Board b;
    b.setWhite(1 << 17 | (unsigned long)1 << 45);
    std::shared_ptr<Move> m(new Move(20, 17, 'b', true));
    REQUIRE_THROWS(b.performMove(m));
}

TEST_CASE("Perform move throws if white jump landing square occupied by enemy man", "[Board]") {
    Board::Board b;
    b.setBlack(1 << 17 | 1 << 13);
    b.setCurMove('w');
    std::shared_ptr<Move> m(new Move(10, 13, 'w', true));
    REQUIRE_THROWS(b.performMove(m));
}

TEST_CASE("Perform move throws if white jump landing square occupied by enemy king", "[Board]") {
    Board::Board b;
    b.setBlack(1 << 13 | (unsigned long) 1 << 49);
    b.setCurMove('w');
    std::shared_ptr<Move> m(new Move(10, 13, 'w', true));
    REQUIRE_THROWS(b.performMove(m));
}

TEST_CASE("Single jump works for black man", "[Board]") {
    Board::Board b;
    b.setBlack(1 << 20);
    b.setWhite(1 << 17);
    std::shared_ptr<Move> m(new Move(20, 17, 'b', true));
    b.performMove(m);
    REQUIRE(b.getBlack() == 8192);
    REQUIRE(b.getWhite() == 0);
}

TEST_CASE("Simple jump works for white man", "[Board]") {
    Board::Board b;
    b.setBlack(1 << 20);
    b.setWhite(1 << 17);
    b.setCurMove('w');
    std::shared_ptr<Move> m(new Move(17, 20, 'w', true));
    b.performMove(m);
    REQUIRE(b.getBlack() == 0);
    REQUIRE(b.getWhite() == (1 << 24));
}

TEST_CASE("Simple jump works for black king", "[Board]") {
    Board::Board b;
    b.setBlack((unsigned long)1 << 52);
    b.setWhite(1 << 17);
    std::shared_ptr<Move> m(new Move(20, 17, 'b', true));
    b.performMove(m);
    REQUIRE(b.getBlack() == ((unsigned long)1 << 45));
    REQUIRE(b.getWhite() == 0);
}

TEST_CASE("Simple jump works for white king", "[Board]") {
    Board::Board b;
    b.setBlack(1 << 20);
    b.setWhite((unsigned long)1 << 49);
    b.setCurMove('w');
    std::shared_ptr<Move> m(new Move(17, 20, 'w', true));
    b.performMove(m);
    REQUIRE(b.getBlack() == 0);
    REQUIRE(b.getWhite() == (unsigned long)1 << 56);
}

TEST_CASE("Complex jump works for black man", "[Board]") {
    Board::Board b;
    b.setBlack(1 << 20);
    b.setWhite(1 << 17 | 1 << 10);
    std::shared_ptr<Move> m(new Move(20, 17, 'b', true));
    m->setChildMove(std::shared_ptr<Move>(new Move(13, 10, 'b', true)));
    b.performMove(m);
    REQUIRE(b.getBlack() == 1 << 6);
    REQUIRE(b.getWhite() == 0);
}

TEST_CASE("Complex jump works for white man", "[Board]") {
    Board::Board b;
    b.setBlack((unsigned long)1 << 45 | 1 << 20);
    b.setWhite(1 << 10);
    b.setCurMove('w');
    std::shared_ptr<Move> m(new Move(10, 13, 'w', true));
    m->setChildMove(std::shared_ptr<Move>(new Move(17, 20, 'w', true)));
    b.performMove(m);
    REQUIRE(b.getBlack() == 0);
    REQUIRE(b.getWhite() == (1 << 24));
}

TEST_CASE("Complex jump works for black king", "[Board]") {
    Board::Board b;
    b.setBlack((unsigned long)1 << 42);
    b.setWhite(1 << 20 | (unsigned long)1 << 45);
    std::shared_ptr<Move> m(new Move(10, 13, 'b', true));
    m->setChildMove(std::shared_ptr<Move>(new Move(17, 20, 'b', true)));
    b.performMove(m);
    REQUIRE(b.getBlack() == ((unsigned long)1 << 56));
    REQUIRE(b.getWhite() == 0);
}

TEST_CASE("Complex jump works for white king", "[Board]") {
    Board::Board b;
    b.setBlack(1 << 20 | (unsigned long)1 << 45);
    b.setWhite((unsigned long)1 << 56);
    b.setCurMove('w');
    std::shared_ptr<Move> m(new Move(24, 20, 'w', true));
    m->setChildMove(std::shared_ptr<Move>(new Move(17, 13, 'w', true)));
    b.performMove(m);
    REQUIRE(b.getBlack() == 0);
    REQUIRE(b.getWhite() == (unsigned long)1 << 42);
}

TEST_CASE("Simple move for black man ending in king row makes it a king", "[Board]") {
    Board::Board b;
    b.setBlack(1 << 5);
    b.setWhite(1 << 21);
    std::shared_ptr<Move> m(new Move(5, 2, 'b', false));
    b.performMove(m);
    REQUIRE(b.getBlack() == (unsigned long)1 << 34);
    REQUIRE(b.getWhite() == 1 << 21);
}

TEST_CASE("Simple move for white man ending in king row makes it a king", "[Board]") {
    Board::Board b;
    b.setBlack(1);
    b.setWhite(1 << 25);
    b.setCurMove('w');
    std::shared_ptr<Move> m(new Move(25, 28, 'w', false));
    b.performMove(m);
    REQUIRE(b.getBlack() == 1);
    REQUIRE(b.getWhite() == (unsigned long)1 << 60);
}

TEST_CASE("Jump move for white man ending in king row makes it a king", "[Board]") {
    Board::Board b;
    b.setBlack(1 << 26);
    b.setWhite(1 << 22);
    b.setCurMove('w');
    std::shared_ptr<Move> m(new Move(22, 26, 'w', true));
    b.performMove(m);
    REQUIRE(b.getBlack() == 0);
    REQUIRE(b.getWhite() == (unsigned long)1 << 61);
}

TEST_CASE("Jump move for black man ending in king row makes it a king", "[Board]") {
    Board::Board b;
    b.setBlack(1 << 9);
    b.setWhite(1 << 5);
    std::shared_ptr<Move> m(new Move(9, 5, 'b', true));
    b.performMove(m);
    REQUIRE(b.getBlack() == (unsigned long)1 << 34);
    REQUIRE(b.getWhite() == 0);
}
