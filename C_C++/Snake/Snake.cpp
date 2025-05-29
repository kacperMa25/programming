#include <random>
#include <vector>
#include <SDL3/SDL.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
bool appleExists = false;
SDL_FRect apple;
enum dir {
    LEFT = SDLK_LEFT,
    RIGHT = SDLK_RIGHT,
    DOWN = SDLK_DOWN,
    UP = SDLK_UP
};

int getRandomNumberWithStep(int minVal, int maxVal, int step);

class Snake{
    public:
    Snake(int nSquares = 1, int speed_ = 10);
    ~Snake();
    void addSquare();
    int size();
    void draw(const int* color, SDL_Renderer*& renderer);
    void generateApple(SDL_Renderer*& gRender);
    std::vector<int> getPos();
    dir getDir();
    bool checkIntersect();

    void up();
    void down();
    void left();
    void right();

    void dirUp();
    void dirDown();
    void dirLeft();
    void dirRight();

    private:
    const float originalPosX = WINDOW_WIDTH / 2;
    const float originalPosY = WINDOW_HEIGHT / 2;
    const float widthOfSqaure = 10;
    const float heightOfSqaure = 10;
    const int offset;
    dir dirOfSnake = UP;

    std::vector<SDL_FRect> squareList;
};

Snake::Snake(int nSquares, int offset_) : offset(offset_), squareList(nSquares){
    for (int i = 0; i < nSquares; ++i){
        squareList[i] = (SDL_FRect({originalPosX + i * offset, originalPosY, widthOfSqaure, heightOfSqaure}));
    }
    srand(time(0));
}

Snake::~Snake(){
    squareList.clear();
}

int Snake::size(){
    return squareList.size();
}

dir Snake::getDir(){
    return dirOfSnake;
}

void Snake::draw(const int* color, SDL_Renderer*& renderer){
    for (auto& square : squareList){
        SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);
        SDL_RenderFillRect(renderer, &square);
    }
}

void Snake::addSquare(){
    if (squareList.empty()){
        squareList.push_back(SDL_FRect({originalPosX, originalPosY, widthOfSqaure, heightOfSqaure}));
    } else {
        const SDL_FRect& lastSqaure = squareList.back();
        if (dirOfSnake == UP)
            squareList.push_back(SDL_FRect({lastSqaure.x, lastSqaure.y + offset, widthOfSqaure, heightOfSqaure}));
        if (dirOfSnake == DOWN)
            squareList.push_back(SDL_FRect({lastSqaure.x, lastSqaure.y - offset, widthOfSqaure, heightOfSqaure}));
        if (dirOfSnake == LEFT)
            squareList.push_back(SDL_FRect({lastSqaure.x + offset, lastSqaure.y, widthOfSqaure, heightOfSqaure}));
        if (dirOfSnake == RIGHT)
            squareList.push_back(SDL_FRect({lastSqaure.x - offset, lastSqaure.y, widthOfSqaure, heightOfSqaure}));

    }
}

void Snake::up(){
    if (squareList[0].y >= offset){
        for (int i = size() - 1; i > 0; --i){
            squareList[i].x = squareList[i - 1].x;
            squareList[i].y = squareList[i - 1].y;
        }
        squareList[0].y -= offset;
    }
    dirOfSnake = UP;
}

void Snake::down()
{
    if (squareList[0].y < WINDOW_HEIGHT - offset){
        for (int i = size() - 1; i > 0; --i){
            squareList[i].x = squareList[i - 1].x;
            squareList[i].y = squareList[i - 1].y;
        }
        squareList[0].y += offset;
    }
    dirOfSnake = DOWN;
}

void Snake::left()
{
    if (squareList[0].x >= offset){
        for (int i = size() - 1; i > 0; --i){
            squareList[i].x = squareList[i - 1].x;
            squareList[i].y = squareList[i - 1].y;
        }
        squareList[0].x -= offset;
    }
    dirOfSnake = LEFT;
}

void Snake::right()
{
    if (squareList[0].x < WINDOW_WIDTH - offset){
        for (int i = size() - 1; i > 0; --i){
            squareList[i].x = squareList[i - 1].x;
            squareList[i].y = squareList[i - 1].y;
        }
        squareList[0].x += offset;
    }
    dirOfSnake = RIGHT;
}

void Snake::dirUp(){
    if (dirOfSnake != DOWN) dirOfSnake = UP;
}

void Snake::dirDown(){
    if (dirOfSnake != UP) dirOfSnake = DOWN;
}

void Snake::dirLeft(){
    if (dirOfSnake != RIGHT) dirOfSnake = LEFT;
}

void Snake::dirRight(){
    if (dirOfSnake != LEFT) dirOfSnake = RIGHT;
}

bool Snake::checkIntersect(){
    bool intersect = false;
    const SDL_FRect& head = squareList[0];

    for (int i = 1; i < squareList.size(); ++i){
        if (head.x == squareList[i].x && head.y == squareList[i].y) {
            intersect = true;
            break;
        }
    }

    return intersect;
}

void Snake::generateApple(SDL_Renderer*& gRender){
    if (appleExists == false){
        float randomX = getRandomNumberWithStep(0, WINDOW_WIDTH - offset * 2, offset);
        float randomY = getRandomNumberWithStep(0, WINDOW_HEIGHT - offset * 2, offset);

        apple = {randomX, randomY, widthOfSqaure, heightOfSqaure};
        appleExists = true;
    }
    SDL_SetRenderDrawColor(gRender, 0xFF, 0x00, 0x00, 0x00);
    SDL_RenderFillRect(gRender, &apple);
}

std::vector<int> Snake::getPos(){
    return {static_cast<int>(squareList[0].x), static_cast<int>(squareList[1].y)};
}

int getRandomNumberWithStep(int minVal, int maxVal, int step){
    if (step <= 0){
        return -1;
    }
    int numChoices = static_cast<int>(std::floor(static_cast<int>((maxVal - minVal)) / step)) + 1;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> dist(0, numChoices - 1);
    int randomStepIndex = dist(generator);

    return minVal + randomStepIndex * step;
}
