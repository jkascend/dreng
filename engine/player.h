#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player
{
    private:
        std::string _name;
    public:
        Player(std::string name);
        std::string getName();
        void setName(std::string name);
};

#endif
