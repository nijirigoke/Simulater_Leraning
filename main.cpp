//
// Created by T118029 on 2021/03/15.
//

#include <iostream>
#include <GL/glut.h>
//#include "simbase.h"
#include "workspace.h"

#define ROBO_R 15

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
    double dx, dy;
    dx = cos(robo.dir)*robo.r*2;
    dy = sin(robo.dir)*robo.r*2;
    glBegin(GL_LINES);
    glVertex2d( robo.x, robo.y );
    glVertex2d( robo.x+dx, robo.y+dy );
    glEnd();
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

void robo_forward( double v )
{
    robo.x = robo.x + cos(robo.dir)* v;
    robo.y = robo.y + sin(robo.dir)* v;
}

void robo_turn( double q )
{
    robo.dir += q;
}

void robo_action( void )
{
    robo_turn( 0.1 );
    robo_forward( 1.0 );

}

void idle(void)
{
    if(fStart==0) return;
    robo_action( );
    display( );
}

void mouse(int button, int state, int x,int y)//マウスボタンの処理
{
    if(state==GLUT_DOWN)
    {//ボタンが押されたら...
        if(fStart==1)fStart = 0;
        else fStart = 1;
        robo_init();
    }
}





int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(510,510);
    graphics();
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);//マウスのボタンを検出
    Initialize();
    glutMainLoop();



    return 0;
}