#include "camera.h"
#include <math.h>
#define SPEED 0.1

void move(Camera *cam, PlayerInput *p){
if (p->W_KEY)
    {
        cam->posx -= sin(cam->anglez / 180 * M_PI) * SPEED;
        cam->posy -= cos(cam->anglez / 180 * M_PI) * SPEED;
    }
    if (p->S_KEY)
    {
        cam->posx += sin(cam->anglez / 180 * M_PI) * SPEED;
        cam->posy += cos(cam->anglez / 180 * M_PI) * SPEED;
    }
    if (p->A_KEY)
    {
        cam->posx += cos(cam->anglez / 180 * M_PI) * SPEED;
        cam->posy += sin(cam->anglez / 180 * M_PI) * -SPEED;
    }
    if (p->D_KEY)
    {
        cam->posx += cos(cam->anglez / 180 * M_PI) * -SPEED;
        cam->posy += sin(cam->anglez / 180 * M_PI) * SPEED;
    }
}
void rotate(Camera *cam, PlayerInput *p){
    cam->anglex -= (p->DY) * 0.2;
    cam->anglez -= (p->DX) * 0.2;

    cam->anglex = cam->anglex >= 0 ? 0 : cam->anglex;
    cam->anglex = cam->anglex <= -180 ? -180 : cam->anglex;
}
