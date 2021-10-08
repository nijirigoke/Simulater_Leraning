//#pragma GCC optimize("Ofast")
//#pragma once
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
//
// Created by T118029 on 2021/03/15.
//

//todo 目標 通信の実装。通信の可視化。センサレンジの可視化。通信波及の可視化。

#include <GL/glut.h>
#include <iostream>
#include "simbase_test.h"
//#include "workspace_test.h"
#include <random>
#include <vector>
#define LEFT   0
#define CENTER 1
#define RIGHT  2
#define TRANGE 1.5 //タッチセンサーのレンジ 半径の倍数
#define RANGE 20 //通信レンジ の半径
#define SENS_RANGE 50 //通信レンジ の半径
#define TEST 10

#define RIGHT_TURN -0.1        //右回転 0.1ラジアンの定義
#define LEFT_TURN    0.1        //左回転 0.1ラジアンの定義
#define ROBOS  1000 //ロボット台数　10台

int glidline=2*point/RANGE;
std::random_device rnd;     // 非決定的な乱数生成器
std::mt19937 mt(rnd());
std::vector<std::vector<int>>glid(glidline,std::vector<int>(glidline,0));// 2*point/RANGE=仕切りの数


typedef struct ROBO {
    double r{};
    double x{}, y{};
    double dir{};

    //信号出力フラグ群
    int receive_flag{};
    int sens_flag{};

    //反応拡散用パラメータ群
    double activator = 0;
    double inhibitor = 0;
    double du = 0.08;
    double dv = 0.48;
    double Cu = 0.0001;
    double Cv = 0.0000;
    double a = 0.01;
    double b = 0.011;
    double c = 0.008;
    double d = 0.009;


    POSITION tsensor[3]{}; //構造体変数の追加
public:
    void draw();

    void action();

    void init();

    void turn(double q);

    void forward(double v);

    int touchsensor(int i);

    double nearrobotsensor();

    static int check_cross_wall(POSITION p1, POSITION p2);

    static int check_cross_others(POSITION p);

//    int SearchRobot(POSITION p, double range);

    int stack = 0;

    int flash_memori = 0;

    double sens_nearrobotsensor();
} ROBO;

ROBO robo[ROBOS];    //要素数ROBOSで配列変数roboを定義

void wall_draw();

void Initialize();

