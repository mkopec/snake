extern const int BLOCK_SIZE;
extern const int BOARD_WIDTH;
extern const int BOARD_HEIGHT;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void drawSnake(SDL_Texture *segment, SDL_Renderer *renderer, std::vector<std::vector<int>>& board);
void drawFood(SDL_Texture *food, SDL_Renderer *renderer, std::vector<std::vector<int>>& board);
void drawWall(SDL_Texture *wall, SDL_Renderer *renderer, std::vector<std::vector<int>>& board);
void drawBackground(SDL_Texture *field, SDL_Renderer *renderer, std::vector<std::vector<int>>& board);
void flashSnake(SDL_Texture *segment_a, SDL_Texture *segment_b, SDL_Renderer *renderer, std::vector<std::vector<int>>& board, int n);
