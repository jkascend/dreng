#include <iostream>
#include "engine/board.h"

int main()
{
    /*
     * Temp example code.
     * Later replace with example app.
     */
    std::cout << "Hello World!" << std::endl;
    Board::Board b;
    std::cout << b.getWhite() << std::endl;
    std::cout << "black: " << b.getBlack() << std::endl;
    return 0;
}
