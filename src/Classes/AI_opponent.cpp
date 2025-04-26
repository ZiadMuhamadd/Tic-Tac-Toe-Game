#include"../../include/AI_opponent.h"

AIopponent::AIopponent(player aiSymbol)
{
    AIplayer = aiSymbol;
    humanplayer = (aiSymbol == player::X) ? player::O : player::X;
}

int AIopponent::minimax(game gameCopy, bool isMax, int alpha, int beta)
{
    if(gameCopy.checkWinner())
    {
        if(gameCopy.getWinner() == AIplayer) return +1;
        else if(gameCopy.getWinner() == humanplayer) return -1;
    }

    if(gameCopy.isBoardFull()) return 0;

    if(isMax)
    {
        int best = negativeInfinity;

        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                if(gameCopy.getCell(i, j) == player::NONE)
                {
                    gameCopy.makeMove(i, j);
                    int score = minimax(gameCopy, false, alpha, beta);
                    gameCopy.undoMove(i, j);
                    best = max(best, score);
                    alpha = max(best, alpha);

                    if(beta <= alpha) return best;
                }
            }
        }
        return best;
    }
    else
    {
        int best = positiveInfinity;

        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                if(gameCopy.getCell(i, j) == player::NONE)
                {
                    gameCopy.makeMove(i, j);
                    int score = minimax(gameCopy, true, alpha, beta);
                    gameCopy.undoMove(i, j);
                    best = min(best, score);
                    beta = min(best, alpha);

                    if(beta <= alpha) return best;
                }
            }
        }
        return best;
    }
}

cell AIopponent::findBestMove(game gameCopy)
{
    int bestScore = negativeInfinity;
    cell bestmove{-1 , -1};

    
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(gameCopy.getCell(i, j) == player::NONE)
            {
                gameCopy.makeMove(i, j);
                int score = minimax(gameCopy, false, negativeInfinity, positiveInfinity);
                gameCopy.undoMove(i, j);

                if(score > bestScore)
                {
                    bestScore = score;
                    bestmove.row = i; 
                    bestmove.col = j;
                }
            }
        }
    }
    return bestmove;
}