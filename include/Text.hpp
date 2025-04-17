#ifndef TEXT_HPP
#define TEXT_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include <memory>
#include "GameObject2.hpp"

class Text : public Util::GameObject {
public:
    Text() : GameObject(
            std::make_unique<Util::Text>(RESOURCE_DIR"/Font/Planes_ValMore.ttf", 40,
                                         append_string_views(s_PhaseTasks[0]),
                                         Util::Color::FromName(Util::Colors::WHITE)),
            100) {
        m_Transform.translation = {0.0F, -270.F};
    }

    void Next(const int phase) {
        auto temp = std::dynamic_pointer_cast<Util::Text>(m_Drawable);
        temp->SetText(append_string_views(s_PhaseTasks[phase]));
    }

    void Set_to(const int phase) {
        auto temp = std::dynamic_pointer_cast<Util::Text>(m_Drawable);
        temp->SetText(append_string_views(s_PhaseTasks[phase]));
    }

    void SetText(const std::string& text) {
        auto temp = std::static_pointer_cast<Util::Text>(m_Drawable);
        if (temp) {
            temp->SetText(text);
        }
    }

    void SetColor(const SDL_Color& color) {
        if (auto temp = std::dynamic_pointer_cast<Util::Text>(m_Drawable)) {
            temp->SetColor(color);
        }
    }

    void SetPosition(const glm::vec2& pos) {
        m_Transform.translation = pos;
    }


private:
    inline static std::string append_string_views(std::string_view sv1) {
        return std::string(sv1) + "\n";
    }

    static constexpr std::string_view s_PhaseTasks[5] = {
            "Press ENTER to start",
            "",
            "",
            "",
            "",
    };
};


#endif //TEXT_HPP
