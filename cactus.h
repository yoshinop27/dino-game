#pragma once

#include <SDL.h>
#include <SDL_image.h>

typedef struct vec {
    float x;
    float y;
} vec_t;

typedef struct node {
    float value;
    struct node* next;
} node_t;

typedef struct cactus_list {
    node_t* head;
} cactus_list_t;

typedef struct collisions {
    float x, y;
    float width, height;
} collision_rect;

// initiates the queue of cacti
void init(cactus_list_t* q);

// add a cacti to queue
void add(cactus_list_t* q, float x);

// remove cacti from queue
void take(cactus_list_t* q);

// pop value from queue
float pull(cactus_list_t* q, int i);

// destory remaining list
void destroy(cactus_list_t* q);

// adjust a position in the queue
void adjust(cactus_list_t* q, float v, int s);

// check for a collision
int checkCollision(collision_rect a, collision_rect b);