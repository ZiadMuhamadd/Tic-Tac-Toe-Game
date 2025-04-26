#include"../../include/AI_opponent.h"

void HvsH()
{
    game x;
    cell move;

    while(!x.isBoardFull())
    {
        cout<<"X's turn\n";
        cin>>move.row>>move.col;
        x.makeMove(move.row, move.col);
        x.printBoard();

        if(x.checkWinner())
        {
            cout<<"The winner is "<<(char)x.getWinner()<<endl;
            break;
        }
        
        cout<<"O's turn\n";
        cin>>move.row>>move.col;
        x.makeMove(move.row, move.col);
        x.printBoard();

        if(x.checkWinner())
        {
            cout<<"The winner is "<<(char)x.getWinner()<<endl;
            break;
        }
    }

    if(x.isBoardFull())
    {
        cout<<"It is a draw";
    }
}

void HvsAI()
{
    game x;
    AIopponent y(player::O);
    cell move;

    while(!x.isBoardFull())
    {
        cout<<"X's turn\n";
        cin>>move.row>>move.col;
        x.makeMove(move.row, move.col);
        x.printBoard();

        if(x.checkWinner())
        {
            cout<<"The winner is "<<(char)x.getWinner()<<endl;
            break;
        }

        move = y.findBestMove(x);
        x.makeMove(move.row, move.col);
        x.printBoard();

        if(x.checkWinner())
        {
            cout<<"The winner is "<<(char)x.getWinner()<<endl;
            break;
        }
    }

    if(x.isBoardFull())
    {
        cout<<"It is a draw\n";
    }
}