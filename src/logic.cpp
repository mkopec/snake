#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <SDL.h>
#include <iomanip>
#include "logic.h"
#include "drawing.h"

void shortenSnake(std::vector<std::vector<int>>& board) {
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            if (board[j][i] > 0)
                board[j][i] -= 1;
        }
    }
}

void generateFood(int * i, int * j, std::vector<std::vector<int>>& board) {
    int posX, posY;
    do {
        posX = rand() % BOARD_WIDTH;
        posY = rand() % BOARD_HEIGHT;
    } while (board[posX][posY] != 0);
    board[posX][posY] = -1;
    *i = posY;
    *j = posX;
}

void generateWall(std::vector<std::vector<int>>& board) {
    int i;
    for (i = 0; i < BOARD_HEIGHT; i++) {
        board[0][i] = -2;
        board[BOARD_WIDTH - 1][i] = -2;
    }

    for (i = 0; i < BOARD_WIDTH; ++i) {
        board[i][0] = -2;
        board[i][BOARD_HEIGHT-1] = -2;
    }
}

bool checkIfLost(int x, int y, int direction, std::vector<std::vector<int>>& board) {
    updatePosition(&x, &y, direction);

    if (board[x][y] > 0 || board[x][y] == -2) {
        return true;
    }
    return false;
}

void updatePosition(int * posX, int * posY, int direction) {
    switch (direction) {
        case 0:
            *posY -= 1;
            if (*posY < 0) {
                *posY = BOARD_HEIGHT - 1;
            }
            break;
        case 1:
            *posX = (*posX+1)%BOARD_WIDTH;
            break;
        case 2:
            *posY = (*posY+1)%BOARD_HEIGHT;
            break;
        case 3:
            *posX -= 1;
            if (*posX < 0) {
                *posX = BOARD_WIDTH - 1;
            }
            break;
        default:
            break;
    }
}
