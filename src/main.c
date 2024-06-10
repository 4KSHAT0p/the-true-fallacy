#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "objects.h"
#include "renderwindow.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define FPS 144.0f

typedef enum GAMESTATE
{
    MENU,
    GAMEPLAY
} gs;

stobj bgs;    // backgrounds
stobj lgmovr; // menu and game over screens
dyobj rock;
dyobj tohno;      // p1
dyobj ryougi;     // p2
dyobj groundTile; // ground tile
TTF_Font *font;   // score
SDL_Surface *surface;
SDL_Texture *text;

gs gstate = MENU;
bool running = true;
renwin REWI;
SDL_Event evnt;

SDL_Rect scr = (SDL_Rect){0, 0, winw, winh};
SDL_Color textcolor = {255, 255, 255, 255}; // font color
SDL_Rect txtdst = {0, 0, winw, 50};

int n = 0; // for random backgrounds

void INIT()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 && TTF_Init() < 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0 && Mix_Init(MIX_INIT_MP3))
    {
        printf("SDL initialization failed: \n%s\n%s", SDL_GetError(), TTF_GetError());
        return;
    }
    srand(time(NULL));

    REWI.window = SDL_CreateWindow("The True Fallacy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winw, winh, SDL_WINDOW_SHOWN);
    REWI.renderer = SDL_CreateRenderer(REWI.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // image initialization

    bgs.imgarr = (SDL_Texture **)calloc(5, sizeof(SDL_Texture *));
    lgmovr.imgarr = (SDL_Texture **)calloc(3, sizeof(SDL_Texture *));
    tohno.sto.imgarr = (SDL_Texture **)calloc(1, sizeof(SDL_Texture *));
    groundTile.sto.imgarr = (SDL_Texture **)calloc(5, sizeof(SDL_Texture *)); // one groundtile for every scene

    bgs.imgarr[0] = IMG_LoadTexture(REWI.renderer, "res/dark.jpg");
    bgs.imgarr[1] = IMG_LoadTexture(REWI.renderer, "res/dark1.jpg");
    bgs.imgarr[2] = IMG_LoadTexture(REWI.renderer, "res/dark3.jpg");
    bgs.imgarr[3] = IMG_LoadTexture(REWI.renderer, "res/dark4.jpg");
    bgs.imgarr[4] = IMG_LoadTexture(REWI.renderer, "res/green.jpg");

    lgmovr.imgarr[0] = IMG_LoadTexture(REWI.renderer, "res/loadingscreen2.jpg");
    lgmovr.imgarr[1] = IMG_LoadTexture(REWI.renderer, "res/tt1.jpg");
    lgmovr.imgarr[2] = IMG_LoadTexture(REWI.renderer, "res/tt2.jpg");

    tohno.sto.imgarr[0] = IMG_LoadTexture(REWI.renderer, "res/characters.png");
    ryougi.sto.imgarr = &tohno.sto.imgarr[0];

    groundTile.sto.imgarr[0] = IMG_LoadTexture(REWI.renderer, "res/jameen1.jpg");
    groundTile.sto.imgarr[1] = groundTile.sto.imgarr[0];
    groundTile.sto.imgarr[2] = groundTile.sto.imgarr[0];
    groundTile.sto.imgarr[3] = groundTile.sto.imgarr[0];
    groundTile.sto.imgarr[4] = IMG_LoadTexture(REWI.renderer, "res/grass1.png");

    font = TTF_OpenFont("fonts/HACKED.ttf", 100);

    // source initialization

    bgs.source = (SDL_Rect *)calloc(5, sizeof(SDL_Rect)); // number of source = number of images for background
    groundTile.sto.source = (SDL_Rect *)calloc(5, sizeof(SDL_Rect));
    lgmovr.source = (SDL_Rect *)calloc(3, sizeof(SDL_Rect));
    tohno.sto.source = (SDL_Rect *)calloc(13, sizeof(SDL_Rect)); // number of source= number of frames for a moving character
    ryougi.sto.source = (SDL_Rect *)calloc(16, sizeof(SDL_Rect));

    for (int i = 0; i < 5; i++)
    {
        bgs.source[i].x = 0;
        bgs.source[i].y = 0;
        if (SDL_QueryTexture(bgs.imgarr[i], NULL, NULL, &bgs.source[i].w, &bgs.source[i].h) < 0)
        {
            printf("source not initialized", SDL_GetError());
        }
    }
    groundTile.sto.source[0] = (SDL_Rect){0, 0, 871, 871};
    groundTile.sto.source[1] = groundTile.sto.source[0];
    groundTile.sto.source[2] = groundTile.sto.source[0];
    groundTile.sto.source[3] = groundTile.sto.source[0];
    groundTile.sto.source[4] = (SDL_Rect){0, 0, 641, 641};

    for (int i = 0; i < 3; i++)
    {
        lgmovr.source[i].x = 0;
        lgmovr.source[i].y = 0;
        if (SDL_QueryTexture(lgmovr.imgarr[i], NULL, NULL, &lgmovr.source[i].w, &lgmovr.source[i].h) < 0)
        {
            printf("source not initialized", SDL_GetError());
        }
    }

    tohno.sto.source[0] = (SDL_Rect){139, 0, 120, 137}; // running
    tohno.sto.source[1] = (SDL_Rect){260, 0, 140, 126};
    tohno.sto.source[2] = (SDL_Rect){510, 0, 134, 132};
    tohno.sto.source[3] = (SDL_Rect){645, 0, 149, 127};
    tohno.sto.source[4] = (SDL_Rect){0, 0, 138, 131}; // standing

    tohno.sto.source[5] = (SDL_Rect){0, 138, 189, 122}; // fighting
    tohno.sto.source[6] = (SDL_Rect){190, 138, 194, 114};
    tohno.sto.source[7] = (SDL_Rect){385, 138, 199, 104};
    tohno.sto.source[8] = (SDL_Rect){585, 138, 197, 92};
    tohno.sto.source[9] = (SDL_Rect){783, 138, 169, 107};
    tohno.sto.source[10] = (SDL_Rect){953, 138, 148, 109};
    tohno.sto.source[11] = (SDL_Rect){0, 261, 136, 113};
    tohno.sto.source[12] = (SDL_Rect){704, 263, 243, 153};

    // initial configurations

    tohno.frame = 4; // initially standing
    tohno.flip = SDL_FLIP_HORIZONTAL;
    tohno.x_vel = 0; // velocities
    tohno.y_vel = 0;
    tohno.sto.dest = (SDL_Rect){150, winh - ((1280 / 9) + tohno.sto.source[tohno.frame].h), tohno.sto.source[tohno.frame].w, tohno.sto.source[tohno.frame].h}; // initial position
    tohno.isMoving = false;
    tohno.frameTime = 0.0f;
    tohno.deltaTime = 0.0f;
    tohno.prevtime = 0;
    tohno.currtime = 0;
    tohno.count = 0;
    tohno.health = 1000;

    ryougi.sto.source[0] = (SDL_Rect){720, 420, 190, 98};  // fly
    ryougi.sto.source[1] = (SDL_Rect){720, 519, 190, 97};  // fly
    ryougi.sto.source[2] = (SDL_Rect){955, 413, 130, 121}; // jump
    ryougi.sto.source[3] = (SDL_Rect){955, 261, 140, 151}; // jump
    ryougi.sto.source[4] = (SDL_Rect){0, 413, 105, 167};   // standing

    ryougi.sto.source[5] = (SDL_Rect){221, 420, 98, 109}; // fighting
    ryougi.sto.source[6] = (SDL_Rect){212, 617, 107, 105};
    ryougi.sto.source[7] = (SDL_Rect){321, 617, 107, 89};
    ryougi.sto.source[8] = (SDL_Rect){432, 617, 107, 89};
    ryougi.sto.source[9] = (SDL_Rect){540, 617, 121, 86};
    ryougi.sto.source[10] = (SDL_Rect){662, 617, 131, 84};
    ryougi.sto.source[11] = (SDL_Rect){794, 617, 125, 95};
    ryougi.sto.source[12] = (SDL_Rect){920, 617, 119, 96};
    ryougi.sto.source[13] = (SDL_Rect){0, 734, 115, 104};
    ryougi.sto.source[14] = (SDL_Rect){116, 734, 195, 126};
    ryougi.sto.source[15] = (SDL_Rect){313, 745, 260, 88};

    ryougi.frame = 4; // initial frame
    ryougi.flip = SDL_FLIP_NONE;
    ryougi.x_vel = 0;
    ryougi.y_vel = 0;
    ryougi.sto.dest = (SDL_Rect){winw - 200 - ryougi.sto.source[ryougi.frame].w, winh - ((1280 / 9) + ryougi.sto.source[ryougi.frame].h), ryougi.sto.source[ryougi.frame].w, ryougi.sto.source[ryougi.frame].h};
    ryougi.isMoving = false;
    ryougi.frameTime = 0.0f;
    ryougi.deltaTime = 0.0f;
    ryougi.prevtime = 0;
    ryougi.currtime = 0;
    ryougi.count = 0;
    ryougi.health = 1000;

    n = rand() % 5;

    char arr[102];
    if (tohno.sto.dest.x < ryougi.sto.dest.x)
    {
        sprintf(arr, "    %d                                                                                         %d    ", tohno.health, ryougi.health);
    }
    else
    {
        sprintf(arr, "    %d                                                                                         %d    ", ryougi.health, tohno.health);
    }
    
    surface = TTF_RenderText_Blended(font, arr, textcolor);
    text = SDL_CreateTextureFromSurface(REWI.renderer, surface);
    
}

void RESET()
{
    gstate = MENU;
    n = rand() % 5;
}

void collide_tohno_wall()
{
    if (check_collision_hor(tohno.sto))
    {
        if (tohno.flip == SDL_FLIP_HORIZONTAL)
        {
            tohno.sto.dest.x = winw - tohno.sto.dest.w;
        }
        else
        {
            tohno.sto.dest.x = 0;
        }
    }
    if (check_collision_ver(tohno.sto))
    {
        if (tohno.sto.dest.y <= 0)
        {
            tohno.sto.dest.y = 0;
        }
        else
        {
            tohno.sto.dest.y = winh - (1280 / 9) - tohno.sto.dest.h;
        }
    }
    return;
}

void collide_ryougi_wall()
{
    if (check_collision_hor(ryougi.sto))
    {
        if (ryougi.flip == SDL_FLIP_HORIZONTAL)
        {
            ryougi.sto.dest.x = winw - ryougi.sto.dest.w;
        }
        else
        {
            ryougi.sto.dest.x = 0;
        }
    }
    if (check_collision_ver(ryougi.sto))
    {
        if (ryougi.sto.dest.y <= 0)
        {
            ryougi.sto.dest.y = 0;
        }
        else
        {
            ryougi.sto.dest.y = winh - (1280 / 9) - ryougi.sto.dest.h;
        }
    }
    return;
}

void collision()
{
    if (tohno.sto.dest.x + tohno.sto.dest.w > ryougi.sto.dest.x && tohno.sto.dest.y + tohno.sto.dest.h > ryougi.sto.dest.y && ryougi.sto.dest.x > tohno.sto.dest.x) // horizontal collison
    {
        tohno.sto.dest.x = ryougi.sto.dest.x - tohno.sto.dest.w;
        if (ryougi.isAttacking)
        {
            tohno.health--;
        }
        if (tohno.isAttacking)
        {
            ryougi.health--;
        }
    }

    if (ryougi.sto.dest.x + ryougi.sto.dest.w > tohno.sto.dest.x && ryougi.sto.dest.y + ryougi.sto.dest.h > tohno.sto.dest.y && tohno.sto.dest.x > ryougi.sto.dest.x)
    {
        ryougi.sto.dest.x = tohno.sto.dest.x - ryougi.sto.dest.w;
        if (ryougi.isAttacking)
        {
            tohno.health--;
        }
        if (tohno.isAttacking)
        {
            ryougi.health--;
        }
    }
}

void gameloop()
{
    while (SDL_PollEvent(&evnt))
    {
        switch (evnt.type)
        {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_KEYDOWN:
            if (gstate == MENU)
            {
                if (evnt.key.keysym.sym == SDLK_RETURN)
                    gstate = GAMEPLAY;
            }
            else
            {
                switch (evnt.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    gstate = MENU;
                    break;

                case SDLK_d:
                    tohno.isMoving = true;
                    tohno.flip = SDL_FLIP_HORIZONTAL;
                    tohno.x_vel = 5;
                    break;
                case SDLK_a:
                    tohno.isMoving = true;
                    tohno.flip = SDL_FLIP_NONE;
                    tohno.x_vel = -5;
                    break;
                case SDLK_x:
                    tohno.isAttacking = true;
                    break;
                case SDLK_SPACE:
                    tohno.y_vel = 4;
                    break;
                case SDLK_KP_4:
                    ryougi.isMoving = true;
                    ryougi.flip = SDL_FLIP_NONE;
                    ryougi.x_vel = -5;
                    break;
                case SDLK_KP_6:
                    ryougi.isMoving = true;
                    ryougi.flip = SDL_FLIP_HORIZONTAL;
                    ryougi.x_vel = 5;
                    break;
                case SDLK_KP_ENTER:
                    ryougi.isAttacking = true;
                    break;
                case SDLK_KP_0:
                    ryougi.y_vel = 4;
                    break;
                }
            }
            break;

        case SDL_KEYUP:
            switch (evnt.key.keysym.sym)
            {
            case SDLK_d:
                tohno.isMoving = false;
                tohno.frame = 4; // stop running
                tohno.x_vel = 0;
                break;
            case SDLK_a:
                tohno.isMoving = false;
                tohno.frame = 4;
                tohno.x_vel = 0;
                break;
            case SDLK_x:
                tohno.isAttacking = false;
                tohno.frame = 4; // stop attacking
                break;
            case SDLK_SPACE:
                tohno.y_vel = -5;
                break;
            case SDLK_KP_4:
                ryougi.isMoving = false;
                ryougi.frame = 4;
                ryougi.x_vel = 0;
                break;
            case SDLK_KP_6:
                ryougi.isMoving = false;
                ryougi.frame = 4;
                ryougi.x_vel = 0;
                break;
            case SDLK_KP_ENTER:
                ryougi.isAttacking = false;
                ryougi.frame = 4;
                break;
            case SDLK_KP_0:
                ryougi.y_vel = -5;
                break;
            }
            break;
        }
    }

    if (gstate == MENU)
    {
        SDL_RenderCopy(REWI.renderer, lgmovr.imgarr[0], &lgmovr.source[0], &lgmovr.source[0]); // loading screen/menu screen
    }
    else
    {

        if (tohno.isMoving && !tohno.isAttacking)
        {
            tohno.frameTime += tohno.deltaTime;

            if (tohno.frameTime >= 1.0f / FPS) // we need 144 frames but i only have 4 so i have to show each frame 28.8 (144/5) times
            {
                tohno.frameTime = 0;
                tohno.sto.dest.x += tohno.x_vel;
                tohno.sto.dest.y -= tohno.y_vel;
                collide_tohno_wall();
                collision();
                if (tohno.count > 36)
                {
                    tohno.count = 0;
                    tohno.frame++;
                }
                tohno.frame %= 4;
            }
            tohno.count++;
        }
        else if (tohno.isAttacking)
        {
            tohno.frameTime += tohno.deltaTime;
            if (tohno.frameTime >= 1.0f / FPS) // we need 144 frames but i only have 8 so i have to show each frame 18 (5) (144/8) times
            {
                tohno.frameTime = 0;
                if (tohno.isMoving)
                {
                    tohno.sto.dest.x += tohno.x_vel;
                }
                tohno.sto.dest.y -= tohno.y_vel;
                collide_tohno_wall();
                collision();
                if (tohno.count > 5)
                {
                    tohno.count = 0;
                    tohno.frame++;
                }
                if (tohno.frame > 12) // last attacking frame is 12
                {
                    tohno.frame = 5;
                }
            }
            tohno.count++;
        }
        else // free fall
        {
            tohno.frameTime += tohno.deltaTime;
            if (tohno.frameTime >= 1.0f / FPS)
            {
                tohno.frameTime = 0;
                tohno.sto.dest.x += tohno.x_vel;
                tohno.sto.dest.y -= tohno.y_vel;
                collide_tohno_wall();
                collision();
            }
        }
        if (ryougi.isMoving && !ryougi.isAttacking)
        {
            ryougi.frameTime += ryougi.deltaTime;

            if (ryougi.frameTime >= 1.0f / FPS)
            {
                ryougi.frameTime = 0;
                ryougi.sto.dest.x += ryougi.x_vel;
                ryougi.sto.dest.y -= ryougi.y_vel;
                collide_ryougi_wall();
                collision();
                if (ryougi.count > 20)
                {
                    ryougi.count = 0;
                    ryougi.frame++;
                }
                ryougi.frame %= 2;
            }
            ryougi.count++;
        }
        else if (ryougi.isAttacking)
        {
            ryougi.frameTime += ryougi.deltaTime;
            if (ryougi.frameTime >= 1.0f / FPS)
            {
                ryougi.frameTime = 0;
                if (ryougi.isMoving)
                {
                    ryougi.sto.dest.x += ryougi.x_vel;
                }
                ryougi.sto.dest.y -= ryougi.y_vel;
                collide_ryougi_wall();
                collision();
                if (ryougi.count > 5) // kayde se 144/11
                {
                    ryougi.count = 0;
                    ryougi.frame++;
                }
                if (ryougi.frame > 15) // last attacking frame is 12
                {
                    ryougi.frame = 5;
                }
            }
            ryougi.count++;
        }
        else // free fall
        {
            ryougi.frameTime += ryougi.deltaTime;
            if (ryougi.frameTime >= 1.0f / FPS) // we need 144 frames but i only have 5 so i have to show each frame 28.8 (144/5) times
            {
                ryougi.frameTime = 0;

                ryougi.sto.dest.x += ryougi.x_vel;
                ryougi.sto.dest.y -= ryougi.y_vel;
                collide_ryougi_wall();
                collision();
                if (ryougi.sto.dest.y == winh - (1280 / 9) - ryougi.sto.dest.h) // agar ground pe h to standing
                {
                    ryougi.frame = 4;
                }
                else if (ryougi.y_vel != 0) // otherwise if yvel is not zero it means she's in air
                {
                    if (ryougi.count > 72) // 144/2=72
                    {
                        ryougi.count = 0;
                        ryougi.frame++;
                    }
                    if (ryougi.frame > 3) // last attacking frame is 12
                    {
                        ryougi.frame = 2;
                    }
                }
            }
            ryougi.count++;
        }
        tohno.sto.dest.w = tohno.sto.source[tohno.frame].w; // updating width and height according to current source
        tohno.sto.dest.h = tohno.sto.source[tohno.frame].h;
        ryougi.sto.dest.w = ryougi.sto.source[ryougi.frame].w;
        ryougi.sto.dest.h = ryougi.sto.source[ryougi.frame].h;
        SDL_RenderClear(REWI.renderer);
        SDL_RenderCopy(REWI.renderer, bgs.imgarr[n], &bgs.source[n], &scr); // render a scene
        for (int j = 0; j < 9; j++)                                         // rendering groundtiles
        {
            groundTile.sto.dest.x = j * (1280 / 9);
            groundTile.sto.dest.y = winh - (1280 / 9);
            groundTile.sto.dest.w = 1280 / 9;
            groundTile.sto.dest.h = 1280 / 9;
            SDL_RenderCopy(REWI.renderer, groundTile.sto.imgarr[n], &groundTile.sto.source[n], &groundTile.sto.dest);
        }

        SDL_RenderCopy(REWI.renderer, text, NULL, &txtdst);
        SDL_RenderCopyEx(REWI.renderer, tohno.sto.imgarr[0], &tohno.sto.source[tohno.frame], &tohno.sto.dest, 0.0, NULL, tohno.flip);
        SDL_RenderCopyEx(REWI.renderer, ryougi.sto.imgarr[0], &ryougi.sto.source[ryougi.frame], &ryougi.sto.dest, 0.0, NULL, ryougi.flip);
    }

    SDL_RenderPresent(REWI.renderer);
}

int main(int argc, char **argv)
{
    INIT();

    while (running)
    {
        tohno.prevtime = tohno.currtime;
        tohno.currtime = SDL_GetTicks();
        tohno.deltaTime = (tohno.currtime - tohno.prevtime) / 1000.0f; // secs

        ryougi.prevtime = ryougi.currtime;
        ryougi.currtime = SDL_GetTicks();
        ryougi.deltaTime = (ryougi.currtime - ryougi.prevtime) / 1000.0f; // secs

        gameloop();
    }
    return 0;
}