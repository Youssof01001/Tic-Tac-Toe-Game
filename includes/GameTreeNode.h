#ifndef GAMETREENODE_H
#define GAMETREENODE_H

#include "Board.h"
#include <vector>

struct GameTreeNode {
    Board board;
    std::pair<int, int> move;
    int score;
    std::vector<GameTreeNode*> children;

    GameTreeNode(Board b, std::pair<int, int> m) : board(b), move(m), score(0) {}
    ~GameTreeNode() {
        for (auto child : children) delete child;
    }
};

#endif

