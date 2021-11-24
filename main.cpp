#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
//
// Created by T118029 on 2021/03/15.
//

#include <GL/glut.h>
#include <iostream>
#include "simbase.h"
#include <random>
#include <vector>
#include <numeric>

#define LEFT   0
#define CENTER 1
#define RIGHT  2
#define TRANGE 1.5 //タッチセンサーのレンジ 半径の倍数
#define RANGE 50 //通信レンジ の半径
#define RIGHT_TURN -0.1        //右回転 0.1ラジアンの定義
#define LEFT_TURN    0.1        //左回転 0.1ラジアンの定義
#define ROBOS  2000 //ロボット台数　10台
using namespace std;

struct GLID_STRUCT {

    double ave_activator;
    double ave_inhibitor;
};

typedef struct ROBO {
    double r{};
    double x{}, y{};
    double dir{};

    int glid_x = 0;
    int glid_y = 0;

    int tCenter, tRight, tLeft;

    //信号出力フラグ群
    int receive_flag{};
    int sens_flag{};

    //反応拡散用パラメータ群
    double activator;
    double inhibitor;
    double sum_activator = 0;
    double sum_inhibitor = 0;
    int touch_counter = 0;

    double dx;
    double dy;
//    double du = 0.08;
//    double dv = 0.50;
//    double Cu = 0.00010;
//    double Cv = 0.0000;
//    double a = 0.01;
//    double b = 0.011;
//    double c = 0.008;
//    double d = 0.009;

    double du = 0.08;
    double dv = 0.50;
    double Cu = 0.0000;
    double Cv = 0.0005;
    double a = 0.01;
    double b = 0.010;
    double c = 0.009;
    double d = 0.010;

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

    double sens_nearrobotsensor();

} ROBO;

ROBO robo[ROBOS];    //要素数ROBOSで配列変数roboを定義
int step_counter = 0;
int epoch = 0;
int gridline = (int) point * 2 / RANGE;
int half_gridline = gridline / (2 * RANGE);
int robotwindow;
int gridwindow;
int windows[2];

std::random_device rnd;     // 非決定的な乱数生成器
std::mt19937 mt(rnd());
GLID_STRUCT GL[100][100];

void calculate_grid_concentration();

void draw_grid_density_map();

void grid_display();

void wall_draw();

void Initialize();

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

    double hoge = 2 * point;
    int num = hoge / (RANGE);

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
    for (int j = 0; j < num; ++j) {
        glVertex2d(-point + (j * RANGE), point);
        glVertex2d(-point + (j * RANGE), -point);
    }

    // yoko
    for (int j = 0; j < num; ++j) {
        glVertex2d(point, point - (j * RANGE));
        glVertex2d(-point, point - (j * RANGE));
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
    grid_wall_draw();
    glutSwapBuffers();

}

void calculate_grid_concentration(GLID_STRUCT **GL) {

    vector<double> sum_glid_activator;
    vector<double> sum_glid_inhibitor;
    double ave_activator = 0;
    double ave_inhibitor = 0;
    //todo 計算に問題あり
    for (int x = 0; x < gridline; ++x) {
        for (int y = 0; y < gridline; ++y) {// y point
            //todo 配列を初期化すべし
            for (int i = 0; i < ROBOS; i++) {// i point
//                cout << x << ";" << robo[i].glid_x << endl;
                if (x == robo[i].glid_x && y == robo[i].glid_y) {
                    sum_glid_activator.push_back(robo[i].activator);
                    sum_glid_inhibitor.push_back(robo[i].inhibitor);
                }
            }
            //todo この段階で平均値を産出すべし
        }
        ave_activator =
                accumulate(sum_glid_activator.begin(), sum_glid_activator.end(), 0.0) / sum_glid_activator.size();
        ave_inhibitor =
                accumulate(sum_glid_inhibitor.begin(), sum_glid_inhibitor.end(), 0.0) / sum_glid_inhibitor.size();
        GL2[x].push_back(GLID_STRUCT(ave_activator, ave_inhibitor));



    }
//    cout << GL1[0][0].ave_activator << "::" << GL1[0][0].ave_inhibitor <<"::"<<GL1[0].size() <<endl;
//    cout << GL2[0][0].ave_activator << "::" << GL2[0][0].ave_inhibitor << "::" << GL2[0].size() << endl;
//    cout<<sum_glid_activator.size()<<"::"<<GL1[0][0].ave_inhibitor<<endl;
}

