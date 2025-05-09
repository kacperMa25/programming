#include "Snake.cpp"

SDL_Window* gWindow = NULL;
SDL_Renderer* gRender = NULL;

bool init();

void close();

int main() {
    if (init()){
        SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

        bool quit = false;
        SDL_Event e;
        Snake snake(3);
        int color[] = {0x00, 0x00, 0x00, 0x00};
        SDL_SetRenderDrawColor(gRender, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRender);
        
        snake.generateApple(gRender);
        snake.draw(color, gRender);
        SDL_RenderPresent(gRender);
        
        const uint64_t FRAME_DELAY = 16;

        while(!quit){
            uint64_t frameStart = SDL_GetTicks();

            while(SDL_PollEvent(&e) != 0){
                if (e.type == SDL_EVENT_QUIT){
                    quit = true;
                } else if(e.type == SDL_EVENT_KEY_DOWN) {

                    switch (e.key.key){
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
                    SDL_SetRenderDrawColor(gRender, 0xFF, 0xFF,  0xFF, 0xFF);
                    SDL_RenderClear(gRender);
                    
                    snake.generateApple(gRender);
                    auto currPos = snake.getPos();
                    if (currPos[0] == apple.x && currPos[1] == apple.y){
                        snake.addSquare();
                        appleExists = false;
                        snake.generateApple(gRender);
                    }
                    snake.draw(color, gRender);
                    SDL_RenderPresent(gRender);

                    uint64_t frameTime = SDL_GetTicks() - frameStart;
                    if (frameTime < FRAME_DELAY){
                        SDL_Delay(FRAME_DELAY - frameTime);
                    }
                }
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
