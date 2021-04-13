#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

typedef struct { //座標を表す構造体
    double x;
    double y;
} POSITION;

void display();

void resize(int w, int h);

void idle();

void mouse(int button, int state, int x, int y);

void simstart();

void init();

void graphics();

void simstep();

void TypeStr(int x, int y, char str[]);

double WindowScale = 1; //画面の広角率（大きくなるほど広い範囲を描画）　１:１ピクセル→１　10:1ピクセル→10
int fStart = 0; //シミュレーションの開始フラグ

void TypeStr(int x, int y, char *str) //文字の表示
{
    glRasterPos2i(x, y);
    glRasterPos2i(x, y);
    while (*str != 0)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (int) *str++);
}

#define PI        3.141592657
#define CIRCLEDIV 24 //円図形　24角形
POSITION circlepos[CIRCLEDIV]; //円図形データ
//回転変換 点ｐをdir回転した座標を戻り値とする
POSITION rotate(POSITION p, double dir) {
    POSITION P;
    P.x = p.x * cos(dir) - p.y * sin(dir);
    P.y = p.x * sin(dir) + p.y * cos(dir);
    return P;
}
void make_circle() //円図形のデータ生成
{
    int i;
    for (i = 0; i < CIRCLEDIV; i++) {
        circlepos[i].x = cos(2.0 * PI * (double) i / (double) CIRCLEDIV);
        circlepos[i].y = sin(2.0 * PI * (double) i / (double) CIRCLEDIV);
    }
}

void draw_circle(double x, double y, double r) //円図形の描画(座標p, 半径r)
{
    int i;
    glPushMatrix();
    glTranslated(x, y, 0);
    glBegin(GL_LINE_LOOP);
    for (i = 0; i < CIRCLEDIV; i++)
        glVertex2d(circlepos[i].x * r, circlepos[i].y * r);
    glEnd();
    glPopMatrix();
}

void resize(int w, int h) {
    glViewport(0, 0, w, h);
    glLoadIdentity();
    glOrtho(-255, 255, -255, 255, -1.0, 1.0);
}

void graphics() //シミュレーション結果（状態）の表示
{
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(510, 510);
}