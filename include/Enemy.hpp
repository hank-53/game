#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "pch.hpp"
#include "Character.hpp"
#include "Player.hpp"

#include <glm/fwd.hpp>
#include <memory>
#include <vector>

#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "Util/Animation.hpp"

enum class EnemyState {
    Idle,
    Move,
    Attack
};

class Enemy : public Character {
public:
    Enemy()
        : Character(RESOURCE_DIR "/Image/Character/enemy/IdleL/1.png", 3000, 10, 170) {
        
        InitAnimations();
        m_Transform.translation = {-200, 200};
        SetDrawable(m_Animations[0][(int)EnemyState::Idle]); // 預設為左向待機動畫
    }

    void SetTarget(std::shared_ptr<Player> player) {
        m_TargetPlayer = player;
    }

    void MoveLeft() {
        float deltaTime = Util::Time::GetDeltaTimeMs() / 1000.0f;
        m_Transform.translation.x = std::max(m_Transform.translation.x - m_Speed * deltaTime, -951.5f);
        faceRight = false;
        SetState(EnemyState::Move);
    }

    void MoveRight() {
        float deltaTime = Util::Time::GetDeltaTimeMs() / 1000.0f;
        m_Transform.translation.x = std::min(m_Transform.translation.x + m_Speed * deltaTime, 931.5f);
        faceRight = true;
        SetState(EnemyState::Move);
    }


    float m_AttackCooldown = 50; // 冷卻
    float m_LastAttackTime = 0.0f;
    void Attack() {
        float currentTime = Util::Time::GetElapsedTimeMs() / 1000.0f;
        if (currentTime - m_LastAttackTime >= m_AttackCooldown) {
            SetState(EnemyState::Attack);
            m_LastAttackTime = currentTime;
            LOG_DEBUG("Enemy Attack!!");
        }
    }

    void hurt(){
        TakeDamage(10);
    }

    void Die(){

    }

    void Update() {
        if (!m_TargetPlayer) {
            SetState(EnemyState::Idle);
            return;
        }

        float playerX = m_TargetPlayer->GetPosition().x;
        float playerY = m_TargetPlayer->GetPosition().y;
        float enemyX = m_Transform.translation.x;
        float enemyY = m_Transform.translation.y;
        float distanceX = std::abs(playerX - enemyX);
        float distanceY = std::abs(playerY - enemyY);
        
        LOG_DEBUG(distanceX);
        LOG_DEBUG(distanceY);

        if (distanceX < 100.0f) {
            if (distanceY == 0) {
                Attack();
                attacking = true;
                moving = false;
            }
        } else if (distanceX < 1000.0f) {
            (playerX < enemyX) ? MoveLeft() : MoveRight();
            moving = true;
            attacking = false;
        }

        if (!(moving && attacking)){
            SetState(EnemyState::Idle);
        }

    }

private:
    std::shared_ptr<Player> m_TargetPlayer;
    EnemyState m_CurrentState = EnemyState::Idle;
    std::shared_ptr<Util::Animation> m_Animations[2][3]; // [0] = 左, [1] = 右 | [Idle, Move, Attack]

    bool faceRight = false;

    bool moving = false;
    bool attacking = false;

    const float height = 300.0; //敵人的長寬
    const float weight = 488.0; 

    void InitAnimations() {
        std::vector<std::string> idleFramesL = LoadFrames("IdleL", 15);
        std::vector<std::string> idleFramesR = LoadFrames("IdleR", 15);
        std::vector<std::string> moveFramesL = LoadFrames("walkingL", 18);
        std::vector<std::string> moveFramesR = LoadFrames("walkingR", 18);
        std::vector<std::string> attackFramesL = LoadFrames("AttackL", 21);
        std::vector<std::string> attackFramesR = LoadFrames("AttackR", 21);

        m_Animations[0][(int)EnemyState::Idle] = CreateAnimation(idleFramesL, 50, 500);
        m_Animations[1][(int)EnemyState::Idle] = CreateAnimation(idleFramesR, 50, 500);
        m_Animations[0][(int)EnemyState::Move] = CreateAnimation(moveFramesL, 50, 0);
        m_Animations[1][(int)EnemyState::Move] = CreateAnimation(moveFramesR, 50, 0);
        m_Animations[0][(int)EnemyState::Attack] = CreateAnimation(attackFramesL, 40, 1000);
        m_Animations[1][(int)EnemyState::Attack] = CreateAnimation(attackFramesR, 40, 1000);
    }

    std::vector<std::string> LoadFrames(const std::string& action, int count) {
        std::vector<std::string> frames;
        for (int i = 1; i <= count; i++) {
            frames.push_back(RESOURCE_DIR "/Image/Character/enemy/" + action + "/" + std::to_string(i) + ".png");
        }
        return frames;
    }

    std::shared_ptr<Util::Animation> CreateAnimation(const std::vector<std::string>& frames, int frameTime, int loopDelay) {
        return std::make_shared<Util::Animation>(frames, true, frameTime, true, loopDelay);
    }

    void SetState(EnemyState newState) {
        if (m_CurrentState == newState) return;
        m_CurrentState = newState;
        SetDrawable(m_Animations[faceRight ? 1 : 0][(int)newState]);
    }
};

#endif
