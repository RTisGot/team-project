#include "scene/TitleScene.h"
#include "scene/LobbyScene.h"
#include "scene/LoadingScene.h"
#include <DxLib.h>
#include <memory>

TitleScene::TitleScene(SceneManager* manager)
    : m_manager(manager),
    m_CurrentPhase(Phase::LogoFadeIn), 
    m_LogoHandle(-1),
    m_TitleMainHandle(-1),
    m_Timer(0),
    m_Alpha(0.0f),
    m_TitleFontHandle(-1)
{
    
}

void TitleScene::Init()
{
    m_TitleFontHandle = CreateFontToHandle(
        "メイリオ",
        48,
        3
    );
}


void TitleScene::Update(float deltaTime)
{
    m_Timer++;// タイマーを進める
   // m_manager->ChangeScene(std::make_shared<LobbyScene>(m_manager));
    // フェーズごとの処理
    switch (m_CurrentPhase)
    {
    case Phase::LogoFadeIn:
        // フェードイン
        m_Alpha += 1.0f / 60.0f;
        if (m_Alpha >= 1.0f)
        {
            m_Alpha = 1.0f;
            m_CurrentPhase = Phase::LogoStay;
            m_Timer = 0; // タイマーリセット
        }
        break;

    case Phase::LogoStay:
        // 60フレーム（約1秒）の間、そのまま表示をキープ
        if (m_Timer > 60)
        {
            m_CurrentPhase = Phase::LogoFadeOut;
            m_Timer = 0;
        }
        break;

        // フェードアウト
    case Phase::LogoFadeOut:
       
        m_Alpha -= 1.0f / 60.0f;
        if (m_Alpha <= 0.0f)
        {
            m_Alpha = 0.0f;
            m_CurrentPhase = Phase::TitleMain; //タイトル画面へ
            m_Timer = 0;
        }
        break;

        //タイトル画面のメインフェーズ
    case Phase::TitleMain:
        //スペースキーの入力gamesceneへ遷移
        //if (CheckHitKey(KEY_INPUT_SPACE) == 1)
      //  {
            m_manager->ChangeScene(std::make_shared<LobbyScene>(m_manager));
     //   }
        break;
    }
}

void TitleScene::Loadgraph() {
    
}

void TitleScene::Draw()
{
    int alpha = static_cast<int>(m_Alpha * 255);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

    const char* title = "ひろって、かえろう";

    
    // 本体
    DrawStringToHandle(
        660,
        200,
        title,
        GetColor(255, 255, 255),
        m_TitleFontHandle);

    DrawString(1200, 690, "Ver1.0", GetColor(255, 255, 255));
    
    DrawString(760, 280, "-- 制作メンバー --", GetColor(255, 255, 255));
    DrawString(800, 320, "渡邉　和斗", GetColor(255, 255, 255));
    DrawString(800, 360, "立原　零央", GetColor(255, 255, 255));
    DrawString(800, 400, "村田　智仁", GetColor(255, 255, 255));
    DrawString(800, 440, "松倉　光姫", GetColor(255, 255, 255));
    
    if (m_LogoHandle == -1)
    {
        //logoの読み込み
        m_LogoHandle = LoadGraph("Game/assets/UI/Neko.png");

    }
    else
    {
        // m_Alphaを変換してブレンドモードを設定
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(m_Alpha * 255));

        if (m_LogoHandle >= 0)
        {
            int logoW = 0, logoH = 0;
            GetGraphSize(m_LogoHandle, &logoW, &logoH);

            float scale = 0.2f;
            int newW = static_cast<int>(logoW * scale);
            int newH = static_cast<int>(logoH * scale);

            int x1 = 200;
            int y1 = 150;

            DrawExtendGraph(x1, y1, x1 + newW, y1 + newH, m_LogoHandle, TRUE);

            // ブレンドモードを通常に戻す
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }

}
