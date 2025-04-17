#ifndef GAMESCENE2_HPP
#define GAMESCENE2_HPP

#include "GameObject2.hpp"
#include "SceneManager.hpp"
#include "Scene.hpp"
#include "BackgroundImage.hpp"
#include "Player.hpp"
#include "Enemy3.hpp"
#include "Slime.hpp"
#include "Tile.hpp"
#include "Text.hpp"
#include "App.hpp"

#include <memory>
#include <cstdlib> // for rand()
#include <ctime>   // for time()

#include "Util/GameObject.hpp"
#include <Util/Renderer.hpp>
#include <vector>
#include "Util/Logger.hpp"
#include "Util/Input.hpp"
#include "camera.hpp"

extern SceneManager scene_manager;

class GameScene2 : public Scene
{
public:
    GameScene2(){LOG_DEBUG("GameScene2 Constructor");}
    ~GameScene2(){LOG_DEBUG("GameScene2 Destructor");}
    void on_enter() override{
        LOG_DEBUG("Entering MainScene");
        spawnTime = Util::Time::GetElapsedTimeMs(); // 初始化為遊戲開始時間

        if (!m_Background) {
        m_Background = std::make_shared<BackgroundImage>(); //背景
        m_Background->m_Transform.scale = {1, 1};
        m_Background->Set_Background(phase);
        }

        m_Tiles.push_back(std::make_shared<Tile>(glm::vec2(0, 100)));  // 右邊的地板

        for (auto& tile : m_Tiles) {
            tile->SetImage(RESOURCE_DIR "/Image/Background/tile 1.png");
            tile->SetLengthWidth(157.0, 618.0);
            m_Root2.AddChild(tile);
        }

        m_Player = std::make_shared<Player>();  //玩家
        m_Player->SetPosition({0.0f, -450});
        m_Player->SetZIndex(5.0);
        m_Player->m_Transform.scale = {1,1};
        m_Player->Setboundary(-881.5,861.5,530,-520);

        m_Camera = std::make_shared<Camera>(800, 600);  //跟隨玩家的鏡頭
        m_Camera->FollowTarget(m_Player);

        PlayerHP_Text = std::make_shared<Text>(); //顯示玩家血量
        PlayerHP_Text->SetColor({255, 0, 0, 255});  // 設定紅色
        PlayerHP_Text->SetPosition({-500, 300});  // 左上角位置

        m_Root2.AddChild(m_Background);
        m_Root2.AddChild(m_Player);
        m_Root.AddChild(PlayerHP_Text);

        // 讓 Renderer 使用 Camera
        m_Root2.SetCamera(m_Camera);
    }
    void on_update() override{
        float currentTime = Util::Time::GetElapsedTimeMs(); // 取得遊戲運行時間的方法
        if(currentTime >= Maxtime) {} // 獲勝 

        m_Player->Update(m_Tiles);      
        m_Camera->Update();  // 讓 Camera 追蹤 Player

        // 更新敵人

        // 迴圈移除死掉的敵人
        
        // 如果經過的時間超過15秒

        std::string hpText = "HP: " + std::to_string(m_Player->GetHP());
        PlayerHP_Text->SetText(hpText);
    }
    void on_render() override {
        m_Root.Update();
        m_Root2.Update();
    }
    void on_exit() override {
        LOG_DEBUG("Exiting MainScene");

        // 移除場景中的物件
        m_Root2.RemoveChild(m_Background);
        m_Root2.RemoveChild(m_Player);
        m_Root.RemoveChild(PlayerHP_Text);
        for (auto& tile : m_Tiles) {
            m_Root2.RemoveChild(tile);
        }

        m_Background.reset();
        m_Player.reset();
        PlayerHP_Text.reset();
    }
    void SpawnOneEnemy();

private:
    const float Maxtime = 180000.0f; // 180s = 3min
    const float spawnInterval = 15000.0f; // 15s
    float spawnTime = 0.0f;

    std::shared_ptr<BackgroundImage> m_Background;
    std::vector<std::shared_ptr<Tile>> m_Tiles; 

    std::shared_ptr<Camera> m_Camera;
    std::shared_ptr<Player> m_Player;
    //std::shared_ptr<Enemy3> m_Enemy;

    std::shared_ptr<Text> PlayerHP_Text;
    //std::shared_ptr<Text> EnemyHP_Text;


    const int phase = 2; 
};



#endif