#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "objects.h"
#define winw 1280
#define winh 720

typedef struct renderwindow
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} renwin;

void create_window_ren(renwin rw, char *title);
void render_s(SDL_Renderer *renderer, stobj obj, SDL_Rect src, SDL_Rect dst,int frame);
void display(SDL_Renderer* renderer);
void clear(SDL_Renderer* renderer);
