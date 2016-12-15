#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <SDL.h>
#include "drawing.h"
#include "logic.h"

const int BLOCK_SIZE = 24;
const int BOARD_WIDTH = 40;
const int BOARD_HEIGHT = 30;
const int SCREEN_WIDTH = BLOCK_SIZE * BOARD_WIDTH;
const int SCREEN_HEIGHT = BLOCK_SIZE * BOARD_HEIGHT;

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
    // Setup the destination rectangle to be at the position we want
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    // Query the texture to get its width and height to use
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

void drawSnake(SDL_Texture *segment, SDL_Renderer *renderer, std::vector<std::vector<int>>& board) {
    int x, y, h, w, i, j;
    SDL_QueryTexture(segment, NULL, NULL, &h, &w);
    for (i = 0; i < BOARD_HEIGHT; ++i) {
        for (j = 0; j < BOARD_WIDTH; ++j) {
            x = j * w;
            y = i * h;

            if (board[j][i] > 0) {
                renderTexture(segment, renderer, x, y);
                std::cout << i << " " << j << " " << board[j][i] << std::endl;
            }
        }
    }
}

void drawFood(SDL_Texture *food, SDL_Renderer *renderer, std::vector<std::vector<int>>& board) {
    int foodPresent = 0;
    int x, y, h, w, i, j;
    SDL_QueryTexture(food, NULL, NULL, &h, &w);

    for (i = 0; i < BOARD_HEIGHT; ++i) {
        for (j = 0; j < BOARD_WIDTH; ++j) {
            if (board[j][i] == -1) {
                foodPresent = 1;
                x = j * w;
                y = i * h;
            }
        }
    }

    if (!foodPresent) {
        generateFood(&i, &j, board);
        x = j * w;
        y = i * h;
    }

    renderTexture(food, renderer, x, y);
}

void drawWall(SDL_Texture *wall, SDL_Renderer *renderer, std::vector<std::vector<int>>& board){
    int x, y, h, w, i, j;
    SDL_QueryTexture(wall, NULL, NULL, &h, &w);
    for (i = 0; i < BOARD_HEIGHT; ++i) {
        for (j = 0; j < BOARD_WIDTH; ++j) {
            x = j * w;
            y = i * h;

            if (board[j][i] == -2)
                renderTexture(wall, renderer, x, y);
        }
    }
}

void flashSnake(SDL_Texture *segment_a, SDL_Texture *segment_b, SDL_Renderer *renderer, std::vector<std::vector<int>>& board, int n) {
    for (int i = 0; i < n; ++i) {
        drawSnake(segment_b, renderer, board);
        SDL_Delay(500);
        SDL_RenderPresent(renderer);

        drawSnake(segment_a, renderer, board);
        SDL_Delay(500);
        SDL_RenderPresent(renderer);
    }
}
