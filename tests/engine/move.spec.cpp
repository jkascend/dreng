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
    Move *child = new Move(2, 3, 'w', false);
    m.addChild(child);
}

TEST_CASE("Get child", "[Move]") {
    Move::Move m(0, 4, 'w', true);
    Move::Move *child = new Move(9, 12, 'w', true);
    Move::Move *child2 = new Move(9, 13, 'w', true);
    m.addChild(child);
    m.addChild(child2);

    Move::Move *fetched1 = m.getChildren()[0];
    Move::Move *fetched2 = m.getChildren()[1];

    REQUIRE(child->getFrom() == fetched1->getFrom());
    REQUIRE(child2->getFrom() == fetched2->getFrom());
}

TEST_CASE("Has children", "[Move]") {
    Move::Move a(0, 4, 'w', false);
    Move::Move m(0, 4, 'w', false);
    Move::Move *child = new Move(2, 4, 'w', false);
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
