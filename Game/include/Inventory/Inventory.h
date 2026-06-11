#pragma once

#include <vector>

/**
 * @brief 所持アイテム情報
 */
struct ItemData
{
    int ItemId;
    int Count;
};

/**
 * @brief インベントリクラス
 */
class Inventory
{
public:
    void AddItem(int itemId);
    bool RemoveItem(int itemId);

    const std::vector<ItemData>& GetItems() const
    {
        return m_Items;
    }

private:
    std::vector<ItemData> m_Items;
};
