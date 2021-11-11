#include <GL/glut.h>

#define MaxWindowNum 4

// WindowID
int WindowID[MaxWindowNum];

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void init(void) {
    glClearColor(0.0, 0.0, 1.0, 1.0);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);

    // 1st Window
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(0, 0);
    WindowID[1] = glutCreateWindow("Main Window");
    glutDisplayFunc(display);
    init();

    // 2nd Window
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(655, 0);
    WindowID[2] = glutCreateWindow("2nd Window");
    glutDisplayFunc(display);
    init();

    glutMainLoop();
    return 0;
}