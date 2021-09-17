//
// Created by T118029 on 2021/03/15.
//

//todo 目標 通信の実装。通信の可視化。センサレンジの可視化。通信波及の可視化。

#include <GL/glut.h>
#include <iostream>
//#include "simbase.h"
#include "workspace.h"
#include <random>
#include <chrono>
#include <iostream>
#include <thread>
#include <functional>
#include <vector>
#include <bits/stdc++.h>

//pull request test

#define LEFT   0
#define CENTER 1
#define RIGHT  2
#define TRANGE 1.5 //タッチセンサーのレンジ 半径の倍数
#define RANGE 50 //通信レンジ の半径
#define SENS_RANGE 50 //通信レンジ の半径
#define TEST 10

#define RIGHT_TURN -0.1        //右回転 0.1ラジアンの定義
#define LEFT_TURN    0.1        //左回転 0.1ラジアンの定義
#define ROBOS  50  //ロボット台数　10台

using namespace std;
std::random_device rnd;     // 非決定的な乱数生成器
std::mt19937 mt(rnd());


typedef struct ROBO {
    double r{};
    double x{}, y{};
    double dir{};
    int receive_flag{};
    int sens_flag{};
    POSITION tsensor[3]{}; //構造体変数の追加
public:
    vector<double> line_memory_x;
    vector<double> line_memory_y;

    void line_draw();

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

    void senser_action();

    void flash_action();

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


void ROBO::line_draw() {

    glBegin(GL_LINE_STRIP);
    glColor3d(0.5, 0.5, 0.5);

//    std::cout<<line_memory_x.size()<<endl;
    for (int i = 0; i < line_memory_x.size(); i++) {
//        cout<<line_memory_x[i]<<endl;
        glVertex2d(line_memory_x[i], line_memory_y[i]);
    }
//           glVertex2d(line_memory_x[1], line_memory_y[1]);
//      glVertex2d(line_memory_x[0], line_memory_y[0]);

    glEnd();

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    graphics();
    wall_draw();
    for (auto &i : robo) {
        i.draw();
//        i.line_draw();
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

    int test = 20;

    std::uniform_int_distribution<int> distr(0, TEST);    // 非決定的な乱数生成器
    int tCenter, tRight, tLeft;

    tCenter = touchsensor(CENTER);    //中央センサーの値
    tRight = touchsensor(RIGHT);        //右センサーの値
    tLeft = touchsensor(LEFT);        //左センサーの値

//    if (sens_flag == 1) {
//        std::cout << "flash::" << flash_memori << std::endl;
//        if (flash_memori >= test - 4) {
//            sens_nearrobotsensor();
//        }
////        flash_memori++;
//
//    }
//
//    if (sens_flag == 0 && receive_flag == 1 && flash_memori < test) {
//        std::cout << flash_memori << std::endl;
//        nearrobotsensor();
//    }
//    if (flash_memori > test) {
//        flash_memori = 0;
//        receive_flag = 0;
//    }

    if (sens_flag == 0 && receive_flag == 1 && flash_memori < test) {
        std::cout << flash_memori << std::endl;
        nearrobotsensor();
    }
    if (flash_memori > test) {
        flash_memori = 0;
        receive_flag = 0;
    }

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

    line_memory_x.push_back(x);
    line_memory_y.push_back(y);
//    line_memory[1].push_back(y);
//std::cout << flash_memori << std::endl;
//std::cout << receive_flag << std::endl;
//    flash_memori++;


}

void ROBO::flash_action() {
//    nearrobotsensor();

    nearrobotsensor();//発信者になるかどうか

}


void idle() {
    if (fStart == 0) return;
    for (auto &i : robo) {
        i.action();
    }
    Sleep(1 * 1);
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

    std::uniform_int_distribution<int> distr(-200, 200);    // 非決定的な乱数生成器
    std::uniform_real_distribution<> dir_gen(0, 360);
    line_memory_x.clear();//vecter の　中身を削除
    line_memory_y.clear();

    if (sens_flag == 1) {
        dir = dir_gen(mt);
//    printf("%f", dir);
        x = 0;
        y = 0;
        r = 10;
    } else {
        dir = dir_gen(mt);
//    printf("%f", dir);
        x = distr(mt);
        y = distr(mt);
        r = 10;
    }
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

//    if (receive_flag == 1) {
//        glColor3d(1.0, 1.0, 0.0);
//        draw_robo_circle(0, 0, r);
//    }
    draw_circle(0, 0, r); //本体外形円の描画　現在の座標系の原点に対して描くことに注意
    glColor3d(0.5, 0.5, 0.5);
    if (sens_flag == 1) {
        draw_circle(0, 0, SENS_RANGE); //通信範囲の描画

    } else {
        draw_circle(0, 0, RANGE); //通信範囲の描画
    }

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
    line_draw();
    glColor3d(1, 1, 1);

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
            i.receive_flag = 1;
        }
    }
}

double ROBO::sens_nearrobotsensor() {
    double l;
    double distance_x;
    double distance_y;
    std::chrono::system_clock::time_point start, end; // 型は auto で可
    end = std::chrono::system_clock::now();  // 計測終了時間
    start = std::chrono::system_clock::now(); // 計測開始時間
    //自身の座標から一定レンジを探索？ 計算量ちゃん…。

    for (auto &i : robo) {

        //他のロボットのちゃん座標
        double another_robo_x = i.x;
        double another_robo_y = i.y;

        distance_x = another_robo_x - x;
        distance_y = another_robo_y - y;

        l = sqrt(pow(distance_x, 2) + pow(distance_y, 2));
        if (l < SENS_RANGE) {
//            printf("%f\n::%d",l,RANGE);
            i.receive_flag = 1;
        }
    }
}


int main(int argc, char *argv[]) {

    for (int i = 0; i < CIRCLEDIV; i++) {
        pin[i] = {250 * sin(2 * PI * (double) i / (double) CIRCLEDIV),
                  250 * cos(2 * PI * (double) i / (double) CIRCLEDIV)};

        // debug
//        printf("%f\n%f\n", pin[i].x, pin[i].y);
//        printf("-------------------------\n");
        wall[i] = {i + 0, i + 1};

        if (i == CIRCLEDIV - 1) wall[i] = {i, 0};
//        printf("%d\n%d\n", wall[i].p1, wall[i].p2);
//        printf("-------------------------\n");

    }

    Initialize();
    robo[0].receive_flag = 1;
    robo[0].sens_flag = 1;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(510, 510);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutIdleFunc(idle);

//    glutMouseFunc(line_draw);
    glutMouseFunc(mouse); //マウスのボタンを検出

    glutMainLoop();


    return 0;
}

void Initialize() {
    make_circle();//円図形データの作成
    for (auto &i : robo) i.init();
}
