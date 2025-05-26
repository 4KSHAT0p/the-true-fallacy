#include <SDL.h>
#include <SDL_image.h>

#include "renderwindow.h"

void create_window_ren(renwin rw, char *title)
{
    rw.window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winw, winh, SDL_WINDOW_SHOWN);
    rw.renderer = SDL_CreateRenderer(rw.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void render_s(SDL_Renderer *renderer, stobj obj, SDL_Rect src, SDL_Rect dst,int frame)
{
    SDL_RenderCopy(renderer, obj.imgarr[frame], &src, &dst);
}

void display(SDL_Renderer *renderer)
{
    SDL_RenderPresent(renderer);
}

void clear(SDL_Renderer* ren)
{
    SDL_RenderClear(ren);
}