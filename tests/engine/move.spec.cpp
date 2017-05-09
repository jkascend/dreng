#include "../../catch.hpp"
#include "../../engine/move.h"

TEST_CASE("Initializations", "[Move]") {
    Move::Move m(2, 3, 'b', true);
    REQUIRE(m.getSide() == 'b');
    REQUIRE(m.getFrom() == 2);
    REQUIRE(m.getTo() == 3);
}

TEST_CASE("Add child", "[Move]") {
    Move m(1, 2, 'b', true);
    std::shared_ptr<Move> child(new Move(2, 3, 'w', false));
    m.addChild(child);
}

TEST_CASE("Get child", "[Move]") {
    Move::Move m(0, 4, 'w', true);
    std::shared_ptr<Move> child1(new Move(9, 12, 'w', true));
    std::shared_ptr<Move> child2(new Move(9, 13, 'w', true));
    m.addChild(child1);
    m.addChild(child2);

    std::shared_ptr<Move> fetched1 = m.getChildren()[0];
    std::shared_ptr<Move> fetched2 = m.getChildren()[1];

    REQUIRE(child1.get()->getFrom() == fetched1.get()->getFrom());
    REQUIRE(child2.get()->getFrom() == fetched2.get()->getFrom());
    REQUIRE(fetched1.get()->getTo() == 12);
    REQUIRE(fetched2.get()->getTo() == 13);
}

TEST_CASE("Has children", "[Move]") {
    Move::Move a(0, 4, 'w', false);
    Move::Move m(0, 4, 'w', false);
    std::shared_ptr<Move> child(new Move(2, 4, 'w', false));
    m.addChild(child);
    REQUIRE(m.hasChildren());
    REQUIRE(!a.hasChildren());
}

TEST_CASE("Generates landing for %8<4 down right", "[Move]") {
    Move::Move m(12, 9, 'w', true);
    REQUIRE(m.getLand() == 5);
}

TEST_CASE("Generates landing for %8<4 down left", "[Move]") {
    Move::Move m(13, 9, 'w', true);
    REQUIRE(m.getLand() == 4);
}

TEST_CASE("Generates landing for %8<4 up right", "[Move]") {
    Move::Move m(4, 9, 'w', true);
    REQUIRE(m.getLand() == 13);
}

TEST_CASE("Generates landing for %8<4 up left", "[Move]") {
    Move::Move m(5, 9, 'w', true);
    REQUIRE(m.getLand() == 12);
}

TEST_CASE("Generates landing for %8>4 down right", "[Move]") {
    Move::Move m(16, 12, 'w', true);
    REQUIRE(m.getLand() == 9);
}

TEST_CASE("Generates landing for %8>4 down left", "[Move]") {
    Move::Move m(17, 12, 'w', true);
    REQUIRE(m.getLand() == 8);
}

TEST_CASE("Generates landing for %8>4 up left", "[Move]") {
    Move::Move m(8, 12, 'w', true);
    REQUIRE(m.getLand() == 17);
}

TEST_CASE("Generates landing for %8>4 up right", "[Move]") {
    Move::Move m(9, 12, 'w', true);
    REQUIRE(m.getLand() == 16);
}

TEST_CASE("Off board returns -1", "[Move]") {
    Move::Move a(4, 8, 'w', true);
    Move::Move b(24, 28, 'w', true);
    Move::Move c(19, 23, 'w', true);
    Move::Move d(7, 3, 'w', true);

    REQUIRE(a.getLand() == -1);
    REQUIRE(b.getLand() == -1);
    REQUIRE(c.getLand() == -1);
    REQUIRE(d.getLand() == -1);
}

TEST_CASE("== operator works", "[Move]") {
    Move::Move a(4, 9, 'w', true);
    Move::Move b(4, 9, 'w', true);

    REQUIRE(a == b);
}

TEST_CASE("== operator additional tests" "[Move]") {
    Move::Move a(4, 9, 'w', false);
    Move::Move b(4, 9, 'w', false);

    REQUIRE(a == b);
}

TEST_CASE("!= operator works", "[Move]") {
    Move::Move a(5, 9, 'w', true);
    Move::Move b(4, 9, 'w', true);
    Move::Move c(5, 10, 'w', true);
    Move::Move d(5, 9, 'b', true);
    Move::Move e(5, 9, 'w', false);

    REQUIRE(a != b);
    REQUIRE(a != c);
    REQUIRE(a != c);
    REQUIRE(a != d);
    REQUIRE(a != e);
}