void draw_grid_density_map() {
    double hoge = 2 * point;
    int num = hoge / (RANGE);
//    cout << GL2[0][0].ave_activator << "::" << GL2[0][0].ave_inhibitor << "::" << GL2[0].size() << endl;
    glBegin(GL_QUADS);

    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < gridline; ++i) {
        for (int j = 0; j < gridline; ++j) {
//            glColor3b(GL2[i][j].ave_activator, GL2[i][j].ave_inhibitor,
            glColor3b(1, 1, 0);
            glVertex2d(-point + (RANGE * i), (RANGE * j));
            glVertex2d(-point + (RANGE * i), (RANGE * (j + 1)));
            glVertex2d(-point + (RANGE * (i + 1)), (RANGE * (j + 1)));
            glVertex2d(-point + (RANGE * (i + 1)), (RANGE * j));
        }
    }
    glEnd();

//    glutSwapBuffers();

}

void ROBO::forward(double v) {
    x = x + cos(dir) * v;
    y = y + sin(dir) * v;
}

void ROBO::turn(double q) {
    dir += q;
}

void ROBO::action() {

    glid_x = (floor(x) / (2 * RANGE)) + half_gridline;//グリッドぎり
    glid_y = (floor(y) / (2 * RANGE)) + half_gridline;

//    std::uniform_int_distribution<int> distr(0, TEST);    // 非決定的な乱数生成器

    dx = activator * a - inhibitor * b + Cu;
    dy = activator * c - inhibitor * d + Cv;

    activator = activator + dx;
    inhibitor = inhibitor + dy;

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
            forward(0.6);//前進1.0ステップ
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

    glColor3d(activator, inhibitor, 1 - 0.5 * (activator + inhibitor));
    draw_robo_circle(0, 0, r);

    draw_circle(0, 0, r); //本体外形円の描画　現在の座標系の原点に対して描くことに注意
//    glColor3d(0.3, 0.3, 0.3);
//    draw_circle(0, 0, RANGE); //通信範囲の描画
//    glColor3d(0.3, 0.3, 0.3);
//    glBegin(GL_LINES);
////    glVertex2d(0, 0); //左センサーの描画
////    glVertex2d(tsensor[LEFT].x, tsensor[LEFT].y);
////    glVertex2d(0, 0); //正面センサーの描画
////    glVertex2d(tsensor[CENTER].x, tsensor[CENTER].y);
////    glVertex2d(0, 0); //右センサーの描画
////    glVertex2d(tsensor[RIGHT].x, tsensor[RIGHT].y);

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

            l = sqrt(pow(distance_x, 2) + pow(distance_y, 2));
            if (l < i.r) {
                double tx;
                double ty;

                tx = du * (activator - i.activator);
                ty = dv * (inhibitor - i.inhibitor);

                i.activator += tx;
                i.inhibitor += ty;

//                i.sum_activator += tx;
//                i.sum_inhibitor += ty;

                sum_inhibitor += inhibitor - ty;
                sum_activator += activator - tx;

                touch_counter++;
                if (step_counter >= 10) {

                    activator = 2 * sum_activator / touch_counter;
                    inhibitor = 2 * sum_inhibitor / touch_counter;
                    sum_activator = 0;
                    sum_inhibitor = 0;
                    touch_counter = 0;

                    step_counter = 0;
                }
                return 1;
            }
        }


    }
    return 0;
}

void idle() {

    if (fStart == 0) return;
    for (auto &i: robo) i.action();
//    Sleep(1 * 100);
    calculate_grid_concentration();

    display();
    for (int i; i < 2; i++) {
        glutSetWindow(windows[i]);
        glutPostRedisplay();
    }

    step_counter++;
    epoch++;
//    cout << epoch << ";;;;";
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
//    glutMainLoop();

//    Initialize();
//    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(point, point);
    windows[1] = glutCreateWindow("Grid");
//    glutCreateSubWindow(0,0,200,200,200);
    glutDisplayFunc(grid_display);
    glutReshapeFunc(resize);
    glutIdleFunc(idle);
//    //サブウィンドウ作成
//    SubWinID[0] = glutCreateSubWindow(MainWinID,0,0,160,120);//左上原点、(0,0)から横160、縦120ピクセル分
//    GLUT_CALL_FUNC_SUB();
//    MY_INIT_SUB();
//    glutMouseFunc(mouse); //マウスのボタンを検出
    glutMainLoop();

    return 0;
}

void Initialize() {
    make_circle();//円図形データの作成
    for (auto &i: robo) i.init();
}
