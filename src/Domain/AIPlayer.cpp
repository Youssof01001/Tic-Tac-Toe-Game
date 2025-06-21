#include "../../includes/AIPlayer.h"

AIPlayer::AIPlayer(Difficulty diff) : difficulty(diff), rng(std::random_device{}())
{
}

std::pair<int, int> AIPlayer::getMove(Board* board)
{
    switch (difficulty) {
    case EASY:
        // Easy mode: 60% strategic, 40% random
        if (rng() % 10 < 6) {
            return getBestMove(board);
        } else {
            return getRandomMove(board);
        }

    case MEDIUM:
        // Medium mode: Smart but makes occasional mistakes
        return getMediumMove(board);

    case HARD:
    default:
        // Hard mode: always best move
        return getBestMove(board);
    }
}

std::pair<int, int> AIPlayer::getMediumMove(Board* board)
{
    // Adjusted Medium AI strategy for excellent players:
    // 1. 90% chance to block immediate wins (10% miss critical blocks!)
    // 2. Always take immediate wins (can't miss obvious wins)
    // 3. 70% of the time play optimally (reduced from 85%)
    // 4. 30% of the time make suboptimal moves (increased from 15%)
    // 5. Occasionally make "human-like" mistakes

    auto availableMoves = board->getAvailableMoves();
    if (availableMoves.empty()) {
        return {-1, -1};
    }

    // Always check for immediate win first
    for (const auto& move : availableMoves) {
        Board testBoard = *board;
        if (testBoard.makeMove(move.first, move.second, 'O')) {
            if (testBoard.checkWin('O')) {
                return move; // Always take the win
            }
        }
    }

    // 90% chance to block opponent's win (10% chance to miss!)
    if (!shouldMissBlock()) {
        for (const auto& move : availableMoves) {
            Board testBoard = *board;
            if (testBoard.makeMove(move.first, move.second, 'X')) {
                if (testBoard.checkWin('X')) {
                    return move; // Block the win
                }
            }
        }
    }

    // 70% optimal play, 30% suboptimal (more mistakes than before)
    if (!shouldMakeMistake()) {
        return getBestMove(board);
    } else {
        // Make more varied mistakes - choose from top 4 moves or even random sometimes
        if (shouldMakeRandomMistake()) {
            return getRandomMove(board); // 5% chance for completely random move
        } else {
            return getSuboptimalMove(board); // Choose from 2nd-4th best moves
        }
    }
}

bool AIPlayer::shouldMakeMistake()
{
    // 30% chance to make a suboptimal move (increased from 15%)
    return (rng() % 100) < 30;
}

bool AIPlayer::shouldMissBlock()
{
    // 10% chance to miss blocking a winning move (new vulnerability!)
    return (rng() % 100) < 10;
}

bool AIPlayer::shouldMakeRandomMistake()
{
    // 5% chance to make a completely random move
    return (rng() % 100) < 5;
}

std::pair<int, int> AIPlayer::getSuboptimalMove(Board* board)
{
    auto availableMoves = board->getAvailableMoves();
    std::vector<std::pair<std::pair<int, int>, int>> moveScores;

    for (const auto& move : availableMoves) {
        Board testBoard = *board;
        if (testBoard.makeMove(move.first, move.second, 'O')) {
            int score = minimax(testBoard, 0, false, INT_MIN, INT_MAX);
            moveScores.push_back({move, score});
        }
    }

    // Sort moves by score (best first)
    std::sort(moveScores.begin(), moveScores.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    // Choose from 2nd to 4th best moves (skip the best move)
    int startIndex = 1; // Skip best move
    int endIndex = std::min(4, static_cast<int>(moveScores.size()));

    if (startIndex >= endIndex) {
        // Fallback to random if not enough moves
        return getRandomMove(board);
    }

    std::uniform_int_distribution<> dist(startIndex, endIndex - 1);
    return moveScores[dist(rng)].first;
}




std::pair<int, int> AIPlayer::getBestMove(Board* board)
{
    int bestScore = INT_MIN;
    std::pair<int, int> bestMove = {-1, -1};

    auto availableMoves = board->getAvailableMoves();

    if (availableMoves.empty()) {
        return bestMove;
    }

    for (const auto& move : availableMoves) {
        Board testBoard = *board;

        if (testBoard.makeMove(move.first, move.second, 'O')) {
            int score = minimax(testBoard, 0, false, INT_MIN, INT_MAX);

            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }
    }

    return bestMove;
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

int AIPlayer::minimax(Board board, int depth, bool isMaximizing, int alpha, int beta)
{
    int boardScore = evaluateBoard(board, depth);

    if (boardScore == 10 || boardScore == -10 || board.checkTie()) {
        return boardScore - depth;
    }

    // Adjust depth based on difficulty
    int maxDepth;
    switch (difficulty) {
    case EASY:
        maxDepth = 4;
        break;
    case MEDIUM:
        maxDepth = 5;
        break;
    case HARD:
    default:
        maxDepth = 9;
        break;
    }

    if (depth >= maxDepth) {
        return boardScore;
    }

    if (isMaximizing) {
        int maxEval = INT_MIN;
        auto moves = board.getAvailableMoves();

        for (const auto& move : moves) {
            Board newBoard = board;
            if (newBoard.makeMove(move.first, move.second, 'O')) {
                int eval = minimax(newBoard, depth + 1, false, alpha, beta);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);

                if (beta <= alpha) {
                    break;
                }
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        auto moves = board.getAvailableMoves();

        for (const auto& move : moves) {
            Board newBoard = board;
            if (newBoard.makeMove(move.first, move.second, 'X')) {
                int eval = minimax(newBoard, depth + 1, true, alpha, beta);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);

                if (beta <= alpha) {
                    break;
                }
            }
        }
        return minEval;
    }
}

int AIPlayer::evaluateBoard(const Board& board, int depth)
{
    if (board.checkWin('O')) {
        return 10 - depth;
    }
    if (board.checkWin('X')) {
        return depth - 10;
    }
    if (board.checkTie()) {
        return 0;
    }

    return 0;
}
