#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"
#include "algo.h"
using namespace std;

void algorithm_A(Board board, Player player, int index[])
{

    int row, col;
    char color = player.get_color();
    Player p1('r');
    Player p2('b');
    if (player.get_color() == 'r')
        minimax(0, true, -10000, 10000, board, p1, p2, &row, &col);
    else if (player.get_color() == 'b')
        minimax(0, true, -10000, 10000, board, p2, p1, &row, &col);

    index[0] = row;
    index[1] = col;
    cout << SCORE(board,player) << endl;
}