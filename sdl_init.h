#pragma once

#include <SDL.h>
#include <SDL_image.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FRAME_RATE 60

void init_sdl();

extern SDL_Window* window;
extern SDL_Renderer* renderer;