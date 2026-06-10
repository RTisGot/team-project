#ifndef DEF_INTERACTIONUI_H
#define DEF_INTERACTIONUI_H

#include <string>

/**
 * @brief インタラクト案内UI
 */
class InteractionUI
{
public:
    InteractionUI();

    /**
     * @brief 表示内容設定
     */
    void SetText(
        bool isVisible,
        const std::string& text);

    /**
     * @brief 描画
     */
    void Draw();

private:
    bool m_IsVisible;
    std::string m_Text;
};

#endif
