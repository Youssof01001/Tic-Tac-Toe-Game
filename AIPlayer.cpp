#include "AIPlayer.h"
#include <limits>

std::pair<int, int> AIPlayer::getBestMove(Board board, char aiPlayer, char humanPlayer) {
    GameTreeNode* root = new GameTreeNode(board);
    minimax(root, aiPlayer, aiPlayer, humanPlayer, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    int bestScore = std::numeric_limits<int>::min();
    std::pair<int, int> bestMove = { -1, -1 };
    for (auto child : root->children) {
        if (child->score > bestScore) {
            bestScore = child->score;
            bestMove = child->move;
        }
    }

    delete root;
    return bestMove;
}

int AIPlayer::minimax(GameTreeNode* node, char currentPlayer, char aiPlayer, char humanPlayer, int alpha, int beta) {
    if (node->state.checkWin(aiPlayer)) return node->score = 10;
    if (node->state.checkWin(humanPlayer)) return node->score = -10;
    if (node->state.checkTie()) return node->score = 0;

    int bestScore = (currentPlayer == aiPlayer) ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    for (auto move : node->state.getAvailableMoves()) {
        Board nextState = node->state;
        nextState.setCell(move.first, move.second, currentPlayer);

        GameTreeNode* child = new GameTreeNode(nextState);
        child->move = move;
        node->children.push_back(child);

        int score = minimax(child, (currentPlayer == aiPlayer) ? humanPlayer : aiPlayer, aiPlayer, humanPlayer, alpha, beta);

        if (currentPlayer == aiPlayer) {
            bestScore = std::max(bestScore, score);
            alpha = std::max(alpha, score);
        }
        else {
            bestScore = std::min(bestScore, score);
            beta = std::min(beta, score);
        }

        if (beta <= alpha)
            break;
    }
    return node->score = bestScore;
}
