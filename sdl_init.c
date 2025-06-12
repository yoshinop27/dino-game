#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "sdl_init.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void init_sdl (){
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        // Something went wrong. Log an error and exit.
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Create an SDL window (centered on screen, 640x480)
    window = SDL_CreateWindow("Dino Game",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH, SCREEN_HEIGHT, 
    SDL_WINDOW_SHOWN);
    if (window == NULL) {
        // Something went wrong. Log an error, shut down SDL, and then exit
        fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

     // Create a renderer for the window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        // Something went wrong. Log and error and shut down
        fprintf(stderr, "Failed to create SDL renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Initialize SDL_image
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
        // Failed. Log an error, clean up, and exit
        fprintf(stderr, "Failed to initialize SDL_image: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}