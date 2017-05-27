#include "player.h"

Player::Player(std::string name)
{
    Player::_name = name;
}

std::string Player::getName()
{
    return Player::_name;
}

void Player::setName(std::string name)
{
    Player::_name = name;
}