#include <gtest/gtest.h>
#include "AIPlayer.h"
#include "Board.h"

//
// Test 1: Makes a valid move on an empty board
//
TEST(AIPlayerTest, MakesValidMoveOnEmptyBoard) {
    Board board;           // All cells empty
    AIPlayer ai;

    std::pair<int, int> move = ai.getBestMove(board, 'X', 'O');

    // Check that move is within valid range
    EXPECT_GE(move.first, 0);
    EXPECT_LE(move.first, 2);
    EXPECT_GE(move.second, 0);
    EXPECT_LE(move.second, 2);
    EXPECT_TRUE(board.isCellEmpty(move.first, move.second));
}

//
// Test 2: Picks the winning move if available
//
TEST(AIPlayerTest, PicksWinningMove) {
    Board board;
    AIPlayer ai;

    // AI has two in a row, can win by playing at (0, 2)
    board.makeMove(0, 0, 'X');
    board.makeMove(0, 1, 'X');
    board.makeMove(1, 0, 'O'); // Opponent move
    board.makeMove(1, 1, 'O'); // Opponent move

    std::pair<int, int> move = ai.getBestMove(board, 'X', 'O');

    EXPECT_EQ(move, std::make_pair(0, 2)); // Expect winning move
}

TEST(AIPlayerTest, BlocksOpponentWinningMove) {
    Board board;
    AIPlayer ai;

    // Opponent has two in a row, must block at (2, 2)
    board.makeMove(2, 0, 'O');
    board.makeMove(2, 1, 'O');
    board.makeMove(0, 0, 'X'); // AI move
    board.makeMove(1, 1, 'X'); // AI move

    std::pair<int, int> move = ai.getBestMove(board, 'X', 'O');

    EXPECT_EQ(move, std::make_pair(2, 2)); // Must block here
}
