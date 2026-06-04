#ifndef DEF_SHIPMAP_H
#define DEF_SHIPMAP_H

#include <DxLib.h>

/**
 * @class ShipMap
 * @brief 船マップクラス
 */
class ShipMap
{
public:
    ShipMap();
    ~ShipMap();

    /**
     * @brief 初期化
     * @return 成功ならtrue
     */
    bool Init();

    /**
     * @brief 更新処理
     */
    void Update();

    /**
     * @brief 描画処理
     */
    void Draw();

    /**
     * @brief モデルハンドル取得
     */
    int GetModelHandle() const { return m_modelHandle; }

private:
    int m_modelHandle; ///< 船マップモデル
    VECTOR m_scale;
};

#endif // DEF_SHIPMAP_H
