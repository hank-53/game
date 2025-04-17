#ifndef MENUSCENE_HPP
#define MENUSCENE_HPP

#include <memory>

#include "App.hpp"
#include "Scene.hpp"
#include "BackgroundImage.hpp"
#include "Text.hpp"
#include "SceneManager.hpp"

#include "Util/GameObject.hpp"
#include <Util/Renderer.hpp>
#include "Util/Logger.hpp"
#include <Util/Keycode.hpp>

extern SceneManager scene_manager;

class MenuScene : public Scene
{
public:
    MenuScene(){
        LOG_DEBUG("MenuScene Constructor");
    };
    ~MenuScene() {
        LOG_DEBUG("MenuScene Destructor");
    }
    void on_enter() override{
        LOG_DEBUG("Entering MenuScene");
        m_Background = std::make_shared<BackgroundImage>();

        m_Text = std::make_shared<Text>();
        m_Text->SetText("Press Enter to Start");

        m_Root2.AddChild(m_Background);
        m_Root.AddChild(m_Text);
    }
    void on_update() override{
        if (Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
            LOG_DEBUG("Enter pressed, switching to MainScene");
            scene_manager.switch_to(SceneManager::SceneType::MAIN);
        }
    }
    void on_render() override{
        m_Root.Update();
        m_Root2.Update();
    }
    void on_exit() override{
        LOG_DEBUG("Exiting MenuScene");
        m_Root2.RemoveChild(m_Background);
        m_Root.RemoveChild(m_Text);
    }

private:
    std::shared_ptr<BackgroundImage> m_Background;
    std::shared_ptr<Text> m_Text;
};

#endif //MENUSCENE_HPP