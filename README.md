# 研究活動報告

## 概要

- 群ロボ（創発、自己組織化）などに興味を持ち、一年ほど前より所属。しかし、成果物などは未だになし。
    - 現在はC言語を用いて過去授業で行われていたシミュレーター作成に取組中。

### todo:

- [x] 反応拡散理論をモチーフにした計算式の構築
- [x] チューリングパターン創出を目指す
- [x] 行動軌跡メモリ
- [x] グリッドマップの可視化
- [x] うまくパターンが出た際の生波形を出す。
- [x] 色設定の際のレンジを出す。何をもってその色にするのかを定義する。
- [x] ロボットの行動記録を取る。
  - ロボットの開始点から最終到達点までってこと。（stepの差というのはあるけどね）
- [x] 自己相関の計算を行う。
- [x] 卒論序論を完成させる。

---

### 以下日記

---

## 2021/12/21

初期値をパターンで与えて、それの値の変化を相関係数を用いて計算する事になった。よって必要な物を次の通り

- グリッドの値をsteo毎に記録する配列（100000step位）
- ロボットにパターンの初期値を与える関数

と思われる。 値の与え方はあらかじめ過去のパターンで出たものを記録しておいてそれを了する（なお人力）

**以下baseとなるチューリングパターン**

| 0 | 1 | 2  | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 |
| --- | --- |----| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | .5 |    |  |  | 1 | 1 | 1 | .5 |  |  |  |  | 1 | 1 | 1 |
| 2 | .5 | .5 |  |  | 1 | 1 | 1 |  |  |  |  | 1 | 1 | 1 | 1 |
| 3 |  | 1  | 1 | 1 | 1 | .5 | 1 | 1 |  |  |  | .5 | 1 |  | .5 |
| 4 |  | .5 | 1 | 1 | 1 |  | .5 | 1 | 1 |  | .5 | 1 | 1 |  |  |
| 5 |  |    | 1 | 1 | 1 |  |  | 1 | 1 | 1 | 1 | 1 | 1 | 1 |  |
| 6 |  | .5 | 1 | 1 | .5 |  |  | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 |
| 7 |  | .5 | 1 | 1 |  |  |  | 1 | .5 | 1 |  |  |  | .5 | 1 |
| 8 |  | 1  | 1 | 1 | 1 |  |  |  |  |  |  |  |  |  |  |
| 9 |  | 1  | 1 | 1 | 1 |  |  |  |  |  |  |  |  |  |  |
| 10 |  |    | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 |  |  |  |  |  |
| 11 |  |    |  | 1 | 1 | 1 | 1 | 1 | 1 | 1 |  |  |  | 1 | .5 |
| 12 | .5 |    |  |  | 1 | 1 | 1 | 1 | 1 | 1 |  |  | .5 | 1 | 1 |
| 13 | 1 |    |  |  |  |  | 1 | 1 | 1 | 1 |  |  |  | 1 | 1 |
| 14 | 1 | .5 |  |  |  |  |  | 1 | 1 |  |  |  |  | 1 | 1 |
| 15 | 1 | .5 |  |  |  |  |  |  | 1 |  |  |  |  | .5 | 1 |

## 2021/12/06

グリッドマップの可視化は完成したが、その色が実態に即しているかは少々疑問点が残る。というのも、glcolor3d関数は1~
0の値でしか色を表現することが不可能にもかかわらず入門の値は1を超えたりマイナスの値になることさえある。その結果実態に即したシミュレーション結果となっているか謎が多い。現実的な会としては3d空間上にプロットするほうが適切なのであろうか？そしたらGLUTじゃなくてgnewplotといった感じになると思うが…。

- シミュレーション空間からのはみ出し者をなくすように変更

`std::uniform_int_distribution<int>
distr(-point+1, point-1); // 非決定的な乱数生成器`

- シミュレーション空間を先輩と同じ1200*1200に変更
- 計算設計をsumでため込んで10stepごとに計算する方式へ改める。
-

