#ifndef _AI_OPPONENT_H
#define _AI_OPPONENT_H

#include"GameBoard.h"

#define negativeInfinity -1000
#define positiveInfinity 1000

class AIopponent
{
    private:

    player AIplayer;
    player humanplayer;

    int minimax(game gameCopy, bool isMax, int alpha, int beta);

    public:

    AIopponent(player aiSymbol);
    cell findBestMove(game gameCopy);
};

#endif