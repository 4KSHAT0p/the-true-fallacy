#include <SDL.h>
#include <SDL_image.h>

#include "objects.h"
#include "renderwindow.h"


bool check_collision_hor(stobj o)
{
    if (o.dest.x <= 0)
    {
        return true;
    }
    if (o.dest.x + o.dest.w >= winw)
    {
        return true;
    }
    return false;
}

bool check_collision_ver(stobj o)
{
    if (o.dest.y + o.dest.h >= winh-(1280/9))
    {
        return true;
    }
    if (o.dest.y <= 0)
    {
        return true;
    }
    return false;
}
