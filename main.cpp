#pragma GCC optimize("O3")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
//
// Created by T118029 on 2021/03/15.
//

#include <GL/glut.h>
#include <GL/freeglut.h>
#include <iostream>
#include "simbase.h"
#include <random>
#include <omp.h>
#include <fstream>

#define LEFT   0
#define CENTER 1
#define RIGHT  2
#define TRANGE 1.5 //タッチセンサーのレンジ 半径の倍数
#define RANGE 50 //通信レンジ の半径
#define INHIBITOR_RANGE 70
#define RIGHT_TURN -0.1        //右回転 0.1ラジアンの定義
#define LEFT_TURN    0.1        //左回転 0.1ラジアンの定義
#define ROBOS  1500 //ロボット台数　10台
#define N 5000
#define MAPDENSITY 80
#define FORWARD 0.6
#define RADIUS 10

double input_concentration[15][15] = {

        {1,       0.5,     0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 1,       0.00001, 0.00001, 0.00001, 0.00001, 0.5,     1},
        {1,       0.5,     0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 1,       1,       0.00001, 0.00001, 0.00001, 1,       1},
        {1,       0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 1,       1,       1,       1,       0.00001, 0.00001, 0.00001, 1,       1},
        {0.5,     0.00001, 0.00001, 0.00001, 1,       1,       1,       1,       1,       1,       0.00001, 0.00001, 0.5,     1,       1},
        {0.00001, 0.00001, 0.00001, 1,       1,       1,       1,       1,       1,       1,       0.00001, 0.00001, 0.00001, 1,       0.5},

        {0.00001, 0.00001, 1,       1,       1,       1,       1,       1,       1,       0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 0.00001},
        {0.00001, 1,       1,       1,       1,       0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 0.00001},
        {0.00001, 1,       1,       1,       1,       0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 0.00001, 0.00001},
        {0.00001, 0.5,     1,       1,       0.00001, 0.00001, 0.00001, 1,       0.5,     1,       0.00001, 0.00001, 0.00001, 0.5,     1},
        {0.00001, 0.5,     1,       1,       0.5,     0.00001, 0.00001, 1,       1,       1,       1,       1,       1,       1,       1,},

        {0.00001, 0.00001, 1,       1,       1,       0.00001, 0.00001, 1,       1,       1,       1,       1,       1,       1,       0.00001},
        {0.00001, 0.5,     1,       1,       1,       0.00001, 0.5,     1,       1,       0.00001, 0.5,     1,       1,       0.00001, 0.00001},
        {0.00001, 1,       1,       1,       1,       0.5,     1,       1,       0.00001, 0.00001, 0.00001, 0.5,     1,       0.00001, 0.5},
        {0.5,     0.5,     0.00001, 0.00001, 1,       1,       1,       0.00001, 0.00001, 0.00001, 0.00001, 1,       1,       1,       1},
        {0.5,     0.00001, 0.00001, 0.00001, 1,       1,       1,       0.5,     0.00001, 0.00001, 0.00001, 0.00001, 1,       1,       1}
};



/**
 grid_memo.md

 | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | .5 |  |  |  | 1 | 1 | 1 | .5 |  |  |  |  | 1 | 1 | 1 |
| 2 | .5 | .5 |  |  | 1 | 1 | 1 |  |  |  |  | 1 | 1 | 1 | 1 |
| 3 |  | 1 | 1 | 1 | 1 | .5 | 1 | 1 |  |  |  | .5 | 1 |  | .5 |
| 4 |  | .5 | 1 | 1 | 1 |  | .5 | 1 | 1 |  | .5 | 1 | 1 |  |  |
| 5 |  |  | 1 | 1 | 1 |  |  | 1 | 1 | 1 | 1 | 1 | 1 | 1 |  |
| 6 |  | .5 | 1 | 1 | .5 |  |  | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 |
| 7 |  | .5 | 1 | 1 |  |  |  | 1 | .5 | 1 |  |  |  | .5 | 1 |
| 8 |  | 1 | 1 | 1 | 1 |  |  |  |  |  |  |  |  |  |  |
| 9 |  | 1 | 1 | 1 | 1 |  |  |  |  |  |  |  |  |  |  |
| 10 |  |  | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 |  |  |  |  |  |
| 11 |  |  |  | 1 | 1 | 1 | 1 | 1 | 1 | 1 |  |  |  | 1 | .5 |
| 12 | .5 |  |  |  | 1 | 1 | 1 | 1 | 1 | 1 |  |  | .5 | 1 | 1 |
| 13 | 1 |  |  |  |  |  | 1 | 1 | 1 | 1 |  |  |  | 1 | 1 |
| 14 | 1 | .5 |  |  |  |  |  | 1 | 1 |  |  |  |  | 1 | 1 |
| 15 | 1 | .5 |  |  |  |  |  |  | 1 |  |  |  |  | .5 | 1 |

 */

