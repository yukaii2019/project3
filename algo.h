#ifndef INCLUDE_ALGO_H_
#define INCLUDE_ALGO_H_
#include "../include/board.h"
#include "../include/player.h"
#include "../include/rules.h"

template <class T>
class stack;

template <class T>
class link
{
    friend class stack<T>;

public:
    link(T &x);

private:
    T value;
    link<T> *next;
};

template <class T>
class stack
{
public:
    stack();
    void push(T &n);
    void pop();
    bool empty();
    T &top();
    int size();

private:
    link<T> *topp;
    int my_size;
};

class Point
{
public:
    Point(int _r, int _c);
    Point();
    void set_row_col(int _r, int _c);
    int r, c;
};

class Neighbor
{
public:
    Neighbor();
    Point *GetNeighbor(int i, int j);

private:
    Point *neighbor[5][6];
};

int chain(Board board, Player player);

int SCORE(Board board, Player player);

int minimax(int depth, bool maximizing_player, int alpha, int beta, Board board, Player R_player, Player B_player, int *x_pos, int *y_pos);

#endif