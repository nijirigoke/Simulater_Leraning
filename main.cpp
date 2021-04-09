//
// Created by T118029 on 2021/03/15.
//

//test
//todo 要修正：ロボットがぐるぐる回っている

#include <GL/glut.h>
#include <iostream>
//#include "simbase.h"
#include "workspace.h"

#define LEFT   0
#define CENTER 1
#define RIGHT  2
#define TRANGE 1.5 //タッチセンサーのレンジ 半径の倍数

#define RIGHT_TURN -0.1        //右回転 0.1ラジアンの定義
#define LEFT_TURN    0.1        //左回転 0.1ラジアンの定義
#define ROBOS  7   //ロボット台数　10台
#define NSENSORRANGE 100    //センサーの検出距離（絶対距離）


typedef struct ROBO {
    double r;
    double x, y;
    double dir;
    POSITION tsensor[3]; //構造体変数の追加
public:
    void draw();

    void action();

    void init();

    void turn(double q);

    void forward(double v);

    int touchsensor(int i);

    double nearrobotsensor();

} ROBO;

ROBO robo[ROBOS];    //要素数ROBOSで配列変数roboを定義


void wall_draw();

int check_cross_wall(POSITION p1, POSITION p2);

int check_cross_others(POSITION p);

int SearchRobot(POSITION p, double range);

void Initialize();

void wall_draw() {
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
    wall_draw();
    for (int i = 0; i < ROBOS; i++) {
        robo[i].draw();
    }

    glutSwapBuffers();
}

void ROBO::forward(double v) {
    for (int i = 0; i < ROBOS; i++) {
        robo[i].x = robo[i].x + cos(robo[i].dir) * v;
        robo[i].y = robo[i].y + sin(robo[i].dir) * v;
    }

}

void ROBO::turn(double q) {
    for (int i = 0; i < ROBOS; i++) {
        robo[i].dir += q;
    }
}

void ROBO::action() {
    int tCenter, tRight, tLeft;

    tCenter = touchsensor(CENTER);    //中央センサーの値
    tRight = touchsensor(RIGHT);        //右センサーの値
    tLeft = touchsensor(LEFT);        //左センサーの値

    //todo 要修正：どうもこの書き方だとデットロックがかかる可能性があるらしい

    if (tLeft == 1)        //左チセンサ反応あり
    {
        turn(RIGHT_TURN);
    } else if (tRight == 1)    //右センサ反応あり
    {
        turn(LEFT_TURN);
    } else if (tCenter == 1) //正面センサ反応あり
    {
        turn(RIGHT_TURN);
    } else    //いずれの条件も当てはまらないのは全てのタッチセンサが０のとき
    {
        forward(0.05);//前進1.0ステップ
    }
}


void idle() {
    if (fStart == 0)
        return;
    for (int i = 0; i < ROBOS; i++) {
        robo[i].action();
    }
    display();
}

void mouse(int button, int state, int x, int y) //マウスボタンの処理
{
    if (state == GLUT_DOWN) { //ボタンが押されたら...
        if (fStart == 1)
            fStart = 0;
        else
            fStart = 1;
        for (int i = 0; i < ROBOS; i++) {
            robo[i].init();
        }
    }
}

void ROBO::init() {
    dir = rand() * 360.0;
    x = 0;
    y = 0;
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
    draw_circle(0, 0, r); //本体外形円の描画　現在の座標系の原点に対して描くことに注意

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
    //todo 要修正：なぜかロボのセンサー情報が共有されているようである。
    //todo これが処理落ちなのかどうかについて調べる必要アリか
    int fw;
    int fo; //他のロボットとの接触フラグ
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

int check_cross_wall(POSITION p1, POSITION p2) {
    int i;

    for (i = 0; i < WALLS; i++) {
        double Wp1x, Wp1y, Wp2x, Wp2y; //線分の両端点
        double Bvx, Bvy;               //線分への垂線の方向ベクトル
        double Bx, By, R;              //球体の位置, 半径
        double Vx, Vy;                 //球体の速度
        double L;                      //単位ベクトル化のための一時変数
        double det, a, b, c, d, e, f;  //ベクトルの式の変数

        //接触判定
        Bx = p1.x;
        By = p1.y;

        Wp1x = pin[wall[i].p1].x;
        Wp1y = pin[wall[i].p1].y;
        Wp2x = pin[wall[i].p2].x;
        Wp2y = pin[wall[i].p2].y;

        Bvx = (p2.x - p1.x);
        Bvy = (p2.y - p1.y);
        L = sqrt(Bvx * Bvx + Bvy * Bvy);
        Bvx /= L; //単位ベクトル化
        Bvy /= L;
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

int check_cross_others(POSITION p) {
    double l;
    double sensor_x;
    double sensor_y;
// 各ロボの座標 for:
// 自他の区別 ？？？
// 区別したらそいつとの距離を産出ること
    sensor_x = p.x;
    sensor_y = p.y;
    for (int i = 0; i < ROBOS; ++i) {
        double another_robo_x = robo[i].x;
        double another_robo_y = robo[i].y;
        double distance_x;
        double distance_y;

        distance_x = another_robo_x - sensor_x;
        distance_y = another_robo_y - sensor_y;

        l = sqrt(pow(distance_x, 2) + pow(distance_y, 2));

        if (l < robo[i].r) {
            return 1;
        }
    }
    return 0;
}

int SearchRobot(POSITION p, double range) {
    int i;
    double l;        //距離
    double dx, dy;    //位置の差
    double min = range;    //最も近い距離
    int minid = -1;        //最も近い位置のロボット番号

    for (i = 0; i < ROBOS; i++) {
        dx = robo[i].x - p.x;
        dy = robo[i].y - p.y;
        l = sqrt(dx * dx + dy * dy);

        if (l == 0) {
            continue;    //距離0なので自分と見なし、スキップ
        }

        if (l < min)    //最短距離であったら
        {
            min = l;    //最短距離の更新
            minid = i;    //ロボット番号の更新
        }
    }
    return minid;
}

double ROBO::nearrobotsensor()//構造体ROBOに所属している関数なので関数名の前に“ROBO::”とつく
{
    double q;
    int i;

    double mx, my;
    double nx, ny;
    double l;
    double inner;
    double outer;

    POSITION p;
    p.x = x;
    p.y = y;

    i = SearchRobot(p, NSENSORRANGE);

    if (i < 0) {
        return 100;    //もしも該当ロボットが無かった場合は100(>π)
    }

    //自分の正面ベクトル
    mx = cos(dir);
    my = sin(dir);

    //相手のベクトル
    nx = robo[i].x - x;
    ny = robo[i].y - y;
    l = sqrt(nx * nx + ny * ny);
    nx /= l;
    ny /= l;

    inner = mx * nx + my * nx;
    outer = mx * ny - my * nx;

    q = acos(inner);
    if (outer < 0)//外積値が負なら
    {
        q = -q;//負号反転
    }
    return q;    //　－π～＋π
}


int main(int argc, char *argv[]) {
    Initialize();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(510, 510);

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
    for (int i = 0; i < ROBOS; ++i) {
        robo[i].init();
    }
}
