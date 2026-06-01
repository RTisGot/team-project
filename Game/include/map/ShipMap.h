#pragma once

class ShipMap
{
public:
    ShipMap();
    ~ShipMap();

    bool Init();

    void Update(const VECTOR& playerPos);
    void Draw();

    int GetModelHandle() const { return m_modelHandle; }

private:
    int m_modelHandle;				///< モデルハンドル
};
