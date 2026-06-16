#pragma once
#include "DxLib.h"

//skyboxクラス
class Skybox {
public:
    Skybox();
    ~Skybox();
    bool Load(const char* filepath);
    void Draw(const VECTOR& centerPos);
private:
    int m_ModelHandle;
};
