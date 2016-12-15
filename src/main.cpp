#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <SDL.h>
#include "res_path.h"
#include "cleanup.h"
#include "drawing.h"
#include "logic.h"

void logSDLError(std::ostream &os, const std::string &msg) {
    os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren) {
    // Initialize to nullptr to avoid danglingpointer issues
    SDL_Texture *texture = nullptr;
    // Load the image
    SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
    // If the loading went ok, convert to texture and return the texture
    if (loadedImage != nullptr) {
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        SDL_FreeSurface(loadedImage);
        // Make sure everything went ok too
        if (texture == nullptr) {
            logSDLError(std::cout, "CreateTextureFromSurface");
        }
    }
    else {
        logSDLError(std::cout, "LoadBMP");
    }
    return texture;
}

int main(int, char**) {
    /* Start RNG for food generation */
    srand(time(NULL));

    /* Starting SDL */
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Snake!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        logSDLError(std::cout, "CreateWindow");
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        logSDLError(std::cout, "CreateRenderer");
        cleanup(window);
        SDL_Quit();
        return 1;
    }

    const std::string resPath = getResourcePath();
    SDL_Texture *segment = loadTexture(resPath + "segment.bmp", renderer);
    SDL_Texture *food = loadTexture(resPath + "food.bmp", renderer);
    SDL_Texture *wall = loadTexture(resPath + "segment.bmp", renderer);
    if (segment == nullptr || food == nullptr || wall == nullptr) {
        cleanup(segment, renderer, window);
        SDL_Quit();
        return 1;
    }

    //int board[BOARD_WIDTH][BOARD_HEIGHT] = { 0 };
    std::vector<std::vector<int>> board (BOARD_WIDTH, std::vector<int> (BOARD_HEIGHT, 0));
    generateWall(board);

    SDL_RenderClear(renderer);

    bool quit = false;
    SDL_Event e;
    int direction, length;
    int posX, posY;

    posX = posY = 1;
    length = 3;
    direction = 1;


    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN){
    			switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (direction != 2)
                            direction = 0;
                        break;
                    case SDLK_RIGHT:
                        if (direction != 3)
                        direction = 1;
                        break;
                    case SDLK_DOWN:
                        if (direction != 0)
                            direction = 2;
                        break;
                    case SDLK_LEFT:
                        if (direction != 1)
                            direction = 3;
                        break;
                    default:
                        break;
                }
    		}
        }

        if (checkIfLost(posX, posY, direction, board)) {
            std::cout << "Przegrałeś!" << std::endl;
            std::cout << "Twój wynik to: " << length - 3 << std::endl;
            std::cout << "Ostatnia pozycja: " << board[posX][posY] << std::endl;

            flashSnake(segment, food, renderer, board, 2);

            quit = true;
            continue;
        }

        shortenSnake(board);
        updatePosition(&posX, &posY, direction);

        if (board[posX][posY] == -1) {
            length += 1;
        }

        board[posX][posY] = length;

        SDL_RenderClear(renderer);
        drawWall(wall, renderer, board);
        drawFood(food, renderer, board);
        drawSnake(segment, renderer, board);
        SDL_RenderPresent(renderer);


        SDL_Delay(70);
    }

    cleanup(wall, food, segment, renderer, window);
    SDL_Quit();

    return 0;
}
