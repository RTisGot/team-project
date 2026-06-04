#include "object/PlayObject.h"

PlayObject::PlayObject()
{
}

PlayObject::~PlayObject()
{
    for (int handle : m_ModelHandles)
    {
        if (handle != -1)
        {
            MV1DeleteModel(handle);
        }
    }
}
//D:\グループ制作\Game\assets\models\map\playground
bool PlayObject::Init()
{
    int slideHandle =
        MV1LoadModel("Game/assets/models/map/playground/slide.mv1");

    int seesawHandle =
        MV1LoadModel("Game/assets/models/map/playground/seesaw.mv1");

    int swingHandle =
        MV1LoadModel("Game/assets/models/map/playground/swing.mv1");

    int monkeyBarHandle =
        MV1LoadModel("Game/assets/models/map/playground/monkeybar.mv1");

    m_ModelHandles.push_back(slideHandle);
    m_ModelHandles.push_back(seesawHandle);
    m_ModelHandles.push_back(swingHandle);
    m_ModelHandles.push_back(monkeyBarHandle);

    // 滑り台
    MV1SetPosition(
        slideHandle,
        VGet(300.0f, 0.0f,-200.0f));

    MV1SetScale(
        slideHandle,
        VGet(0.05f, 0.05f, 0.05f));


    // シーソー
    MV1SetPosition(
        seesawHandle,
        VGet(-60.0f, 0.0f, -200.0f));

    MV1SetScale(
        seesawHandle,
        VGet(0.05f, 0.05f, 0.05f));

    // ブランコ
    MV1SetPosition(
        swingHandle,
        VGet(60.0f, 0.0f, -200.0f));

    MV1SetScale(
        swingHandle,
        VGet(0.1f, 0.1f, 0.1f));

    // うんてい
    MV1SetPosition(
        monkeyBarHandle,
        VGet(150.0f, 0.0f, -200.0f));

    MV1SetScale(
        monkeyBarHandle,
        VGet(0.02f, 0.02f, 0.02f));

    return true;
}

void PlayObject::Update()
{
    // 現状処理なし
}

void PlayObject::Draw()
{
    for (int handle : m_ModelHandles)
    {
        if (handle != -1)
        {
            MV1DrawModel(handle);
        }
    }
}