void wall_draw() {

    glBegin(GL_LINES);
    for (auto &i : wall) {
        glVertex2d(pin[i.p1].x, pin[i.p1].y);
        glVertex2d(pin[i.p2].x, pin[i.p2].y);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    graphics();
    wall_draw();
    for (auto &i : robo) {
        i.draw();
    }

    glutSwapBuffers();
}

void ROBO::forward(double v) {
    x = x + cos(dir) * v;
    y = y + sin(dir) * v;
}

void ROBO::turn(double q) {
    dir += q;
}

void ROBO::action() {
//    nearrobotsensor();


    std::uniform_int_distribution<int> distr(0, TEST);    // 非決定的な乱数生成器
    int tCenter, tRight, tLeft;

    tCenter = touchsensor(CENTER);    //中央センサーの値
    tRight = touchsensor(RIGHT);        //右センサーの値
    tLeft = touchsensor(LEFT);        //左センサーの値
    nearrobotsensor();

    if (stack < 25) {
        if (tLeft == 1)        //左チセンサ反応あり
        {
            turn(RIGHT_TURN);
            stack++;
        } else if (tRight == 1)    //右センサ反応あり
        {
            turn(LEFT_TURN);
            stack++;
        } else if (tCenter == 1) //正面センサ反応あり
        {
            turn(RIGHT_TURN);
            stack++;
        } else    //いずれの条件も当てはまらないのは全てのタッチセンサが０のとき
        {
            forward(0.5);//前進1.0ステップ
            stack = 0;
        }
    } else {
        turn(0.4 * PI);
        stack = 0;
    }


//std::cout << flash_memori << std::endl;
//std::cout << receive_flag << std::endl;

}


void idle() {
    if (fStart == 0) return;
    for (auto &i : robo) i.action();
//    Sleep(1 * 100);
    display();
}

void mouse(int button, int state, int x, int y) //マウスボタンの処理
{
    if (state == GLUT_DOWN) { //ボタンが押されたら...
        if (fStart == 1)
            fStart = 0;
        else
            fStart = 1;
        for (auto &i : robo) {
            i.init();
        }
    }
}

void ROBO::init() {

    std::uniform_int_distribution<int> distr(-point, point);    // 非決定的な乱数生成器
    std::uniform_real_distribution<> dir_gen(0, 360);
    std::uniform_real_distribution<> rando(0.0, 1.0);

    activator = rando(mt);
    inhibitor = rando(mt);


    dir = dir_gen(mt);
//    printf("%f", dir);
    x = distr(mt);
    y = distr(mt);
    r = 10;

    tsensor[CENTER].x = TRANGE * r; //タッチセンサーのレンジ（棒の長さ）
    tsensor[CENTER].y = 0;
    tsensor[LEFT].x = TRANGE * r * cos(60.0 / 180.0 * PI); //正面が０度なので、左は６０度
    tsensor[LEFT].y = TRANGE * r * sin(60.0 / 180.0 * PI);
    tsensor[RIGHT].x = TRANGE * r * cos(-60.0 / 180.0 * PI); //正面が０度なので、右は－６０度
    tsensor[RIGHT].y = TRANGE * r * sin(-60.0 / 180.0 * PI);
}

void ROBO::draw() {
    glPushMatrix(); //現在の座標系の保存
    glTranslated(x, y, 0);              //ロボットの現在座標へ座標系をずらす
    glRotated(dir / PI * 180, 0, 0, 1); //進行方向へZ軸回転

//    double dx = activator * a - inhibitor * b + Cu;
//    double dy = activator * c - inhibitor * d + Cv;

//    activator = inhibitor + dx;
//    activator = inhibitor + dy;

    glColor3d(activator, inhibitor, 1 - 0.5 * (activator + inhibitor));
    draw_robo_circle(0, 0, r);

    draw_circle(0, 0, r); //本体外形円の描画　現在の座標系の原点に対して描くことに注意
    glColor3d(0.5, 0.5, 0.5);
    draw_circle(0, 0, RANGE); //通信範囲の描画
    glColor3d(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2d(0, 0); //左センサーの描画
    glVertex2d(tsensor[LEFT].x, tsensor[LEFT].y);
    glVertex2d(0, 0); //正面センサーの描画
    glVertex2d(tsensor[CENTER].x, tsensor[CENTER].y);
    glVertex2d(0, 0); //右センサーの描画
    glVertex2d(tsensor[RIGHT].x, tsensor[RIGHT].y);
    glEnd();
    glPopMatrix(); //保存ておいた座標系へ戻す
}

//接触センサー関数 戻り値に　接触状態を１　非接触状態を０　返す
int ROBO::touchsensor(int i)
//構造体ROBOに所属している関数なので関数名の前に“ROBO::”とついている。
{
    int fw = 0;
    int fo = 0; //他のロボットとの接触フラグ
    POSITION p1, p2;
    // p1はロボットの中心座標
    p1.x = x; //ロボットの中心座標　この関数は構造体の中の関数なので、ｘは構造体ROBOの中のｘを意味する。
    p1.y = y; //同様にｙ

    // p2はセンサーの座標
    //まずロボット本体からの相対座標系で計算
    p2 = tsensor[i];      // i番のセンサーの座標を代入(p2は相対座標)
    //空間に対する進行方向を計算
    p2 = rotate(p2, dir); //ロボットの進行方向に回転（p2は絶対方向、相対位置）
    //空間に対する位置を計算
    p2.x += x;            //ロボットの中心座標を足す（p2は絶対座標）
    p2.y += y;

    //ロボット中心からセンサーまでの線分p1,p2と壁の交差チェック
    fw = check_cross_wall(p1, p2);

    //ここに他のロボットの検出判定処理を入れる
    fo = check_cross_others(p2); //センサーの先端位置

    if (fw == 1 || fo == 1) //どちらかのフラグが１だったらセンサー反応あり
    {
        return 1;
    }
    return 0; //センサー反応なし
}

int ROBO::check_cross_wall(POSITION p1, POSITION p2) {

    for (auto &i : wall) {
        double Wp1x, Wp1y, Wp2x, Wp2y; //線分の両端点
        double Bvx, Bvy;               //線分への垂線の方向ベクトル
        double Bx, By, R;              //球体の位置, 半径
        double det, a, b, c, d, e, f;  //ベクトルの式の変数

        //接触判定
        Bx = p1.x;
        By = p1.y;

        Wp1x = pin[i.p1].x;
        Wp1y = pin[i.p1].y;
        Wp2x = pin[i.p2].x;
        Wp2y = pin[i.p2].y;

        Bvx = (p2.x - p1.x);
        Bvy = (p2.y - p1.y);

        a = Wp2x - Wp1x;
        b = -Bvx;
        c = Wp2y - Wp1y;
        d = -Bvy;
        e = Bx - Wp1x;
        f = By - Wp1y;
        det = a * d - b * c;

        if (det != 0) {
            double t, s; //接触条件処理の条件変数
            s = 1.0 / det * (d * e - b * f);
            t = 1.0 / det * (-c * e + a * f);
            if (0 < t && t < 1 && 0 < s && s < 1) { //交差あり
                return 1;
            }
        }
    }
    return 0; //交差なし
}

int ROBO::check_cross_others(POSITION p) {
    double l;
    double sensor_x;
    double sensor_y;
// 各ロボの座標 for:
// 自他の区別 ？？？
// 区別したらそいつとの距離を産出すること
    sensor_x = p.x;
    sensor_y = p.y;
    for (auto &i : robo) {
        double another_robo_x = i.x;
        double another_robo_y = i.y;
        double distance_x;
        double distance_y;

        distance_x = another_robo_x - sensor_x;
        distance_y = another_robo_y - sensor_y;

        l = sqrt(pow(distance_x, 2) + pow(distance_y, 2));

        if (l < i.r) {
            return 1;
        }
    }
    return 0;
}

double ROBO::nearrobotsensor() {
    double l;
    double distance_x;
    double distance_y;

    //自身の座標から一定レンジを探索？ 計算量ちゃん…。
    for (auto &i : robo) {

        //他のロボットのちゃん座標
        double another_robo_x = i.x;
        double another_robo_y = i.y;

        distance_x = another_robo_x - x;
        distance_y = another_robo_y - y;

        l = sqrt(pow(distance_x, 2) + pow(distance_y, 2));
        if (l < RANGE) {
//            printf("%f\n::%d",l,RANGE);
//            i.receive_flag = 1;
            double tx;
            double ty;

            tx = du * (activator - inhibitor);
            ty = dv * (activator - inhibitor);
            activator = activator - tx;
            i.activator = i.activator + tx;
            inhibitor = inhibitor - ty;
            i.inhibitor = i.inhibitor + ty;
        }
    }
}


int main(int argc, char *argv[]) {

    for (int i = 0; i < glidline; ++i) {
        for (int j = 0; j < glidline; ++j) {
            glid[i][j]=
        }
    }

    Initialize();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(point, point);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutIdleFunc(idle);
    glutMouseFunc(mouse); //マウスのボタンを検出
    glutMainLoop();


    return 0;
}

void Initialize() {
    make_circle();//円図形データの作成
    for (auto &i : robo) i.init();
}
