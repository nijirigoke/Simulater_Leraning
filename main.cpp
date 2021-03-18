//
// Created by T118029 on 2021/03/15.
//

#include <iostream>
#include <GL/glut.h>
//#include "simbase.h"
#include "workspace.h"

#define ROBO_R 5

void robo_init();
void robo_draw();
void Initialize();
void resize(int w, int h);
void wall_draw();

typedef struct {
    double r;/*本体の半径*/
    double x, y;/*本体の位置*/
    double dir;/*本体の進行方向*/
} ROBO;

ROBO robo;

void robo_init() {
    robo.x = 0;
    robo.y = 0;
    robo.dir = 0;
    robo.r = ROBO_R;
}

void Initialize()
{
    make_circle();//円図形データの作成
    robo_init();
}

void robo_draw(void)
{
    draw_circle( robo.x, robo.y, robo.r );
}

void wall_draw(void)
{
    int i;

    glBegin(GL_LINES);
    for(i=0;i<WALLS;i++)
    {
        glVertex2d(pin[wall[i].p1].x, pin[wall[i].p1].y);
        glVertex2d(pin[wall[i].p2].x, pin[wall[i].p2].y);
    }
    glEnd();
}




void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    graphics();
    robo_draw();
    wall_draw();
    glutSwapBuffers();
}


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
//    glutInitWindowSize(510,510);
    graphics();
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutMouseFunc(mouse);//マウスのボタンを検出
    Initialize();
    glutMainLoop();
    return 0;
}