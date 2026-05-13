# Coding Rules



## 基本方針



- 可読性を最優先
- 処理を機能ごとに分割する
- 1ファイル肥大化を避ける
- 1関数は30〜40行程度を目安にする



---



# 命名規則



## クラス名



PascalCase



例:



- Player
- EnemyManager



---



## 関数名



PascalCase



例:



- Update
- Draw
- LoadModel



---



## メンバ変数



m_ prefix を付ける



例:



- m_Position
- m_HP



---



## ローカル変数



camelCase



例:



- moveSpeed
- targetPos



---



## 定数



UPPER_SNAKE_CASE



例:



- MAX_PLAYER_HP



---



# メモリ管理



- new / delete を直接使用しない
- std::unique_ptr を使用する
- 所有権を明確にする



---



# 関数ルール



- 1関数1責務
- 深いネストを避ける
- early return を推奨



---



# クラスルール



- Update と Draw を分離する
- 描画とロジックを混在させない
- 外部依存を最小限にする



---



# includeルール



include順:



1. 自分のヘッダ
2. STL
3. DxLib
4. 他クラス



---



# コメントルール



- 「何をしているか」ではなく

&#x20; 「なぜ必要か」を書く



- 複雑な処理には説明を書く



- 一時対応には TODO を付ける



---



# DxLibルール



- MV1モデルハンドルはクラス管理する
- モデル読み込みを毎フレーム行わない
- Draw と Update を分離する



---



# 禁止事項



- 巨大switch文
- マジックナンバー
- グローバル変数多用
- 1クラスへの機能集中

