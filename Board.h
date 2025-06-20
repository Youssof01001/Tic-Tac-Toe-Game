#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board {
public:
    Board();
    bool makeMove(int row, int col, char player);
    bool checkWin(char player) const;
    bool checkTie() const;
    void displayBoard() const;
    std::vector<std::pair<int, int>> getAvailableMoves() const;
    char getCell(int row, int col) const;
    void setCell(int row, int col, char value);

private:
    char board[3][3];
};

#endif
