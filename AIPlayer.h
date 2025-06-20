#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Board.h"
#include <vector>

struct GameTreeNode {
    Board state;
    int score;
    std::pair<int, int> move;
    std::vector<GameTreeNode*> children;
    GameTreeNode(Board b) : state(b), score(0), move({ -1, -1 }) {}
    ~GameTreeNode() {
        for (auto child : children)
            delete child;
    }
};

class AIPlayer {
public:
    std::pair<int, int> getBestMove(Board board, char aiPlayer, char humanPlayer);

private:
    int minimax(GameTreeNode* node, char currentPlayer, char aiPlayer, char humanPlayer, int alpha, int beta);
};

#endif
