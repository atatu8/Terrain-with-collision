#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "camera.h"
#include <math.h>
#include <time.h>

#define WORLD_W 200
#define WORLD_H 200
#define SCREEN_W 800
#define SCREEN_H 800
#define SIZE 1 //TODO if SIZE < 1 i see strange things

void changeHeight(Camera *cam);

char exitfromgame = 0;

double cursorposx;
double cursorposy;
double oldcursorposx;
double oldcursorposy;

typedef struct
{
    float x;
    float y;
    float z;

} Point3;

Point3 vertices[WORLD_H][WORLD_W];
char active[WORLD_H][WORLD_W];

unsigned int ind[WORLD_H - 1][WORLD_W - 1][6];
unsigned char colors[WORLD_H][WORLD_W][3];

void generateHill(float r, float height, float x, float y){
    float startx = x - r;
    float starty = y - r;

    for(int i = starty; i < y + r; i++){
        if(i >= 0 && i < WORLD_H)
        for(int j = startx; j < x + r; j++){
            if(j >= 0 && j < WORLD_W){
                float len = pow(x - j,2) + pow(y-i, 2);
                if(r * r >= len){
                    float temp = height * cos((sqrt(len/180)));
                    if(temp <= 0 )
                        continue;
                    vertices[i][j].z+=temp;
                    // vertices[i][j].z+= 3 * (r/len);
                }
            }
        }
    }
}


void initWorld()
{
    srand(time(0));
    for (int i = 0; i < WORLD_H; i++)
    {
        for (int j = 0; j < WORLD_W; j++)
        {
            vertices[i][j].x = j*SIZE;
            vertices[i][j].y = i*SIZE;
            vertices[i][j].z = (rand() % 200 * 0.002);
        }
    }


    for(int i = 0; i < 50; i++){
        generateHill(20 + rand()%30, 4 + rand()%15, rand()%WORLD_W, rand()%WORLD_H);
    }

    for (int i = 0; i < WORLD_H; i++)
    {
        for (int j = 0; j < WORLD_W - 1; j++)
        {
            colors[i][j][0] = rand() % 255;
            colors[i][j][1] = rand() % 256;
            colors[i][j][2] = rand() % 256;
        }
    }

    for (int i = 0; i < WORLD_H - 1; i++)
    {
        for (int j = 0; j < WORLD_W - 1; j++)
        {
            ind[i][j][0] = j + WORLD_W * i;
            ind[i][j][1] = j + WORLD_W * i + WORLD_W;
            ind[i][j][2] = j + WORLD_W * i + WORLD_W + 1;

            ind[i][j][3] = j + WORLD_W * i;
            ind[i][j][4] = j + WORLD_W * i + WORLD_W + 1;
            ind[i][j][5] = j + WORLD_W * i + 1;
        }
    }

    for (int i = 0; i < WORLD_H; i++)
    {
        for (int j = 0; j < WORLD_W; j++)
        {
            active[i][j] = vertices[i][j].z >= 0.4 ? 1 : 0;
        }
    }
}

void drawWorld()
{

    glColor3f(0, 1, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vertices);
    glColorPointer(3, GL_UNSIGNED_BYTE, 0, &colors);

    glPushMatrix();
    glTranslated(0, 0, -3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, (WORLD_W - 1) * (WORLD_H - 1) * 6, GL_UNSIGNED_INT, &ind);
    glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void handleInput(PlayerInput *p, GLFWwindow *window)
{
    oldcursorposx = cursorposx;
    oldcursorposy = cursorposy;
    glfwGetCursorPos(window, &cursorposx, &cursorposy);

    p->W_KEY = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
    p->S_KEY = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
    p->A_KEY = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
    p->D_KEY = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
    p->DY = oldcursorposy - cursorposy;
    p->DX = oldcursorposx - cursorposx;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        exitfromgame = 1;
}

void moveWorld(Camera *cam, PlayerInput *p)
{
    changeHeight(cam);

    move(cam, p);
    rotate(cam, p);

    glRotatef(cam->anglex, 1, 0, 0);
    glRotatef(cam->anglez, 0, 0, 1);
    glTranslatef(cam->posx, cam->posy, cam->posz);
}


float findArea(float x1, float y1, float x2, float y2, float x3, float y3)
{
    return (abs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2))) / 2;
}

void changeHeight(Camera *cam)
{
    int x = ((int)abs(cam->posx)/SIZE);
    int y = ((int)abs(cam->posy)/SIZE);

    float fx = abs(cam->posx)/SIZE;
    float fy = abs(cam->posy)/SIZE;

    float area1,area2,area3,area4, area5, area6;
    area1 = findArea(fx, fy, 1 + x, 0 + y, 1 + x, 1 + y) / 0.5;
    area2 = findArea(fx, fy, 0 + x,0 + y,1 + x,0 + y) / 0.5;
    area3 = findArea(fx, fy, 0 + x,0 + y,1 + x,1 + y) / 0.5;

    area4 = findArea(fx, fy, 0 + x, 1 + y, 1 + x, 1 + y) / 0.5;
    area5 = findArea(fx, fy, 0 + x,0 + y,1 + x,1 + y) / 0.5;
    area6 = findArea(fx, fy, 0 + x,0 + y,0 + x,1 + y) / 0.5;

    if(area4 + area5 + area6 <= 1.001){
        cam->posz = -(vertices[y][x].z * area4 + vertices[y+1][x].z * area5 + vertices[y+1][x+1].z * area6);
    }
    else
        cam->posz = -(vertices[y][x].z * area1 + vertices[y+1][x+1].z * area2 + vertices[y][x+1].z * area3);

}

int main()
{
    initWorld();
    PlayerInput playerInput = {0, 0, 0, 0, 0, 0};
    Camera camera = {-100, -100, 0, 0, 0};

    glfwInit();
    GLFWwindow *window = glfwCreateWindow(800, 800, "test", 0, 0);
    glfwMakeContextCurrent(window);

    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glFrustum(-1, 1, -1, 1, 1, 200);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwGetCursorPos(window, &cursorposx, &cursorposy);

    while (!glfwWindowShouldClose(window))
    {
        if (exitfromgame)
        {
            glfwTerminate();
            return 0;
        }

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glPushMatrix();
        handleInput(&playerInput, window);
        moveWorld(&camera, &playerInput);
        drawWorld();
        glPopMatrix();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}