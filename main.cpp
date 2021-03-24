//
// Created by T118029 on 2021/03/15.
//

#include <iostream>
#include <GL/glut.h>
//#include "simbase.h"
#include "workspace.h"

#define LEFT 0
#define CENTER 1
#define RIGHT 2
#define TRANGE 1.5 //タッチセンサーのレンジ 半径の倍数

#define ROBO_R 15

void robo_init();

void robo_draw();

void Initialize();

void resize(int w, int h);

void wall_draw();

int check_cross_wall(POSITION p1, POSITION p2);

int check_cross_others(POSITION p);

typedef struct {
    double r;/*本体の半径*/
    double x, y;/*本体の位置*/
    double dir;/*本体の進行方向*/
    POSITION tsensor[3];//構造体変数の追加
//    この宣言は、ロボットのパラメータの追加なので、構造体ROBOに追加する。
public:
    void draw();

    void action();

    void init();

    void turn(double q);

    void forward(double v);

    int touchsensor(int i);
} ROBO;


ROBO robo;

void robo_init() {
    robo.x = 0;
    robo.y = 0;
    robo.dir = 0;
    robo.r = ROBO_R;
}

void Initialize() {
    make_circle();//円図形データの作成
    robo_init();
}

void robo_draw(void) {
    double dx, dy;
    dx = cos(robo.dir) * robo.r * 2;
    dy = sin(robo.dir) * robo.r * 2;
    glBegin(GL_LINES);
    glVertex2d(robo.x, robo.y);
    glVertex2d(robo.x + dx, robo.y + dy);
    glEnd();
    draw_circle(robo.x, robo.y, robo.r);

}

void wall_draw(void) {
    int i;

    glBegin(GL_LINES);
    for (i = 0; i < WALLS; i++) {
        glVertex2d(pin[wall[i].p1].x, pin[wall[i].p1].y);
        glVertex2d(pin[wall[i].p2].x, pin[wall[i].p2].y);
    }
    glEnd();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    graphics();
    robo_draw();
    wall_draw();
    glutSwapBuffers();
}

void robo_forward(double v) {
    robo.x = robo.x + cos(robo.dir) * v;
    robo.y = robo.y + sin(robo.dir) * v;
}

void robo_turn(double q) {
    robo.dir += q;
}

void robo_action(void) {
    robo_turn(0.1);
    robo_forward(1.0);

}

void idle(void) {
    if (fStart == 0) return;
    robo_action();
    display();
}

void mouse(int button, int state, int x, int y)//マウスボタンの処理
{
    if (state == GLUT_DOWN) {//ボタンが押されたら...
        if (fStart == 1)fStart = 0;
        else fStart = 1;
        robo_init();
    }
}

void ROBO::init() {
    dir = rand() * 360.0;
    x = 250;
    y = 250;
    r = 10;
    tsensor[CENTER].x = TRANGE * r;//タッチセンサーのレンジ（棒の長さ）
    tsensor[CENTER].y = 0;
    tsensor[LEFT].x = TRANGE * r * cos(60.0 / 180.0 * PI);//正面が０度なので、左は６０度
    tsensor[LEFT].y = TRANGE * r * sin(60.0 / 180.0 * PI);
    tsensor[RIGHT].x = TRANGE * r * cos(-60.0 / 180.0 * PI);//正面が０度なので、右は－６０度
    tsensor[RIGHT].y = TRANGE * r * sin(-60.0 / 180.0 * PI);
}


void ROBO::draw(void) {
    glPushMatrix();    //現在の座標系の保存

    glTranslated(x, y, 0);        //ロボットの現在座標へ座標系をずらす
    glRotated(dir / PI * 180, 0, 0, 1);    //進行方向へZ軸回転
    draw_circle(0, 0, r);            //本体外形円の描画　現在の座標系の原点に対して描くことに注意

    glBegin(GL_LINES);
    glVertex2d(0, 0);        //左センサーの描画
    glVertex2d(tsensor[LEFT].x, tsensor[LEFT].y);
    glVertex2d(0, 0);        //正面センサーの描画
    glVertex2d(tsensor[CENTER].x, tsensor[CENTER].y);
    glVertex2d(0, 0);        //右センサーの描画
    glVertex2d(tsensor[RIGHT].x, tsensor[RIGHT].y);
    glEnd();

    glPopMatrix();    //保存ておいた座標系へ戻す
}

//回転変換 点ｐをdir回転した座標を戻り値とする
POSITION rotate(POSITION p, double dir) {
    POSITION P;
    P.x = p.x * cos(dir) - p.y * sin(dir);
    P.y = p.x * sin(dir) + p.y * cos(dir);
    return P;
}


//接触センサー関数 戻り値に　接触状態を１　非接触状態を０　返す
int ROBO::touchsensor(int i)//構造体ROBOに所属している関数なので関数名の前に“ROBO::”とついている。
{
    int fw;
    int fo;//他のロボットとの接触フラグ
    POSITION p1, p2;
//p1はロボットの中心座標
    p1.x = x;//ロボットの中心座標　この関数は構造体の中の関数なので、ｘは構造体ROBOの中のｘを意味する。
    p1.y = y;//同様にｙ

//p2はセンサーの座標
//まずロボット本体からの相対座標系で計算
    p2 = tsensor[i];//i番のセンサーの座標を代入(p2は相対座標)
//空間に対する進行方向を計算
    p2 = rotate(p2, dir);//ロボットの進行方向に回転（p2は絶対方向、相対位置）
//空間に対する位置を計算
    p2.x += x;//ロボットの中心座標を足す（p2は絶対座標）
    p2.y += y;

//ロボット中心からセンサーまでの線分p1,p2と壁の交差チェック
    fw = check_cross_wall(p1, p2);

//ここに他のロボットの検出判定処理を入れる
    fo = check_cross_others(p2);//センサーの先端位置

    if (fw == 1 || fo == 1)//どちらかのフラグが１だったらセンサー反応あり
    {
        return 1;
    }
    return 0;//センサー反応なし
}

int check_cross_others(POSITION p) {

    //   作成せよ
    return 0;
}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(510, 510);
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