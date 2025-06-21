#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <utility>
#include <vector>
#include <climits>
#include <algorithm>
#include <random>
#include "Board.h"

class AIPlayer
{
public:
    enum Difficulty {
        EASY,
        MEDIUM,
        HARD
    };

    AIPlayer(Difficulty diff = HARD);
    ~AIPlayer() = default;

    std::pair<int, int> getMove(Board* board);
    void setDifficulty(Difficulty diff) { difficulty = diff; }

private:
    Difficulty difficulty;
    std::mt19937 rng;
    int minimax(Board board, int depth, bool isMaximizing, int alpha, int beta);
    int evaluateBoard(const Board& board, int depth);
    std::pair<int, int> getBestMove(Board* board);
    std::pair<int, int> getRandomMove(Board* board);
    std::pair<int, int> getMediumMove(Board* board);
    std::pair<int, int> getSuboptimalMove(Board* board);
    bool shouldMakeMistake();
    bool shouldMissBlock();
    bool shouldMakeRandomMistake();
};

#endif // AIPLAYER_H
