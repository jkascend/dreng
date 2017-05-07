#include <iostream>
#include "engine/board.h"
using namespace std;


int main()
{
    Board::Board b;
    b.makeWhiteKing(0);
    b.makeBlackKing(31);
    cout << b.getWhite() << endl;
    cout << b.getBlack() << endl;
    return 0;
}
