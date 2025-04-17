#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Util/GameObject.hpp"
#include "Player.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "GameObject2.hpp"
#include "Util/Logger.hpp"

class Camera : public GameObject2 {
public:
    Camera(float width, float height)
        : m_Width(width), m_Height(height) {}

    void FollowTarget(const std::shared_ptr<Player>& target) {
        m_Target = target;
    }

    void Update() {
        if (m_Target) {
            auto targetPos = m_Target->GetTransform().translation;

            // 限制 Camera 不超出背景
            m_Transform.translation.x = std::clamp(targetPos.x, m_WorldLeft + m_Width, m_WorldRight - m_Width);
            m_Transform.translation.y = std::clamp(targetPos.y, m_WorldBottom + m_Height, m_WorldTop - m_Height);

            // LOG_DEBUG("Camera Position: ", m_Transform.translation.x, m_Transform.translation.y);
        }
    }
    
    void SetZoom(float zoomFactor) {
        m_Width = 800 / zoomFactor;  // 800 是原始寬度
        m_Height = 600 / zoomFactor; // 600 是原始高度
    }

    glm::vec2 GetPosition() const { return m_Transform.translation; }

    glm::mat4 GetViewMatrix() const {
        return glm::translate(glm::mat4(1.0f), glm::vec3{-m_Transform.translation, 0.0f});
    }

private:
    float m_Width, m_Height;
    float m_WorldLeft = -1419.0f;
    float m_WorldRight = 1419.0f;
    float m_WorldTop = 857.0f;
    float m_WorldBottom = -857.0f;
    
    std::shared_ptr<Player> m_Target = nullptr;
};

#endif // CAMERA_HPP
