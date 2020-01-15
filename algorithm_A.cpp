#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"
using namespace std;

template <class T>
class stack;

template <class T>
class link{
    friend class stack<T>;
public:
    link(T &x);
private:
    T value;
    link<T> *next;
};

template <class T>
class stack{
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

class Point{
public:
    Point(int _r, int _c);
    Point();
    void set_row_col(int _r, int _c);
    int r, c;
};

class Neighbor{
public:
    Neighbor();
    Point *GetNeighbor(int i, int j);
private:
    Point *neighbor[5][6];
};

int chain(Board board, Player player);

int SCORE(Board board, Player player);

int minimax(int depth, bool maximizing_player, int alpha, int beta, Board board, Player R_player, Player B_player, int *x_pos, int *y_pos);


template <class T>
link<T>::link(T &x){
    value = x;
    next = nullptr;
}

template <class T>
stack<T>::stack(){
    topp = nullptr;
    my_size = 0;
}

template <class T>
void stack<T>::push(T &n){
    link<T> *tmp = new link<T>(n);
    if (empty()){
        topp = tmp;
    }
    else{
        tmp->next = topp;
        topp = tmp;
    }
    my_size++;
}

template <class T>
void stack<T>::pop(){
    if (empty())
        return;
    link<T> *deletenode = topp;
    topp = topp->next;
    delete deletenode;
    my_size--;
}

template <class T>
bool stack<T>::empty(){
    return topp == nullptr;
}

template <class T>
T &stack<T>::top(){
    return topp->value;
}

template <class T>
int stack<T>::size(){
    return my_size;
}

Point::Point(int _r, int _c){
    r = _r;
    c = _c;
}

Point::Point(){}

void Point::set_row_col(int _r, int _c){
    r = _r;
    c = _c;
}

Neighbor::Neighbor(){
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 6; j++){
            int l = 0;
            neighbor[i][j] = new Point[4];
            for (int k = 0; k < 4; k++)
                neighbor[i][j][k].set_row_col(-1, -1);
            if (i - 1 >= 0)
                neighbor[i][j][l++].set_row_col(i - 1, j);
            if (i + 1 <= 4)
                neighbor[i][j][l++].set_row_col(i + 1, j);
            if (j - 1 >= 0)
                neighbor[i][j][l++].set_row_col(i, j - 1);
            if (j + 1 <= 5)
                neighbor[i][j][l].set_row_col(i, j + 1);
        }
    }
}

Point *Neighbor::GetNeighbor(int i, int j){
    return neighbor[i][j];
}

int chain(Board board, Player player){
    int length = 0;
    Neighbor n;
    bool visited[5][6] = {0};
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 6; j++){
            if (board.get_orbs_num(i, j) == board.get_capacity(i, j) - 1 && board.get_cell_color(i, j) == player.get_color()){
                int len = 0;
                stack<Point> visit_stack;
                Point p(i, j);
                visit_stack.push(p);
                while (!visit_stack.empty()){
                    p = visit_stack.top();
                    visit_stack.pop();
                    visited[p.r][p.c] = true;
                    len += 1;
                    Point *neighbor = n.GetNeighbor(p.r, p.c);
                    for (int k = 0; k < 4; k++){
                        if (neighbor[k].r != -1){
                            if (board.get_orbs_num(neighbor[k].r, neighbor[k].c) == board.get_capacity(neighbor[k].r, neighbor[k].c) - 1 && board.get_cell_color(neighbor[k].r, neighbor[k].c) == player.get_color()){
                                p.set_row_col(neighbor[k].r, neighbor[k].c);
                                if (visited[p.r][p.c] == false)
                                    visit_stack.push(p);
                            }
                        }
                    }
                }
                if (len > 1)
                    length += len;
            }
        }
    }
    return length;
}
int SCORE(Board board, Player player){
    Neighbor n;
    int sc = 0;
    int my_orbs = 0;
    int enemy_orbs = 0;
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 6; j++){
            if (player.get_color() == board.get_cell_color(i, j)){
                my_orbs += board.get_orbs_num(i, j);
                bool flag_not_vulnerable = true;
                Point *neighbor = n.GetNeighbor(i, j);
                for (int k = 0; k < 4; k++){
                    if (neighbor[k].r != -1){
                        if (board.get_cell_color(neighbor[k].r, neighbor[k].c) != 'w' && board.get_cell_color(neighbor[k].r, neighbor[k].c) != player.get_color() && board.get_orbs_num(neighbor[k].r, neighbor[k].c) == board.get_capacity(neighbor[k].r, neighbor[k].c) - 1){
                            sc -= 5 - board.get_capacity(i, j);
                            flag_not_vulnerable = false;
                        }
                    }
                }
                if (flag_not_vulnerable == true){
                    if (board.get_capacity(i, j) == 3)
                        sc += 2;
                    else if (board.get_capacity(i, j) == 2)
                        sc += 3;
                    if (board.get_orbs_num(i, j) == board.get_capacity(i, j) - 1)
                        sc += 2;
                }
            }
            else
                enemy_orbs += board.get_orbs_num(i, j);
        }
    }
    sc += my_orbs;
    if (enemy_orbs == 0 && my_orbs > 1)
        return 10000;
    else if (my_orbs == 0 && enemy_orbs > 1)
        return -10000;
    sc += 2 * chain(board, player);
    return sc;
}
int minimax(int depth, bool maximizing_player, int alpha, int beta, Board board, Player R_player, Player B_player, int *x_pos, int *y_pos){
    if (depth == 3){
        return SCORE(board, R_player);
    }
    Board board_copy = board;
    if (maximizing_player){
        int best = -10000;
        bool flag = false;
        for (int i = 0; i < 5; i++){
            for (int j = 0; j < 6; j++){
                if (board.place_orb(i, j, &R_player)){
                    int val = minimax(depth + 1, false, alpha, beta, board, R_player, B_player, x_pos, y_pos);
                    if (best < val && depth == 0){
                        *x_pos = i;
                        *y_pos = j;
                    }
                    best = max(best, val);
                    alpha = max(alpha, best);
                    board = board_copy;
                    if (beta <= alpha){
                        flag = true;
                        break;
                    }
                }
            }
            if(flag == true)
                break;
        }
        return best;
    }
    else{
        int best = 10000;
        bool flag = false;
        for (int i = 0; i < 5; i++){
            for (int j = 0; j < 6; j++){
                //Board board_copy = board;
                if (board.place_orb(i, j, &B_player)){
                    int val = minimax(depth + 1, true, alpha, beta, board, R_player, B_player, x_pos, y_pos);
                    best = min(best, val);
                    beta = min(beta, best);
                    board = board_copy;
                    if (beta <= alpha){
                        flag = true;
                        break;
                    }
                }
            }
            if(flag == true)
                break;
        }
        return best;
    }
}

void algorithm_A(Board board, Player player, int index[]){
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
}

