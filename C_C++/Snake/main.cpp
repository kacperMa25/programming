#include "Snake.cpp"


const int WINDOW_HEIGHT = 720;
const int WINDOW_WIDTH = 640;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRender = NULL;

bool init();

void close();

void processFrame(Snake& snake, const int* snakeColor, const int* bgColor, const SDL_FRect snakeView);

int main() {
    if (init()){
        SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

        bool quit = false;
        SDL_Event e;
        Snake snake(3, 10);
        int color[] = {0x00, 0x00, 0x00, 0x00};
        int bgColor[] = {0x80, 0xAA, 0x80, 0x00};
        SDL_Rect snakeView;
        snakeView.x = (WINDOW_WIDTH - GAME_WIDTH) / 2;
        snakeView.y = (WINDOW_HEIGHT - GAME_HEIGHT) / 2;
        snakeView.w = GAME_WIDTH;
        snakeView.h = GAME_HEIGHT;
        SDL_FRect snakeBg;
        snakeBg.x = (WINDOW_WIDTH - GAME_WIDTH) / 2;
        snakeBg.y = (WINDOW_HEIGHT - GAME_HEIGHT) / 2;
        snakeBg.w = GAME_WIDTH;
        snakeBg.h = GAME_HEIGHT;
        SDL_SetRenderDrawColor(gRender, bgColor[0], bgColor[1],  bgColor[2], bgColor[3]);
        SDL_RenderFillRect(gRender, &snakeBg);
        SDL_RenderClear(gRender);

        snake.generateApple(gRender);
        snake.draw(color, gRender);
        SDL_RenderPresent(gRender);

        const unsigned int TARGET_FPS = 10;
        const unsigned int FRAME_DELAY =  1000 / TARGET_FPS;
        while(!quit && !snake.checkIntersect()){
            uint64_t frameStart = SDL_GetTicks();

            while(SDL_PollEvent(&e) != 0){
                if (e.type == SDL_EVENT_QUIT){
                    quit = true;
                } else if(e.type == SDL_EVENT_KEY_DOWN) {

                    switch (e.key.key){
                        case SDLK_UP:
                        snake.dirUp();
                        break;
                        case SDLK_DOWN:
                        snake.dirDown();
                        break;
                        case SDLK_LEFT:
                        snake.dirLeft();
                        break;
                        case SDLK_RIGHT:
                        snake.dirRight();
                        break;
                    }

                }
            }
            processFrame(snake, color, bgColor, snakeBg);

            uint64_t frameTime = SDL_GetTicks() - frameStart;
            if (frameTime < FRAME_DELAY){
                SDL_Delay(FRAME_DELAY - frameTime);
            }
        }
    }
    close();
    return 0;
}

bool init(){
    bool success = true;

    if (!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("Couldn't initialize SDL!: %s\n", SDL_GetError());
        success = false;
    } else {
        if (!SDL_CreateWindowAndRenderer("Moving Square", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &gWindow, &gRender)){
            SDL_Log("Couldn't create window or render! SDL Error: %s", SDL_GetError());
            success = false;
        } else {
            SDL_SetRenderDrawColor(gRender, 0xFF, 0XFF, 0xFF, 0xFF);
        }
    }
    return success;
}

void close(){
    SDL_DestroyRenderer(gRender);
    SDL_DestroyWindow(gWindow);

    SDL_Quit();
}

void processFrame(Snake& snake, const int* snakeColor, const int* bgColor, const SDL_FRect snakeBg){

    switch (snake.getDir()){
        case SDLK_UP:
        snake.up();
        break;
        case SDLK_DOWN:
        snake.down();
        break;
        case SDLK_LEFT:
        snake.left();
        break;
        case SDLK_RIGHT:
        snake.right();
        break;
    }

    SDL_SetRenderDrawColor(gRender, bgColor[0], bgColor[1],  bgColor[2], bgColor[3]);
    SDL_RenderFillRect(gRender, &snakeBg);
    SDL_RenderClear(gRender);

    snake.generateApple(gRender);
    auto currPos = snake.getPos();
    if (currPos[0] == apple.x && currPos[1] == apple.y){
        snake.addSquare();
        appleExists = false;
        snake.generateApple(gRender);
    }
    snake.draw(snakeColor, gRender);
    SDL_RenderPresent(gRender);
}
