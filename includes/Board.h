#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <utility>

class Board
{
public:
    Board();

    bool makeMove(int row, int col, char player);
    bool checkWin(char player) const;
    bool checkTie() const;
    std::vector<std::pair<int, int>> getAvailableMoves() const;
    void reset();
    char getCell(int row, int col) const;

private:
    char board[3][3];
    bool isValidMove(int row, int col) const;
};

#endif // BOARD_H
