# Folder Rules

## 基本方針

- 機能ごとにフォルダを分ける
- 新機能は可能な限り新ファイルを作る
- 1ファイル肥大化を避ける

---

# src

ゲームコード。

## src/system

ゲーム全体管理。

例:

- Game.cpp
- Input.cpp
- Time.cpp

---

## src/scene

シーン管理。

例:

- TitleScene.cpp
- GameScene.cpp
- ResultScene.cpp

---

## src/player

プレイヤー関連。

例:

- Player.cpp
- PlayerMove.cpp
- PlayerAnim.cpp

---

## src/enemy

敵関連。

例:

- EnemyBase.cpp
- Slime.cpp

---

## src/map

マップ関連。

例:

- Map.cpp
- StageLoader.cpp
- CollisionMap.cpp

---

## src/item

拾えるアイテム。

例:

- HealItem.cpp
- Battery.cpp

---

## src/object

ギミック・オブジェクト。

例:

- Door.cpp
- Lift.cpp
- Switch.cpp

---

## src/ui

UI関連。

例:

- Button.cpp
- HUD.cpp

---

## src/effect

演出関連。

例:

- Particle.cpp
- Explosion.cpp

---

## src/utility

汎用処理。

例:

- Math.cpp
- Random.cpp
- Collision.cpp