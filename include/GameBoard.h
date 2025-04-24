#ifndef _GAMEBOARD_H
#define _GAMEBOARD_H

#include<iostream>

using namespace std;

enum class player{NONE = ' ', X = 'X', O = 'O'};

struct cell
{
    int row;
    int col;
};


class game
{
    private:

    player board[3][3];
    player winner;
    player currentPlayer;
    int movesCounter;

    public:

    game();
    void reset();
    bool makeMove(int row , int col);
    player getWinner();
    player getCurrentPlayer();
    player getCell(int row, int col);
    void getAvailableMOves(cell moves[]);
    void printBoard();
    bool isBoardFull();
};

#endif