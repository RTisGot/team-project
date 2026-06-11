#include "ui/InteractionUI.h"
#include <DxLib.h>

InteractionUI::InteractionUI()
    :
    m_IsVisible(false),
    m_Text("")
{
}

void InteractionUI::SetText(
    bool isVisible,
    const std::string& text)
{
    m_IsVisible = isVisible;
    m_Text = text;
}

void InteractionUI::Draw()
{
    if (!m_IsVisible)
    {
        return;
    }

    int screenX;
    int screenY;

    GetDrawScreenSize(
        &screenX,
        &screenY);

    DrawFormatString(
        screenX / 2 - 80,
        screenY - 120,
        GetColor(0, 0, 0),
        "%s",
        m_Text.c_str());
}
