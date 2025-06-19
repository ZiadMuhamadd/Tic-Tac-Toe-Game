#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <utility>
#include <vector>
#include <random>
#include "Board.h"

class AIPlayer
{
public:
    AIPlayer();
    std::pair<int, int> getMove(Board* board);

private:
    std::mt19937 rng;
    int minimax(Board* board, int depth, bool isMaximizing);
    std::pair<int, int> getBestMove(Board* board);
    std::pair<int, int> getRandomMove(Board* board);
};

#endif // AIPLAYER_H