using namespace std;

struct GLID_STRUCT {
    double ave_activator;
    double ave_inhibitor;
};

typedef struct ROBO {
    int step_counter = 0;

    double r{};
    double x{}, y{};
    double dir{};

    int glid_x = 0;
    int glid_y = 0;

    int map_glid_x = 0;
    int map_glid_y = 0;

    int tCenter = 0;
    int tRight = 0;
    int tLeft = 0;

    //信号出力フラグ群
    int act_flag{};
    int inh_flag{};

    //反応拡散用パラメータ群
    double activator = 0;
    double inhibitor = 0;
    double sum_activator = 0;
    double sum_inhibitor = 0;
    int touch_counter = 0;
    int act_touch_counter = 0;
    int inh_touch_counter = 0;

    double dx = 0;
    double dy = 0;

    double dv = 0.40;
    double du = 0.08;
    double Cv = 0.0000;
    double Cu = 0.00010;
    double a = 0.010;
    double b = 0.011;
    double c = 0.008;
    double d = 0.009;

//    double du = 0.000;
//    double dv = 0.000;
//    double Cu = 0.000000;
//    double Cv = 0.0000;
//    double a = 0.000;
//    double b = 0.000;
//    double c = 0.0000;
//    double d = 0.0000;

//
//    double dv = 0.40;
//    double du = 0.08;
//    double Cv = 0.0000;
//    double Cu = 0.0001;
//    double a = 0.010;
//    double b = 0.02;
//    double c = 0.010;
//    double d = 0.015;


    POSITION tsensor[3]{}; //構造体変数の追加
public:
    void draw();

    void action();

    void init();

    void turn(double q);

    void forward(double v);

    int touchsensor(int i);

    static int check_cross_wall(POSITION p1, POSITION p2);

    int check_cross_others(POSITION p);

    int stack = 0;

    void nearrobotsensor();

} ROBO;

ROBO robo[ROBOS];//要素数ROBOSで配列変数roboを定義

int epoch = 0;
// 通信可能性のあるロボットを効率的に探すためのグリッド切り
int gridline = (int) point * 2 / INHIBITOR_RANGE;
int half_gridline = gridline / 2;
// ビジュアライズしたマップのためのグリッド切り
int map_gridline = (int) point * 2 / MAPDENSITY;
int half_map_gridline = map_gridline / 2;

int windows[2];

double save_grid_activater[100000][225] = {};
double save_robot_log[50][10000] = {};
double save_autocorr[N][225] = {};
double save_autocorr_fin[N] = {};

std::random_device rnd;     // 非決定的な乱数生成器
std::mt19937 mt(rnd());
GLID_STRUCT GL[100][100];
POSITION ROBOTLOG[50][10000];

double input_ave = 0;

void calculate_grid_concentration();

void draw_grid_density_map();

void grid_display();

void wall_draw();

void Initialize();

void input_turingpattern();

void calculate_grid_concentration_memory();

void save_grid_concentration();

double covariance();

double calculate_input_ave();

double calculate_GL_ave();

double stdd_now();

double stdd_target();

void calculate_autocorr();

void save_robot_loging();

void wall_draw() {
    glBegin(GL_LINES);
    for (auto &i: wall) {
        glVertex2d(pin[i.p1].x, pin[i.p1].y);
        glVertex2d(pin[i.p2].x, pin[i.p2].y);
//        cout << pin[i.p1].x << pin[i.p1].y << endl;
    }
    glEnd();
}

void grid_wall_draw() {

    glBegin(GL_LINES);
    glColor3d(1, 1, 1);

    for (auto &i: wall) {
        glVertex2d(pin[i.p1].x, pin[i.p1].y);
        glVertex2d(pin[i.p2].x, pin[i.p2].y);
//        cout << pin[i.p1].x << pin[i.p1].y << endl;
    }
    glEnd();

    glBegin(GL_LINES);
    // tate
    for (int j = 0; j < map_gridline; ++j) {
        glVertex2d(-point + (j * MAPDENSITY), point);
        glVertex2d(-point + (j * MAPDENSITY), -point);
    }

    // yoko

    for (int j = 0; j < map_gridline; ++j) {
        glVertex2d(point, point - (j * MAPDENSITY));
        glVertex2d(-point, point - (j * MAPDENSITY));
    }
    glEnd();
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);
    graphics();
    wall_draw();
    for (auto &i: robo) {
        i.draw();
    }
    glutSwapBuffers();
}

