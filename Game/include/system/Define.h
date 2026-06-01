#ifndef DEF_DEFINE_H
#define DEF_DEFINE_H

//オブジェクトのジャンプ関連の列挙体

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

typedef enum {
    OBJ_NO_JUMP, //ジャンプしていない
    OBJ_JUMP, //ジャンプ
    OBJ_LOWJUMP, //小ジャンプ
    OBJ_FALL //落下中
}Object_Jump;

extern Object_Jump object_jump;

#endif
