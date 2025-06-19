#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <utility>
#include <vector>
#include <climits>
#include <algorithm>
#include "Board.h"

class AIPlayer
{
public:
    AIPlayer();
    ~AIPlayer() = default;

    std::pair<int, int> getMove(Board* board);

private:
    int minimax(Board board, int depth, bool isMaximizing, int alpha, int beta);
    int evaluateBoard(const Board& board);
    std::pair<int, int> getBestMove(Board* board);
};

#endif // AIPLAYER_H
