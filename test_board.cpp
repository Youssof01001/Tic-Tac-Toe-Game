#include <gtest/gtest.h>
#include "Board.h"

TEST(BoardTest, InitiallyEmptyBoard) {
    Board board;
    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col)
            EXPECT_EQ(board.getCell(row, col), ' ');
}

TEST(BoardTest, MakeMoveSuccess) {
    Board board;
    EXPECT_TRUE(board.makeMove(1, 1, 'X'));
    EXPECT_EQ(board.getCell(1, 1), 'X');
}

TEST(BoardTest, MakeMoveInvalidIfOccupied) {
    Board board;
    board.makeMove(0, 0, 'O');
    EXPECT_FALSE(board.makeMove(0, 0, 'X'));
    EXPECT_EQ(board.getCell(0, 0), 'O');
}

TEST(BoardTest, CheckWinByRow) {
    Board board;
    board.makeMove(0, 0, 'X');
    board.makeMove(0, 1, 'X');
    board.makeMove(0, 2, 'X');
    EXPECT_TRUE(board.checkWin('X'));
}

TEST(BoardTest, CheckWinByColumn) {
    Board board;
    board.makeMove(0, 1, 'O');
    board.makeMove(1, 1, 'O');
    board.makeMove(2, 1, 'O');
    EXPECT_TRUE(board.checkWin('O'));
}

TEST(BoardTest, CheckWinByDiagonal) {
    Board board;
    board.makeMove(0, 0, 'X');
    board.makeMove(1, 1, 'X');
    board.makeMove(2, 2, 'X');
    EXPECT_TRUE(board.checkWin('X'));
}

TEST(BoardTest, CheckTieCondition) {
    Board board;
    board.makeMove(0, 0, 'X');
    board.makeMove(0, 1, 'O');
    board.makeMove(0, 2, 'X');
    board.makeMove(1, 0, 'X');
    board.makeMove(1, 1, 'O');
    board.makeMove(1, 2, 'O');
    board.makeMove(2, 0, 'O');
    board.makeMove(2, 1, 'X');
    board.makeMove(2, 2, 'X');
    EXPECT_TRUE(board.checkTie());
    EXPECT_FALSE(board.checkWin('X'));
    EXPECT_FALSE(board.checkWin('O'));
}

TEST(BoardTest, ResetBoardClearsCells) {
    Board board;
    board.makeMove(0, 0, 'X');
    board.makeMove(1, 1, 'O');
    board.reset();
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            EXPECT_EQ(board.getCell(i, j), ' ');
}
