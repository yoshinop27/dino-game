#pragma ONCE

#include "cactus.h"

void init(cactus_list_t* q){
    q->head = NULL;
}

void add (cactus_list_t* q, float x){
    if (q->head == NULL){
        node_t* new = malloc(sizeof(node_t));
        new->value = x;
        new->next = NULL;
        q->head = new;
    } else {
        node_t* traverse = q->head;
        while(traverse->next != NULL){
            traverse = traverse->next;
        }
        node_t* new = malloc(sizeof(node_t));
        new->value = x;
        new->next = NULL;
        traverse->next = new;
    }
}

void take (cactus_list_t* q){
    if (q->head != NULL){
        node_t* temp = q->head;
        q->head = q->head->next;
        free(temp);
    }
}

float pull(cactus_list_t* q, int s){
    if (q->head != NULL && s == 1){
        return q->head->value;
    } else if (q->head != NULL){
        node_t* traverse = q->head;
        for (int j = 1; j<s; j++){
             traverse = traverse->next;
        }
        return traverse->value;
    } else {
        return -100;
    }
}

void adjust(cactus_list_t* q, float v, int s){
    if (q->head != NULL && s == 1){
        q->head->value = v;
    } else if (q->head != NULL){
        node_t* traverse = q->head;
        for (int j = 1; j<s; j++){
             traverse = traverse->next;
        }
        traverse->value = v;
    }
}

void destroy(cactus_list_t* q){
    if (q->head == NULL){}
    else {
        node_t* current = q->head;
        node_t* next;
        while (current != NULL){
            next = current->next;
            free(current);
            current=next;
        }
        q->head = NULL;
    }
}

int checkCollision(collision_rect a, collision_rect b){
    if (a.x < b.x + b.width && a.x + a.width > b.x &&
    a.y < b.y + b.height && a.y + a.height > b.y){
        return 1;
    }
    else {
        return 0;
    }
}