void grid_display() {
    glClear(GL_COLOR_BUFFER_BIT);
    graphics();
    draw_grid_density_map();
    grid_wall_draw();

    glutSwapBuffers();

}

void calculate_grid_concentration() {

    // 初期化
    double sum_glid_activator;
    double sum_glid_inhibitor;

    for (int x = 0; x < map_gridline; ++x) {
        for (int y = 0; y < map_gridline; ++y) {// y point
            GL[x][y].ave_activator = 0;
            GL[x][y].ave_inhibitor = 0;
        }
    }
    // 初期化 終わり

    for (int x = 0; x < map_gridline; ++x) {
        for (int y = 0; y < map_gridline; ++y) {// y point
            int counter = 0;
            //配列を初期化すべし
            sum_glid_activator = 0;
            sum_glid_inhibitor = 0;
            for (int i = 0; i < ROBOS; i++) {// i point
//                cout << x << ";" << robo[i].map_glid_x <<  ";" << y << ";"<< robo[i].map_glid_y<< endl;
                if (x == robo[i].map_glid_x && y == robo[i].map_glid_y) {
                    counter++;
                    sum_glid_activator += robo[i].activator;
                    sum_glid_inhibitor += robo[i].inhibitor;
                }
            }
            GL[x][y].ave_activator = sum_glid_activator / counter;
            GL[x][y].ave_inhibitor = sum_glid_inhibitor / counter;
        }
    }

    cout << "epoch," << epoch << endl;
//         << robo[0].inhibitor << "," << robo[0].sum_activator << endl;
//    cout << GL[15][15].ave_activator<<","<<GL[15][15].ave_inhibitor<<endl;
//    cout <<epoch<<"," <<robo[0].activator << "," << robo[0].inhibitor << endl;
}

void draw_grid_density_map() {

    for (int i = 0; i < map_gridline; ++i) {
        for (int j = 0; j < map_gridline; ++j) {
            glBegin(GL_TRIANGLE_STRIP);


            glColor3d(GL[i][j].ave_activator, GL[i][j].ave_inhibitor,
                      1 - 0.5 * (GL[i][j].ave_activator + GL[i][j].ave_inhibitor));

            glColor3d(GL[i][j].ave_activator, 0, 0);
            glVertex2d(-point + (MAPDENSITY * i), -point + (MAPDENSITY * j));
            glVertex2d(-point + (MAPDENSITY * (i + 1)), -point + (MAPDENSITY * j));
            glVertex2d(-point + (MAPDENSITY * i), -point + (MAPDENSITY * (j + 1) * 1));
            glVertex2d(-point + (MAPDENSITY * (i + 1)), -point + (MAPDENSITY * (j + 1) * 1));
            glEnd();

        }
    }
}

void ROBO::forward(double v) {
    x = x + cos(dir) * v;
    y = y + sin(dir) * v;
}

void ROBO::turn(double q) {
    dir += q;
}

void ROBO::action() {
    step_counter++;

    glid_x = (int) half_gridline + floor(x) / (2 * INHIBITOR_RANGE);//グリッドぎり
    glid_y = (int) half_gridline + floor(y) / (2 * INHIBITOR_RANGE);

    map_glid_x = half_map_gridline + floor(x) / (MAPDENSITY);
    map_glid_y = half_map_gridline + floor(y) / (MAPDENSITY);
//    cout<<map_glid_x<<endl;
//    cout<<map_glid_y<<endl;

//    std::uniform_int_distribution<int> distr(0, TEST);    // 非決定的な乱数生成器

    dx = activator * a - inhibitor * b + Cu;
    dy = activator * c - inhibitor * d + Cv;

//    cout<<dx<<endl;
    activator = activator + dx;
    inhibitor = inhibitor + dy;
    if (activator > 1) {
        activator = 1;
    }
    if (activator < 0) {
        activator = 0;
    }
    nearrobotsensor();

    tCenter = touchsensor(CENTER);    //中央センサーの値
    tRight = touchsensor(RIGHT);        //右センサーの値
    tLeft = touchsensor(LEFT);        //左センサーの値

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
            forward(FORWARD);//前進1.0ステップ
            stack = 0;
        }
    } else {
        turn(1 * PI);
        stack = 0;
    }

