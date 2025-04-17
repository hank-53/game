#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <SDL_timer.h>
#include <memory>

#include "Scene.hpp"
#include "MenuScene.hpp"
#include "MainScene.hpp"
#include "GameScene1.hpp"
#include "GameScene2.hpp"
#include "SceneManager.hpp"
#include "SelectScene.hpp"

SceneManager scene_manager;
std::shared_ptr<Scene> menu_scene;
std::shared_ptr<Scene> main_scene;
std::shared_ptr<Scene> game_scene1;
std::shared_ptr<Scene> game_scene2;
std::shared_ptr<Scene> select_scene;



void App::Start() {
    LOG_TRACE("Start");

    menu_scene = std::make_shared<MenuScene>();
    main_scene = std::make_shared<MainScene>();
    select_scene = std::make_shared<SelectScene>();
    game_scene1 = std::make_shared<GameScene1>();
    game_scene2 = std::make_shared<GameScene2>();

    scene_manager.set_current_scene(menu_scene);
    
    m_CurrentState = State::UPDATE;

}

void App::Update() {
    
    //TODO: do your things here and delete this line <3
    scene_manager.on_render();
    scene_manager.on_update();
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
