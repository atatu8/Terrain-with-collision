#ifndef CAMERA_H
#define CAMERA_H

typedef struct{
    float posx;
    float posy;
    float posz;
    float anglex;
    float anglez;
} Camera;

typedef struct{
    char W_KEY;
    char S_KEY;
    char A_KEY;
    char D_KEY;
    char DX;
    char DY;
} PlayerInput;

void move( Camera *cam, PlayerInput *p);
void rotate( Camera *cam, PlayerInput *p);

#endif