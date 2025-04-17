#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include "SDL_timer.h"
#include "Util/Image.hpp"
#include "Util/GameObject.hpp"
#include "Util/Time.hpp"
#include <memory>

#include "GameObject2.hpp"

class Character : public GameObject2 {
public:
    explicit Character(const std::string& ImagePath, int hp, int atk, int speed)
        : m_HP(hp), m_ATK(atk), m_Speed(speed) {
        SetImage(ImagePath);
        ResetPosition();
    }

    Character(const Character&) = delete;
    Character(Character&&) = delete;
    Character& operator=(const Character&) = delete;
    Character& operator=(Character&&) = delete;

    [[nodiscard]] const std::string& GetImagePath() const { return m_ImagePath; }
    [[nodiscard]] const glm::vec2& GetPosition() const { return m_Transform.translation; }
    [[nodiscard]] bool GetVisibility() const { return m_Visible; }

    int GetHP() const { return m_HP; }
    int GetATK() const { return m_ATK; }
    int GetSpeed() const { return m_Speed; }

    void TakeDamage(int damage){
        m_HP -= damage;
    };
    virtual void Die() {
        // 預設死亡行為 (可讓子類別 override)
    }
    void SetImage(const std::string& ImagePath) {
        m_ImagePath = ImagePath;
        m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
    }
    void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }
    virtual void Update() {
        // 預留給子類別 (處理物理、動畫等)
    }
protected:
    std::string m_ImagePath;

    int m_HP;
    int m_ATK;
    int m_Speed;
    int m_Defense;

private:
    void ResetPosition() { m_Transform.translation = {0, 0}; }
};

#endif // CHARACTER_HPP
