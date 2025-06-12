#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include <SDL.h>
#include <SDL_image.h>

#include "sdl_init.h"
#include "cactus.h"

#define DINO_HEIGHT 80
#define DINO_WIDTH 30
#define CACTUS_HEIGHT 60
#define CACTUS_WIDTH 40
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FRAME_RATE 60
#define JUMP_HEIGHT -200
#define GRAVITY 4


int main (){
    srand(time(NULL));

    init_sdl();
     // Load an image
    SDL_Surface* dino_surface = IMG_Load("dino.png");
    if (dino_surface == NULL) {
        fprintf(stderr, "Failed to load image: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_Surface* cactus_surface = IMG_Load("cactus.png");
    if (cactus_surface == NULL) {
        fprintf(stderr, "Failed to load image: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Create a texture from the loaded surface
    SDL_Texture* dino_texture = SDL_CreateTextureFromSurface(renderer, dino_surface);
    if (dino_texture == NULL) {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(dino_surface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_Texture* cactus_texture = SDL_CreateTextureFromSurface(renderer, cactus_surface);
    if (cactus_texture == NULL) {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(dino_surface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Free the surface
    SDL_FreeSurface(dino_surface);
    SDL_FreeSurface(cactus_surface);

    // vectors for our characters
    vec_t dino_pos = {.x = 50, .y = SCREEN_HEIGHT-DINO_HEIGHT};
    vec_t dino_vel = {.x = 0, .y = 0 };
    vec_t cactus_pos = {.x = SCREEN_WIDTH-CACTUS_HEIGHT, .y = SCREEN_HEIGHT-CACTUS_HEIGHT};
    //vec_t cactus_vel = {.x = 0, .y = 0};

    int buffer = 0;
    int running = 1;
    int jumping = 0;
    int spawn_count = 0;
    int frames = 0;
    int cactus_speed = 3;
    int next_spawn = (rand() % 50) + 100; 
    int frames_since_last_spawn = 0;

    cactus_list_t* list = malloc(sizeof(cactus_list_t));
    init(list);
    while (running){
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
            // When the user clicks the "X" in the window we get an SDL_QUIT event
            running = false;
            } else if (event.type == SDL_KEYDOWN) {
            // Handle key down events
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                     jumping = 1;
                    if (dino_pos.y >= SCREEN_HEIGHT - DINO_HEIGHT) {
                        dino_vel.y = JUMP_HEIGHT;
                    }
                    break;
                default:
                    break;
                }
            }
        }

        if (frames % 2400 == 0 && frames != 0){
            cactus_speed += 1.5;
            buffer += 7;
        }
    
        // Add a cacti to the list
        if (frames_since_last_spawn >= next_spawn) {
            add(list, cactus_pos.x);
            spawn_count++;
            frames_since_last_spawn = 0;
            next_spawn = (rand() % 50) + 100 - buffer; 
        } else {
            frames_since_last_spawn++;
        }

        //update dino position
        if (jumping && dino_pos.y <= SCREEN_HEIGHT - DINO_HEIGHT){
            dino_pos.y += dino_vel.y / FRAME_RATE;
            dino_vel.y += GRAVITY;
        } else if (dino_pos.y >= SCREEN_HEIGHT - DINO_HEIGHT && jumping == 1) {
            dino_pos.y = SCREEN_HEIGHT - DINO_HEIGHT;
            dino_vel.y = 0;
            jumping = 0;
        }

        //check cactus position
        if(list->head != NULL){
            if (list->head->value < -40){
                take(list);
                spawn_count--;
            }
        }
        
        SDL_RenderClear(renderer); 

        // Draw Background
        SDL_Rect background = {
            .x = 0,
            .y = 0,
            .w = SCREEN_WIDTH,
            .h = SCREEN_HEIGHT
        };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &background);

        // Draw the dinosaur
        SDL_Rect dino_dest = {
            .x = dino_pos.x, 
            .y = dino_pos.y, 
            .w = DINO_WIDTH, 
            .h = DINO_HEIGHT
        };

        SDL_RenderCopy(renderer, dino_texture, NULL, &dino_dest);

        // Update positions for all cacti in queue
        for (int i = 1; i<=spawn_count && spawn_count != 0; i++){
            float upd_cactus_pos;
            //get new position
            upd_cactus_pos = pull(list,i) - cactus_speed;
            //adjust position in linked list
            adjust(list, upd_cactus_pos, i);

            // Draw the cactus
            SDL_Rect cactus_dest = {
            .x = upd_cactus_pos,
            .y = cactus_pos.y,
            .w = CACTUS_WIDTH,
            .h = CACTUS_HEIGHT
            };

            SDL_RenderCopy(renderer, cactus_texture, NULL, &cactus_dest);

            // convert sdl_rect into collision detection struct
            collision_rect dino_rect = { dino_pos.x, dino_pos.y, DINO_WIDTH, DINO_HEIGHT};
            collision_rect cactus_rect = { cactus_dest.x, cactus_dest.y, CACTUS_WIDTH, CACTUS_HEIGHT};

            if (checkCollision(dino_rect, cactus_rect)) {
                printf("Your score was %d\n", frames);
                running = 0;
                break; 
            }

        }
        
    
        // Update the display
        SDL_RenderPresent(renderer);

        // Pause before drawing the next frame
        SDL_Delay(1000 / FRAME_RATE);

        frames++;
    }

    // Shut down and exit
    SDL_DestroyTexture(dino_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    destroy(list);
    free(list);

    return 0;
}

