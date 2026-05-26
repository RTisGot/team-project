#include "DxLib.h"
//#include "Input.h"
#include "system/camera.h"

//カメラ初期化
void Camera_Initialize() {
    //奥行0.1～1000までをカメラの描画範囲とする
    SetCameraNearFar(0.1f, 1000.0f);

    //プレイヤーを見る角度にカメラを設置
    SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 1.2f, -3.8f), VGet(0.0f, 1.0f, 0.0f));

    //ライティング処理をOFF
    SetUseLighting(FALSE);
}

//カメラ情報更新
void Camera_Update() {

}

//描画
void Camera_Draw() {

}

//カメラ終了処理
void Camera_Finalize() {

}
