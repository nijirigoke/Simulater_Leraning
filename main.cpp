//
// Created by T118029 on 2021/03/15.
//

#include <iostream>

#define ROBO_R 5

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

void robo_draw(void)
{
    draw_circle( robo.x, robo.y, robo.r );
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    graphics();
    robo_draw();
    glutSwapBuffers();
}


int main(int argc, char const *argv[]) {
    /* code */
    std::cout << "hello";

    return 0;
}
