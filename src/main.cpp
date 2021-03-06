#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
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
    SDL_Surface *loadedImage = IMG_Load(file.c_str());
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
    /* Generator liczb losowych do losowania pozycji jedzenia */
    srand(time(NULL));

    /* Uruchamianie SDL */
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }

    /* Tworzenie okna */
    SDL_Window *window = SDL_CreateWindow("Snake!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        logSDLError(std::cout, "CreateWindow");
        SDL_Quit();
        return 1;
    }

    /* Tworzenie renderera dla okna */
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        logSDLError(std::cout, "CreateRenderer");
        cleanup(window);
        SDL_Quit();
        return 1;
    }

    /* Ładowanie tekstur */
    const std::string resPath = getResourcePath();
    SDL_Texture *segment = IMG_LoadTexture(renderer, (resPath + "segment.png").c_str());
    SDL_Texture *food = IMG_LoadTexture(renderer, (resPath + "dead.png").c_str());
    SDL_Texture *wall = IMG_LoadTexture(renderer, (resPath + "segment.png").c_str());
    SDL_Texture *dead = IMG_LoadTexture(renderer, (resPath + "dead.png").c_str());
    SDL_Texture *field = IMG_LoadTexture(renderer, (resPath + "field.png").c_str());
    if (segment == nullptr || food == nullptr || wall == nullptr) {
        cleanup(segment, renderer, window);
        SDL_Quit();
        return 1;
    }

    /* Utworzenie planszy i ścian */
    std::vector<std::vector<int>> board (BOARD_WIDTH, std::vector<int> (BOARD_HEIGHT, 0));
    generateWall(board);

    /* Inicjalizacja zmiennych dla logiki gry */
    bool quit = false;
    int direction, length, newDirection;
    int posX, posY;
    SDL_Event e;

    posX = posY = 1;
    length = 3;
    newDirection = 1;

    /* Główna pętla gry */
    while (!quit) {
        /* Obsługa zdarzeń i klawiatury */
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            /* Kierunki: 0 - Góra, 1 - Prawo, 2 - Dół, 3 - Lewo */
            if (e.type == SDL_KEYDOWN){
    			switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (direction != 2)
                            newDirection = 0;
                        break;
                    case SDLK_RIGHT:
                        if (direction != 3)
                            newDirection = 1;
                        break;
                    case SDLK_DOWN:
                        if (direction != 0)
                            newDirection = 2;
                        break;
                    case SDLK_LEFT:
                        if (direction != 1)
                            newDirection = 3;
                        break;
                    default:
                        break;
                }
    		}
        }

        /* Dzięki temu nie da się zawrócić w miejscu */
        direction = newDirection;

        /* Sprawdzanie kolizji przed zmianą pozycji węża */
        if (checkIfLost(posX, posY, direction, board)) {
            std::cout << "Przegrałeś!" << std::endl;
            std::cout << "Twój wynik to: " << length - 3 << std::endl;

            /* Podwójne świecenie wężem przy przegranej */
            flashSnake(segment, dead, renderer, board, 2);

            quit = true;
            continue;
        }

        /* Przesunięcie i ewentualne predłużenie węża */
        updatePosition(&posX, &posY, direction);
        if (board[posX][posY] == -1)
            length += 1;
        else
            shortenSnake(board);
        board[posX][posY] = length;

        /* Rysowanie planszy i węża */
        SDL_RenderClear(renderer);
        drawBackground(field, renderer, board);
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
