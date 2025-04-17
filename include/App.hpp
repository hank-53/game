#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include <memory>

#include "MenuScene.hpp"
#include "SceneManager.hpp"


class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)


private:
    void ValidTask();
private:
    State m_CurrentState = State::START;
    bool m_WDown = false;
    bool m_ADown = false;
    bool m_SDown = false;
    bool m_DDown = false;
    bool m_JDown = false;

};

#endif //APP_HPP
