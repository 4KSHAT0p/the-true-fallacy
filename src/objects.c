#include <SDL.h>
#include <SDL_image.h>

#include "objects.h"
#include "renderwindow.h"
// bool check_collision(stobj o1, stobj o2)
// {
//     if (o1.dest.x + o1.dest.w >= o2.dest.x || o2.dest.x + o2.dest.w >= o1.dest.x) // hor collision
//     {
//         return true;
//     }
//     if (o1.dest.y + o1.dest.h >= o2.dest.y || o2.dest.y + o2.dest.h >= o1.dest.y) // vertical collision
//     {
//         return true;
//     }
//     return false;
// }

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
