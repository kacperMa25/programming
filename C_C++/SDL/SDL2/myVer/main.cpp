#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

const int W_WIDTH = 640;
const int W_HEIGHT = 480;

SDL_Window *window = NULL;

SDL_Surface *screenSurface = NULL;

SDL_Surface *helloWorldSurface = NULL;

bool init() {
    bool sucess = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        sucess = false;
        printf("Couldn't init SDL, SDL Error: %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow(
            "SDL HelloWorld, My Ver", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, W_WIDTH, W_HEIGHT, SDL_WINDOW_SHOWN);

        if (window == NULL) {
            sucess = false;
            printf("Couldn't init window, SDL Error: %s\n", SDL_GetError());
        } else {
            screenSurface = SDL_GetWindowSurface(window);
        }
    }

    return sucess;
}

bool loadMedia() {
    bool success = true;

    helloWorldSurface = SDL_LoadBMP("hello_world.bmp");
    if (helloWorldSurface == NULL) {
        printf("Couldn't load BMP, SDL Error: %s", SDL_GetError());
        success = false;
    }
    return success;
}

void close() {
    SDL_FreeSurface(helloWorldSurface);
    helloWorldSurface = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

int main(int argc, char *args[]) {
    if (!init()) {
        printf("Failed to initialize\n");
    } else {
        if (!loadMedia()) {
            printf("Failed to load media\n");
        } else {
            SDL_BlitSurface(helloWorldSurface, NULL, screenSurface, NULL);

            SDL_UpdateWindowSurface(window);

            SDL_Event e;
            bool quit = false;
            while (quit == false) {
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT)
                        quit = true;
                }
            }
        }
    }
    close();
}
