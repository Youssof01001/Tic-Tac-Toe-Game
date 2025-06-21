#include <gtest/gtest.h>
#include "../includes/AIPlayer.h"
#include "../includes/Board.h"

// Helper to set up a board state
void setBoardState(Board& board, const std::vector<std::tuple<int, int, char>>& moves) {
    for (const auto& [row, col, player] : moves) {
        board.makeMove(row, col, player);
    }
}

// HARD difficulty always plays optimally
TEST(AIPlayerTest, HardMode_ChoosesWinningMove) {
    Board board;
    setBoardState(board, {
                             {0, 0, 'O'}, {0, 1, 'O'}  // O can win by placing at (0,2)
                         });

    AIPlayer ai(AIPlayer::HARD);
    auto move = ai.getMove(&board);

    EXPECT_EQ(move, std::make_pair(0, 2));
}

TEST(AIPlayerTest, HardMode_BlocksOpponentWin) {
    Board board;
    setBoardState(board, {
                             {1, 0, 'X'}, {1, 1, 'X'}  // X could win at (1,2)
                         });

    AIPlayer ai(AIPlayer::HARD);
    auto move = ai.getMove(&board);

    EXPECT_EQ(move, std::make_pair(1, 2));
}

// MEDIUM mode sometimes blocks and sometimes makes mistakes
TEST(AIPlayerTest, MediumMode_WillBlockOrNot) {
    Board board;
    setBoardState(board, {
                             {2, 0, 'X'}, {2, 1, 'X'}
                         });

    AIPlayer ai(AIPlayer::MEDIUM);

    int blockCount = 0;
    for (int i = 0; i < 100; ++i) {
        auto move = ai.getMove(&board);
        if (move == std::make_pair(2, 2)) {
            blockCount++;
        }
    }

    // Should block most of the time (about 90%)
    EXPECT_GT(blockCount, 80);
}

// EASY mode sometimes picks randomly, sometimes smart
TEST(AIPlayerTest, EasyMode_MakesRandomOrBestMoves) {
    Board board;
    setBoardState(board, {
                             {0, 0, 'X'}, {1, 1, 'O'}, {0, 1, 'X'}
                         });

    AIPlayer ai(AIPlayer::EASY);

    std::set<std::pair<int, int>> observedMoves;
    for (int i = 0; i < 50; ++i) {
        auto move = ai.getMove(&board);
        observedMoves.insert(move);
    }

    // Should show variety (random and smart moves)
    EXPECT_GT(observedMoves.size(), 1);
}

TEST(AIPlayerTest, AIHandlesFullBoard) {
    Board board;
    setBoardState(board, {
                             {0, 0, 'X'}, {0, 1, 'O'}, {0, 2, 'X'},
                             {1, 0, 'X'}, {1, 1, 'O'}, {1, 2, 'O'},
                             {2, 0, 'O'}, {2, 1, 'X'}, {2, 2, 'X'}
                         });

    AIPlayer ai(AIPlayer::HARD);
    auto move = ai.getMove(&board);
    EXPECT_EQ(move, std::make_pair(-1, -1));  // No move possible
}
