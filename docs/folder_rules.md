# Folder Rules

## 基本方針

- 機能ごとにフォルダを分ける
- 新機能は可能な限り新ファイルを作る
- 1ファイル肥大化を避ける

---

# source

ゲームコード。

## source/system

ゲーム全体管理。

例:

- Game.cpp
- Input.cpp
- Time.cpp

---

## source/scene

シーン管理。

例:

- TitleScene.cpp
- GameScene.cpp
- ResultScene.cpp

---

## source/player

プレイヤー関連。

例:

- Player.cpp
- PlayerMove.cpp
- PlayerAnim.cpp

---

## source/enemy

敵関連。

例:

- EnemyBase.cpp
- Slime.cpp

---

## source/map

マップ関連。

例:

- Map.cpp
- StageLoader.cpp
- CollisionMap.cpp

---

## source/item

拾えるアイテム。

例:

- HealItem.cpp
- Battery.cpp

---

## source/object

ギミック・オブジェクト。

例:

- Door.cpp
- Lift.cpp
- Switch.cpp

---

## source/ui

UI関連。

例:

- Button.cpp
- HUD.cpp

---

## source/effect

演出関連。

例:

- Particle.cpp
- Explosion.cpp

---

## source/utility

汎用処理。

例:

- Math.cpp
- Random.cpp
- Collision.cpp