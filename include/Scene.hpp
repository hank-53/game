#ifndef SCENE_HPP
#define SCENE_HPP

#include "Util/Renderer.hpp"

#include <memory>
#include "GameObject2.hpp"
#include "Renderer2.hpp"

class Scene : public GameObject2
{
public:
    // Scene() = default;
    Scene() : m_Root2(std::vector<std::shared_ptr<GameObject2>>{}) {}  // ✅ 正確初始化 Renderer2
    virtual ~Scene() = default;
    virtual void on_enter() = 0;
    virtual void on_update() = 0;
    virtual void on_render() = 0;
    virtual void on_exit() = 0;
protected:
    std::vector<std::shared_ptr<GameObject2>> m_Objects;
    Util::Renderer m_Root; 
    Renderer2 m_Root2; //用來渲染遊戲畫面中會隨玩家移動的物體
};




#endif //SCENE_HPP