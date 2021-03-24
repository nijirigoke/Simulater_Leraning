#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

typedef struct {//座標を表す構造体
    double x;
    double y;
} POSITION;

void display(void);
void resize(int w, int h);
void idle(void);
void mouse(int button, int state, int x,int y);
void simstart(void);
void init(void);
void graphics(void);
void simstep(void);
void TypeStr(int x, int y, char str[]);

double WindowScale = 1;//画面の広角率（大きくなるほど広い範囲を描画）　１:１ピクセル→１　10:1ピクセル→10
int fStart=0;	//シミュレーションの開始フラグ

void TypeStr(int x, int y, char *str)//文字の表示
{
    glRasterPos2i(x,y);
    while(*str!=0)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,(int)*str++);
}

#define PI 3.141592657
#define CIRCLEDIV 24	//円図形　24角形
POSITION circlepos[CIRCLEDIV];	//円図形データ

void make_circle()//円図形のデータ生成
{
    int i;
    for(i=0;i<CIRCLEDIV;i++){
        circlepos[i].x=cos(2.0*PI*(double)i/(double)CIRCLEDIV);
        circlepos[i].y=sin(2.0*PI*(double)i/(double)CIRCLEDIV);
    }
}

void draw_circle( double x, double y, double r )//円図形の描画(座標p, 半径r)
{
    int i;
    glPushMatrix();
    glTranslated(x, y, 0);
    glBegin(GL_LINE_LOOP);
    for(i=0;i<CIRCLEDIV;i++)glVertex2d(circlepos[i].x*r, circlepos[i].y*r);
    glEnd();
    glPopMatrix();
}

//void mouse(int button, int state, int x,int y)//マウスボタンの処理
//{
//    if(state==GLUT_DOWN){//ボタンが押されたら...
//        init();//初期化
//        fStart=1;
//    }
//}

//void Initialize()//プログラム実行前の初期化
//{
//    make_circle();//円図形データの作成
//    init();//その他初期化
//}

//int main(int argc, char* argv[])
//{
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
//    glutCreateWindow(argv[0]);
//    glutDisplayFunc(display);
//    glutReshapeFunc(resize);
//    glutMouseFunc(mouse);//マウスのボタンを検出
//    glutIdleFunc(idle);
//    glClearColor(0,0,0,0);
//    Initialize();
//    glutMainLoop();
//    return 0;
//}

void resize(int w, int h)
{
glViewport(0,0,w,h);
glLoadIdentity();
glOrtho( -255, 255, -255, 255, -1.0, 1.0 );

}

//void display()
//{
//    glClear(GL_COLOR_BUFFER_BIT);
//    graphics();
//    glutSwapBuffers();
//}

//void idle()
//{
//    if(fStart==0)return;
//    simstep();
//    display();
//}

//ここ以降に主要プログラムを書く


//void init() //シミュレーションの初期条件
//{
//
//}
//
//void simstep()//シミュレーションのステップの計算
//{
//
//}
//
void graphics()//シミュレーション結果（状態）の表示
{
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(510,510);
}

