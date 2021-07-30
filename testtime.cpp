#include <iostream>
#include <cmath>
//#include <cstdio>
#include <GL/glut.h>


struct object {
    double x, y; //位置
    double V_x, V_y; //速度
    double Vo_x, Vo_y; //初速
};
typedef struct object OBJCT;


void display();

void reshape(int w, int h);

void idle(void);

void DRAW_XYZ();

using namespace std;


inline void GLUT_INIT() {
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(200, 200);
}

inline void GLUT_CALL_FUNC() {
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutIdleFunc(idle);
}

inline void MY_INIT() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
}


OBJCT obj;

inline void SET_OBJ_DATA() {
#define Vo 70
#define g 9.8
#define theta 45
#define PI_OVER_180 0.0174532925


    obj.Vo_x = Vo * cos(theta * PI_OVER_180); //X成分の初速計算
    obj.Vo_y = Vo * sin(theta * PI_OVER_180); //Y成分の初速計算

}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    GLUT_INIT();
    glutCreateWindow("window name");
    GLUT_CALL_FUNC();
    MY_INIT();
    SET_OBJ_DATA();

    glutMainLoop();

    return 0;
}


/********[ここから各種コールバック]****************************************/

void display() {
    static double t = 0;

    if (!(obj.x > 0 && obj.y < 0)) {
        obj.x = obj.Vo_x * t;
        obj.y = obj.Vo_y * t - 0.5 * g * t * t;

        if (obj.y < 0) {
            printf("t : %f\n", t);
            printf("x : %f\n", obj.x);
        }


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();

        gluLookAt(30.0, 30.0, 60.0, 15, 10.0, 0.0, 0.0, 1.0, 0.0);


        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        DRAW_XYZ();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        glPushMatrix();
        glTranslated(obj.x / 20, obj.y / 20, 0);
        glutSolidCube(1);
        glPopMatrix();


        glDisable(GL_DEPTH_TEST);

        glutSwapBuffers();
        t += 0.01;
    } else {
        t = 0;
        obj.x = 0, obj.y = 0;
    }


}


void reshape(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (double) w / (double) h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


void idle(void) {
    glutPostRedisplay();
}


void DRAW_XYZ() {
    glBegin(GL_LINES);

    glColor3d(0, 1, 0);//x
    glVertex2d(-100, 0);
    glVertex2d(100, 0);

    glColor3d(1, 0, 0);//y
    glVertex2d(0, 0);
    glVertex2d(0, 100);

    glColor3d(0, 0, 1);//z
    glVertex3d(0, 0, -100);
    glVertex3d(0, 0, 100);
    glEnd();

}