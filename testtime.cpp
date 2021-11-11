#include <iostream>
#include <vector>

using namespace std;

struct GLID {
    double ave_activator;
    double ave_inhibitor;

    GLID(int act, int inh) : ave_activator(act), ave_inhibitor(inh) {}

};

/* typedefと同じ。型エイリアス宣言というc++の機能 */
/* using identifier attr = type-id ;の形で宣言される */
// typedef vector<vector<Edge>> Graph;と同義
using Graph = vector<vector<GLID>>;

int main() {
    /* 宣言はこれでOK。コンテナだからメモリを確保しているだけ */
    Graph TEST(3);

    /* ひとつひとつ代入する必要がある */
    /* 1次元の領域に追加 */
    TEST[0].push_back(GLID(1, 2));
    TEST[1].push_back(GLID(1, 2));
    TEST[2].push_back(GLID(1, 2));

    /* 二次元の領域に追加 */
    TEST[0].push_back(GLID(8, 12));
    cout << TEST[0][1].ave_activator << endl;
    cout << TEST[0][1].ave_inhibitor << endl;

    return 0;
}