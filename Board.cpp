#include "Board.h"
#include <iostream>

Board::Board() {
    for (auto& row : board)
        for (char& cell : row)
            cell = ' ';
}

bool Board::makeMove(int row, int col, char player) {
    if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ') {
        board[row][col] = player;
        return true;
    }
    return false;
}

bool Board::checkWin(char player) const {
    for (int i = 0; i < 3; ++i)
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player))
            return true;
    return (board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
           (board[0][2] == player && board[1][1] == player && board[2][0] == player);
}

bool Board::checkTie() const {
    for (auto& row : board)
        for (char cell : row)
            if (cell == ' ')
                return false;
    return true;
}

void Board::displayBoard() const {
    for (int i = 0; i < 3; ++i) {
        std::cout << " ";
        for (int j = 0; j < 3; ++j) {
            std::cout << board[i][j];
            if (j < 2) std::cout << " | ";
        }
        if (i < 2) std::cout << "\n-----------\n";
    }
    std::cout << std::endl;
}

std::vector<std::pair<int, int>> Board::getAvailableMoves() const {
    std::vector<std::pair<int, int>> moves;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i][j] == ' ')
                moves.emplace_back(i, j);
    return moves;
}

char Board::getCell(int row, int col) const {
    return board[row][col];
}

void Board::setCell(int row, int col, char value) {
    board[row][col] = value;
}
