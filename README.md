# 研究活動報告

## 概要

- 群ロボ（創発、自己組織化）などに興味を持ち、一年ほど前より所属。しかし、成果物などは未だになし。
    - 現在はC言語を用いて過去授業で行われていたシミュレーター作成に取組中。
        - こちらは一応動作するものが作成済み。
        - プログラム内で利用されているOpenGL操作用ライブラリGLUTからGLFWへ切り替え、もしくは多言語への移植を考案中。

### todo:

- [x] ロボットに通信（刺激の伝播）を実装
- [ ] 通信（刺激の伝播）を可視化
    - [x] 通信レンジを可視化（通信状態の有無も表示）
    - [ ] 行動ログを表示 gnewplot??

---

### 以下日記

---

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






