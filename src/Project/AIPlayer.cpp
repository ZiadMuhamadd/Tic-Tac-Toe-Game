#include "AIPlayer.h"

AIPlayer::AIPlayer()
{
}

std::pair<int, int> AIPlayer::getMove(Board* board)
{
    return getBestMove(board);
}

std::pair<int, int> AIPlayer::getBestMove(Board* board)
{
    int bestScore = INT_MIN;
    std::pair<int, int> bestMove = {-1, -1};

    auto availableMoves = board->getAvailableMoves();

    // If no moves available, return invalid move
    if (availableMoves.empty()) {
        return bestMove;
    }

    for (const auto& move : availableMoves) {
        // Make a copy of the board to test the move
        Board testBoard = *board;

        if (testBoard.makeMove(move.first, move.second, 'O')) {
            // Calculate score using minimax with alpha-beta pruning
            int score = minimax(testBoard, 0, false, INT_MIN, INT_MAX);

            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }
    }

    return bestMove;
}

int AIPlayer::minimax(Board board, int depth, bool isMaximizing, int alpha, int beta)
{
    // Check terminal states first
    int boardScore = evaluateBoard(board);

    // If game is over or we've reached maximum depth, return evaluation
    if (boardScore == 10 || boardScore == -10 || board.checkTie() || depth >= 9) {
        return boardScore - depth; // Prefer quicker wins/slower losses
    }

    if (isMaximizing) {
        // AI's turn (maximizing player)
        int maxEval = INT_MIN;
        auto moves = board.getAvailableMoves();

        for (const auto& move : moves) {
            Board newBoard = board;
            if (newBoard.makeMove(move.first, move.second, 'O')) {
                int eval = minimax(newBoard, depth + 1, false, alpha, beta);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);

                // Alpha-beta pruning
                if (beta <= alpha) {
                    break;
                }
            }
        }
        return maxEval;
    } else {
        // Human's turn (minimizing player)
        int minEval = INT_MAX;
        auto moves = board.getAvailableMoves();

        for (const auto& move : moves) {
            Board newBoard = board;
            if (newBoard.makeMove(move.first, move.second, 'X')) {
                int eval = minimax(newBoard, depth + 1, true, alpha, beta);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);

                // Alpha-beta pruning
                if (beta <= alpha) {
                    break;
                }
            }
        }
        return minEval;
    }
}

int AIPlayer::evaluateBoard(const Board& board)
{
    // Terminal state evaluations
    if (board.checkWin('O')) {
        return 10; // AI wins
    }
    if (board.checkWin('X')) {
        return -10; // Human wins
    }
    if (board.checkTie()) {
        return 0; // Tie
    }

    // For non-terminal states, return neutral
    return 0;
}