## 2021/11/24

グリッドの計算方式の再考。 データ構造を構造体配列に変更する。またセルごとの計算方式に問題があり、各セル毎に平均が取れていないことが確認された。

## 2021/11/12

各グリッドの値を記録するvecterとそれを計算する関数を実装。ベクターのサイズ的に正しく生成されているか疑問符がつくが、いったん生成できた。つぎはこれをビジュアライズする必要がある。

## 2021/11/11

グリッドを切り、各グリッドの濃度を求めるために、二次元Vecterの構造体を作成した。

## 2021/11/08

計算方法としてsum_activater+tyと他のロボットに対してもしたほうがいいのかなと考案中。しかしパターンが発生しないという問題が発生中でもあるので困っているところ。

## 2021/10/13

- step数を記録する寄稿を導入
- 反応拡散における計算方法を修正。
    - 研究報告yoshida24-12を参考
- いわゆるチューリングパターンの発現を確認
    - 1000stepあたりでパターン形成が落ち着くようである。

## 2021/10/08

- シミュレーターはすべてのロボットの位置を総当たりするというものであったが、これをグリッド方式で自身の周囲のロボットに対してのみ計算を行う手法へ転換を図った。
- グリッドマップなる概念を手に入れた。
- 計算関連のコードが散逸しているため、ヘッダーファイルにまとめることを考案中。

## 2021/09/17

- 任意の大きさにシミュレーション範囲を広げることができる変数 point を simbase_test に追記
- 興奮度を黄色と青であらわすプログラムがおおよそ完成。
- 密度をあげると計算にちめいてきな程遅くなる。
- 反応拡散理論のきれいな実装ができているかいまいちわかっていない。

## 2021/07/30

- センサーレンジを50に変更 55ぐらいが丁度いいかも
- 拡散の様子を確かめるためにsleepを１０００mに変更
- 仕事が増えたぞ！！

具体的にはロボットの軌跡を書くプログラムを展開すべしとお達しが。 描画に関しては各ロボット1000step程計算してから描画してもいいかも

普通にvecterでごり押ししました。台数が増えてくるといろいろ大変になってくる。というかglutめんどくさいね…。順番をちゃんとやらないといけないのマジで沼ポイントでした。ぴえん（🥺🥺🥺ぴえんを許すな）

### キーワード

- 反応拡散モデル。
    - チューリングパターン
- [デマ拡散](file:///C:/Users/T118029b/Downloads/07shirai.pdf)
- [パーコレーションモデル](http://www.kisoriko.jp/kisoriko/?TakeiMasato%2FPercolation)
- 通信密度。
    - フェロモン敵サムシング、通信強度的なあれね。

## 2021/07/28

- 色つけたよー

## 2021/07/26

- ロボットにステップカウント方式のタイマーを搭載
- カウンターによってフラグをさげる仕組みを搭載
- 発信用ロボットを中心に配置し固定。

## 2021/07/07

### 現状

ロボットの移動ロジックに改良を加えてスタックを防止することに成功。一応のシミュレータベースは確立された

### ロボットの挙動フロー

発信ロボットパターン

1. 各ロボット同士の距離を検索
2. 指定されたレンジ内のロボットのフラグを立てる

受信ロボットパターン

1. 外部から刺激のフラグを立てられる
2. フラグが立ち、自身が発信者に
3. 上記と同じ挙動
4. クールタイムまで発信を停止。それまでフラグ状態を解消しない。

## 2021/04/13

成功！！ actiondでなぜか一つ進行ごとに進めてたぞ！！

## 2021/04/13

試したこと。

ロボットが一体として行動してしまう問題について以下の検証をおこなった。

- ロボットのセンサー値は共有されている。
- その結果行動が同期している。

これより全関数を構造体に含めることによって状況の改善を図ってみた

検証の結果以下のことがわかった。

- タッチセンサーデータの共有は発生していない。
- 構造体化しても状況は改善しない

続く検証をmotomu






