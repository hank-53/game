#ifndef SELECTSCENE_HPP
#define SELECTSCENE_HPP

#include <memory>

#include "App.hpp"
#include "Scene.hpp"
#include "BackgroundImage.hpp"
#include "Player.hpp"
#include "Text.hpp"
#include "SceneManager.hpp"
#include "Tile.hpp"

#include "Util/GameObject.hpp"
#include <Util/Renderer.hpp>
#include "Util/Logger.hpp"
#include <Util/Keycode.hpp>

extern SceneManager scene_manager;

class SelectScene : public Scene
{
public:
    SelectScene(){
        LOG_DEBUG("SelectScene Constructor");
    };
    ~SelectScene() {
        LOG_DEBUG("SelectScene Destructor");
    }
    void on_enter() override{ 
            m_Background = std::make_shared<BackgroundImage>(); //背景
            m_Background->m_Transform.scale = {1, 1};
            m_Background->Set_Background(phase);

            m_Player = std::make_shared<Player>();  //玩家
            m_Player->SetPosition({0.0f, -450});
            m_Player->SetZIndex(5.0);
            m_Player->m_Transform.scale = {1,1};
            m_Player->Setboundary(-951.5,931.5,530,-420);

            m_Camera = std::make_shared<Camera>(800, 600);  //跟隨玩家的鏡頭
            m_Camera->FollowTarget(m_Player);

            m_Root2.AddChild(m_Background);
            m_Root2.AddChild(m_Player);

            m_Root2.SetCamera(m_Camera);
    }
    void on_update() override{
        m_Player->Update(m_Tiles);
        m_Camera->Update();  // 讓 Camera 追蹤 Player
    }
    void on_render() override{
        m_Root2.Update();
    }
    void on_exit() override{}
private:
    std::shared_ptr<BackgroundImage> m_Background;
    std::vector<std::shared_ptr<Tile>> m_Tiles; 
    std::shared_ptr<Player> m_Player;
    std::shared_ptr<Camera> m_Camera;
    int phase = 1;
};

#endif