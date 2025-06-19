#include "AIPlayer.h"
#include <algorithm>
#include <climits>

AIPlayer::AIPlayer() : rng(std::random_device{}())
{
}

std::pair<int, int> AIPlayer::getMove(Board* board)
{
    // Use minimax for strategic play, with some randomness
    if (rng() % 10 < 7) { // 70% strategic, 30% random
        return getBestMove(board);
    } else {
        return getRandomMove(board);
    }
}

std::pair<int, int> AIPlayer::getBestMove(Board* board)
{
    int bestScore = INT_MIN;
    std::pair<int, int> bestMove = {-1, -1};

    auto availableMoves = board->getAvailableMoves();

    for (const auto& move : availableMoves) {
        // Make temporary move
        Board tempBoard = *board;
        tempBoard.makeMove(move.first, move.second, 'O');

        int score = minimax(&tempBoard, 0, false);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove.first != -1 ? bestMove : getRandomMove(board);
}

std::pair<int, int> AIPlayer::getRandomMove(Board* board)
{
    auto availableMoves = board->getAvailableMoves();
    if (!availableMoves.empty()) {
        std::uniform_int_distribution<> dist(0, availableMoves.size() - 1);
        return availableMoves[dist(rng)];
    }
    return {-1, -1};
}

int AIPlayer::minimax(Board* board, int depth, bool isMaximizing)
{
    if (board->checkWin('O')) return 10 - depth;
    if (board->checkWin('X')) return depth - 10;
    if (board->checkTie()) return 0;

    if (depth >= 6) return 0; // Limit depth for performance

    if (isMaximizing) {
        int bestScore = INT_MIN;
        auto availableMoves = board->getAvailableMoves();

        for (const auto& move : availableMoves) {
            Board tempBoard = *board;
            tempBoard.makeMove(move.first, move.second, 'O');
            int score = minimax(&tempBoard, depth + 1, false);
            bestScore = std::max(score, bestScore);
        }
        return bestScore;
    } else {
        int bestScore = INT_MAX;
        auto availableMoves = board->getAvailableMoves();

        for (const auto& move : availableMoves) {
            Board tempBoard = *board;
            tempBoard.makeMove(move.first, move.second, 'X');
            int score = minimax(&tempBoard, depth + 1, true);
            bestScore = std::min(score, bestScore);
        }
        return bestScore;
    }
}
