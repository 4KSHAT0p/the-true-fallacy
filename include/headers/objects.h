#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>

typedef struct stobj
{
    SDL_Texture **imgarr; // image ka array
    SDL_Rect *source;     // source ka array hoga. agar 1 source hi kyu na ho
    SDL_Rect dest;
} stobj;

typedef struct dyobj
{
    stobj sto;
    int frame; // src useful to update according to fps (no. of source)
    float x_vel, y_vel;
    SDL_RendererFlip flip;
    bool isMoving;
    bool isAttacking;
    float frameTime;
    float deltaTime;
    Uint32 prevtime, currtime; // ms
    int count;
    int health;
} dyobj;

bool check_collision_hor(stobj obj);
bool check_collision_ver(stobj obj);