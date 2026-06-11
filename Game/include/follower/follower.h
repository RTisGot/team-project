#ifndef DEF_FOLLOWER_H
#define DEF_FOLLOWER_H

#include <DxLib.h>
#include <Inventory/Inventory.h>
#include <vector>

class OrbManager;
class OrbActor;

class Follower
{
public:
    Follower();
    ~Follower();

    void LoadModel();

    void Update();
    void Draw();

    void SetTargetPosition(const VECTOR& pos);
    void SetTargetAngle(float angle);

    /**
     * @brief オーブマネージャー設定
     */
    void SetOrbManager(OrbManager* pOrbManager);

    /**
     * @brief サーチ中か取得
     */
    bool IsSearching() const
    {
        return m_IsSearching;
    }

    Inventory& GetInventory()
    {
        return m_Inventory;
    }

    /**
    * @brief オーブを受け取る
    */
    void ReceiveOrb(uint32_t orbId);

    /**
    * @brief オーブ所持確認
    */
    bool HasOrb() const;

private:
    int m_ModelHandle;

    VECTOR m_Position;
    VECTOR m_TargetPosition;

    float m_Angle;
    float m_TargetAngle;

    //=========================
    // サーチ関連
    //=========================

    bool m_IsSearching;
    float m_SearchTimer;
    float m_CoolTimeTimer;

    uint32_t m_HoldingOrbId;
    OrbManager* m_pOrbManager;

    static constexpr float SEARCH_TIME = 5.0f;
    static constexpr float COOL_TIME = 10.0f;

    /**
    * @brief クールタイム残り時間取得
    */
    float GetCoolTime() const
    {
        return m_CoolTimeTimer;
    }

    Follower* m_pFollower;
    Inventory m_Inventory;
};

#endif // DEF_FOLLOWER_H
