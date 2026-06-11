#include "Inventory/Inventory.h"

void Inventory::AddItem(int itemId)
{
    for (auto& item : m_Items)
    {
        if (item.ItemId == itemId)
        {
            item.Count++;
            return;
        }
    }

    ItemData newItem;
    newItem.ItemId = itemId;
    newItem.Count = 1;

    m_Items.push_back(newItem);
}

bool Inventory::RemoveItem(int itemId)
{
    for (auto& item : m_Items)
    {
        if (item.ItemId == itemId)
        {
            item.Count--;

            if (item.Count <= 0)
            {
                item = m_Items.back();
                m_Items.pop_back();
            }

            return true;
        }
    }

    return false;
}