//std::cout << flash_memori << std::endl;
//std::cout << receive_flag << std::endl;

}

void ROBO::init() {

    std::uniform_int_distribution<int> distr(-point + 1, point - 1);    // 非決定的な乱数生成器
    std::uniform_real_distribution<> dir_gen(0, 360);
    std::uniform_real_distribution<> rando(0.0, 1.0);

    sum_activator = 0;
    sum_inhibitor = 0;
    touch_counter = 0;
    activator = rando(mt);
    inhibitor = rando(mt);

    dir = dir_gen(mt);
//    printf("%f", dir);
    x = distr(mt);
    y = distr(mt);
    r = RADIUS;

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

//    glColor3d(activator, inhibitor, 1 - 0.5 * (activator + inhibitor));
//    glColor3d(activator, inhibitor, 0);
    glColor3d(activator, 0, 0);
    draw_robo_circle(0, 0, r);
    glColor3d(0.5, 0.5, 0.5);
    draw_circle(0, 0, r); //本体外形円の描画　現在の座標系の原点に対して描くことに注意
//
//    glColor3d(0, 0, 0.5);
//    draw_circle(0, 0, RANGE); //通信範囲の描画

//    glColor3d(0, 0.5, 0);
//    draw_circle(0, 0, INHIBITOR_RANGE); //通信範囲の描画

//    glColor3d(0.3, 0.3, 0.3);
//    glBegin(GL_LINES);
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
int ROBO::touchsensor(int i) {
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

    for (auto &i: wall) {
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

    for (auto &i: robo) {
        if (
                (glid_x == i.glid_x || glid_x == i.glid_x + 1 || glid_x == i.glid_x - 1) &&
                (glid_y == i.glid_y || glid_y == i.glid_y + 1 || glid_y == i.glid_y - 1)
                ) {
            double another_robo_x = i.x;
            double another_robo_y = i.y;
            double distance_x;
            double distance_y;

            distance_x = another_robo_x - sensor_x;
            distance_y = another_robo_y - sensor_y;

            l = sqrt(distance_x * distance_x + distance_y * distance_y);
            if (l < i.r) {
                return 1;
            }
        }
    }
    return 0;
}

void ROBO::nearrobotsensor() {

    double l;
    double distance_x;
    double distance_y;
    double tmp;

    //自身の座標から一定レンジを探索？ 計算量ちゃん…。


    for (auto &i: robo) {
        if ((glid_x == i.glid_x || glid_x == i.glid_x + 1 || glid_x == i.glid_x - 1) &&
            (glid_y == i.glid_y || glid_y == i.glid_y + 1 || glid_y == i.glid_y - 1)) {

            //他のロボットのちゃん座標
            double another_robo_x = i.x;
            double another_robo_y = i.y;

            distance_x = another_robo_x - x;
            distance_y = another_robo_y - y;

            l = sqrt(distance_x * distance_x + distance_y * distance_y);


            if (l < RANGE) {
//                cout<<l<<endl;
//                cout<<epoch<<","<<l<<","<<RANGE<< endl;
                act_touch_counter++;
                i.act_touch_counter++;

                double tx;
                double ty;

                tx = du * (activator - i.activator);

                sum_activator += activator - tx;
                i.sum_activator += i.activator + tx;

                act_flag = 1;

            }
            if (l < INHIBITOR_RANGE) {
//                cout<<l<<endl;
//                cout<<epoch<<","<<l<<","<<RANGE<< endl;
                inh_touch_counter++;
                i.inh_touch_counter++;

                double tx;
                double ty;

                ty = dv * (inhibitor - i.inhibitor);

                sum_inhibitor += inhibitor - ty;
                i.sum_inhibitor += i.inhibitor + ty;
                inh_flag = 1;

            }

            if (step_counter >= 10) {
                if (act_flag == 1) {
//                    tmp = sum_activator / act_touch_counter;
                    activator = sum_activator / act_touch_counter;

                    act_flag = 0;
                }
                if (inh_flag == 1) {
                    inhibitor = sum_inhibitor / inh_touch_counter;
                    inh_flag = 0;
                }
                sum_activator = 0;
                sum_inhibitor = 0;
                touch_counter = 0;
                act_touch_counter = 0;
                inh_touch_counter = 0;
                step_counter = 0;
            }
        }
    }
}

void idle() {

    if (fStart == 0) return;

    for (auto &i: robo) i.action();

    if (epoch == 0) {
        input_turingpattern();
        input_ave = calculate_input_ave();
        cout << "input_ave," << input_ave << endl;
    }

    calculate_grid_concentration();
    save_grid_concentration();
    save_robot_loging();

    for (int i; i < 2; i++) {
        glutSetWindow(windows[i]);
        glutPostRedisplay();
    }

    epoch++;

    if (epoch > 10000) {
        glutLeaveMainLoop();
    }
//    cout << epoch << ";;;;";

}

void save_robot_loging() {
    double x = 0;
    double y = 0;

    for (int i = 0; i < 50; ++i) {
        ROBOTLOG[i][epoch].x = robo[i].x;
        ROBOTLOG[i][epoch].y = robo[i].y;

    }

}

void save_grid_concentration() {

    for (int x = 0; x < map_gridline; ++x) {
        for (int y = 0; y < map_gridline; ++y) {
            if (GL[x][y].ave_activator != GL[x][y].ave_activator) {
                save_grid_activater[epoch][x * (map_gridline) + y] = input_concentration[y][x];
                cout << "なんなん？" << endl;
            } else {
                save_grid_activater[epoch][x * (map_gridline) + y] = GL[x][y].ave_activator;
            }
        }
    }

//    cout << Sxy << "," << Sx << "," << Sy << "," << r << endl;
}

void mouse(int button, int state, int x, int y) //マウスボタンの処理
{
    if (state == GLUT_DOWN) { //ボタンが押されたら...
        if (fStart == 1)
            fStart = 0;
        else
            fStart = 1;
        for (auto &i: robo) {
            i.init();
        }
    }
}

int main(int argc, char *argv[]) {

    Initialize();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(point, point);
    windows[0] = glutCreateWindow("Robot sim");
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
//    glutIdleFunc(idle);
    glutMouseFunc(mouse); //マウスのボタンを検出 これを切るとコマ送りになる。
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(point, point);
    windows[1] = glutCreateWindow("Grid");
//    glutCreateSubWindow(0,0,200,200,200);
    glutDisplayFunc(grid_display);
    glutReshapeFunc(resize);
    glutIdleFunc(idle);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    glutMainLoop();

    calculate_autocorr();
    cout << "finish" << endl;

    return 0;
}


void Initialize() {
    cout << "map_gridline:" << map_gridline << endl;
    epoch = 0;
    make_circle();//円図形データの作成
    for (auto &i: robo) i.init();

}

double calculate_input_ave() {
    double sum = 0;

    for (int i = 0; i < map_gridline; ++i) {
        for (int j = 0; j < map_gridline; ++j) {
            sum += input_concentration[i][j];
        }
    }
    return sum / (gridline * gridline);
}

void input_turingpattern() {

    //ロボットすべてに対して濃度のinputを開始
    for (auto &i: robo) {
        //X軸検索
        for (int x = 0; x < map_gridline; ++x) {
            //y軸検索
            for (int y = 0; y < map_gridline; ++y) {
                //あらかじめ設定されたパターんを酔いこみ
                if (i.map_glid_x == x && i.map_glid_y == y) {
//                    cout<<i.map_glid_x<<","<<i.map_glid_y<<endl;
                    i.activator = input_concentration[y][x];
                    i.inhibitor = 0;
                }
            }
        }
    }
}

void calculate_autocorr() {
    double tmp = map_gridline * map_gridline;
    double sum = 0;
    double ave[N] = {};

    //各列の平均を取得。
    for (int i = 0; i < tmp; ++i) {
        for (int j = 0; j < N * 2; ++j) {
            ave[i] += save_grid_activater[j][i] / (N * 2);
        }
//        save_autocorr[i]=sum
    }

    //自己共分散
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < tmp; ++k) {
                save_autocorr[i][k] +=
                        ((save_grid_activater[j][k] - ave[k]) *
                         (save_grid_activater[j + i][k] - ave[k]));
            }
        }
//        save_autocorr[i]=sum
    }

    for (int i = 1; i < N; ++i) {
        for (int j = 0; j < tmp; ++j) {
            save_autocorr[i][j] = save_autocorr[i][j] / (save_autocorr[0][j]);
            save_autocorr_fin[i] += save_autocorr[i][j] / tmp;
        }
    }


    ofstream outputfile("C:\\Users\\Jun\\CLionProjects\\Simulater_Leraning\\test.csv");

    for (int i = 0; i < N; ++i) {
        outputfile << save_autocorr_fin[i];
        for (int j = 0; j < 225; ++j) {
            outputfile << "," << save_autocorr[i][j];
        }
        outputfile << endl;
    }
    outputfile.close();
    cout << "calcfinish" << endl;
}
