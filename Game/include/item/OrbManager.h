#ifndef ORB_MANAGER_H
#define ORB_MANAGER_H

#include <memory>
#include <vector>
#include <DxLib.h>

class OrbActor; 
class Player;
class CollisionManager;

class OrbManager
{
public:

    OrbManager() = default;
    ~OrbManager();

    bool Init();

    void Update(Player* player, CollisionManager* collisionManager);

    void Draw() const;

    /*
     * @brief オーブを生成する
     * @param id オーブのID
     * @param position 生成する位置
     */
    void CreateOrb(uint32_t id, const VECTOR& position);

    /*
     * @brief 指定した位置から最も近いオーブを検索する
     * @param position 検索の基準となる位置
     * @param range 検索範囲（この距離以内のオーブが対象）
     * @return 最も近いオーブへの共有ポインタ。該当するオーブがない場合はnullptr。
     */
    std::shared_ptr<OrbActor> FindNearestOrb(const VECTOR& position, float range);

    /*
     * @brief オーブIDからオーブを検索する
     * @param id 検索するオーブのID
     * @return 該当するオーブへの共有ポインタ。該当するオーブがない場合はnullptr。
     */
    std::shared_ptr<OrbActor>FindOrbById(uint32_t id);
private:

    int m_OrbModelHandle = -1; // オーブのモデルハンドル
    std::vector<std::shared_ptr<OrbActor>> m_Orbs;  // オーブのリスト
    Player* m_Player; // プレイヤーへのポインタ
};

#endif // ORB_MANAGER_H
