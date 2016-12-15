void shortenSnake(std::vector<std::vector<int>>& board);
void generateFood(int * i, int * j, std::vector<std::vector<int>>& board);
void generateWall(std::vector<std::vector<int>>& board);
void updatePosition(int * posX, int * posY, int direction);
bool checkIfLost(int x, int y, int direction, std::vector<std::vector<int>>& board);
