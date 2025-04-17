#ifndef LEVEL_SELECT_UI_HPP
#define LEVEL_SELECT_UI_HPP

#include "GameObject2.hpp"
#include "Text.hpp"
#include <vector>
#include <functional>

class LevelSelectUI : public GameObject2 {
public:
    LevelSelectUI();

    void SetOnSelectCallback(std::function<void(int)> cb);
    void SetOnCancelCallback(std::function<void()> cb);
    void Show();
    void Hide();

    void UpdateUI(); // 可呼叫於 on_update()
    void RenderUI(); // 可呼叫於 on_render()

private:
    struct Button {
        std::shared_ptr<Text> label;
        glm::vec2 position;
        glm::vec2 size;
        int levelId;
    };

    std::vector<Button> m_Buttons;
    std::shared_ptr<Text> cancelButton;
    bool isVisible = false;

    std::function<void(int)> onSelect;
    std::function<void()> onCancel;

    bool IsMouseOver(const glm::vec2& pos, const glm::vec2& size);
};

#endif
