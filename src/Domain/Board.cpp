#include "../../includes/Board.h"

Board::Board()
{
    reset();
}

void Board::reset()
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = ' ';
        }
    }
}

bool Board::makeMove(int row, int col, char player)
{
    if (isValidMove(row, col)) {
        board[row][col] = player;
        return true;
    }
    return false;
}

bool Board::isValidMove(int row, int col) const
{
    return (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ');
}

bool Board::checkWin(char player) const
{
    // Check rows
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
            return true;
        }
    }

    // Check columns
    for (int j = 0; j < 3; ++j) {
        if (board[0][j] == player && board[1][j] == player && board[2][j] == player) {
            return true;
        }
    }

    // Check diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
        return true;
    }
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
        return true;
    }

    return false;
}

bool Board::checkTie() const
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') {
                return false;
            }
        }
    }
    return !checkWin('X') && !checkWin('O');
}

std::vector<std::pair<int, int>> Board::getAvailableMoves() const
{
    std::vector<std::pair<int, int>> moves;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') {
                moves.push_back(std::make_pair(i, j));
            }
        }
    }
    return moves;
}

char Board::getCell(int row, int col) const
{
    if (row >= 0 && row < 3 && col >= 0 && col < 3) {
        return board[row][col];
    }
    return ' ';
}
