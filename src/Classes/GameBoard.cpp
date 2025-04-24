#include"../../include/GameBoard.h"

game::game()
{
    reset();
}

void game::reset()
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            board[i][j] = player::NONE;
        }
    }

    currentPlayer = player::X;
}

bool game::makeMove(int row, int col)
{
    if(row < 0 || row >= 3 || col < 0 || col >= 3)
    {
        return false;
    }

    if(board[row][col] != player::NONE)
    {
        return false;
    }

    board[row][col] = currentPlayer;
    movesCounter++;

    currentPlayer = (currentPlayer == player::X) ? player::O : player::X;

    return true;
}

player game::getCurrentPlayer()
{
    return currentPlayer;
}

player game::getCell(int row, int col)
{
    if(row < 0 || row >= 3 || col < 0 || col >= 3)
    {
        return player::NONE;
    }

    return board[row][col];
}

void game::getAvailableMOves(cell moves[])
{
    int index = 0;

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(board[i][j] == player::NONE)
            {
                moves[index].row = i;
                moves[index].col = j;
                index++;
            }
        }
    }
}

void game::printBoard()
{
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            cout << (char)board[i][j];

            if(j<2) cout<< " | ";
        }
        
        if(i<2) cout<< "--+---+--\n";
    }
}

player game::getWinner()
{
    for(int i=0; i<3; i++)
    {
        if(board[i][0] != player::NONE && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];

        if(board[0][i] != player::NONE && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }

    if(board[0][0] != player::NONE && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];

    if(board[0][2] != player::NONE && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[2][0];

    return player::NONE;
}

bool game::isBoardFull()
{
    return movesCounter == 9;
}