#pragma once
/**
 * @file GameData.h
 * @brief ゲーム全体で使用するデータや定数の定義
 */

/**
 * @brief キャラクターステータスの構造体
 * @note 今後正式に仕様が決まったら追加予定
 */
struct CharacterStatus
{
    int hp;
};

/**
 * @class GameData
 * @brief ゲーム全体で使用するデータや定数を管理するクラス
 */
class GameData
{
public:
    GameData();
    ~GameData() = default;

    //TODO: 今後正式に仕様が決まったらMapやお供など追加予定

};
