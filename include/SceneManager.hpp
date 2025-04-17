#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include "Scene.hpp"
#include "App.hpp"

#include <atomic>
#include <memory>
#include "Util/Logger.hpp"

extern std::shared_ptr<Scene> menu_scene;
extern std::shared_ptr<Scene> main_scene;
extern std::shared_ptr<Scene> game_scene1;
extern std::shared_ptr<Scene> game_scene2;
extern std::shared_ptr<Scene> select_scene;

class SceneManager{
public:
    enum class SceneType{
        MENU,
        MAIN,
        SELECT,
        GAME1,
        GAME2,
        GAME3
    };
    SceneManager(){
        LOG_DEBUG("Scenemanager Constructor");
    }
    ~SceneManager() {
        LOG_DEBUG("Scenemanager Destructor");
    };
    void set_current_scene(std::shared_ptr<Scene> scene){
        current_scene = scene.get();
        current_scene->on_enter();
    }
    void switch_to(SceneType type){
        current_scene->on_exit();
        switch (type) 
        {
        case SceneType::MENU:
            current_scene = menu_scene.get();
            break;
        case SceneType::MAIN:
            current_scene = main_scene.get();
            break;
        case SceneType::SELECT:
            current_scene = select_scene.get();
            break;
        case SceneType::GAME1:
            current_scene = game_scene1.get();
            break;
        case SceneType::GAME2:
            current_scene = game_scene2.get();
            break;
        default:
            return;
        }
        if (current_scene) {
            current_scene->on_enter();
        }
    }
    
    void on_update()
    {
        if (current_scene) {
            current_scene->on_update();
        }
    }

    void on_render()
    {
        if (current_scene) {
            current_scene->on_render();
        }
    }
    void on_exit(){
        if (current_scene) {
            current_scene->on_exit();
        }
    }
private:
    Scene* current_scene = nullptr;
};

#endif //SCENEMANAGER_HPP