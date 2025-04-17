#ifndef ENEMY3_HPP
#define ENEMY3_HPP

#include "pch.hpp"
#include "Character.hpp"
#include "Player.hpp"

#include <glm/fwd.hpp>
#include <memory>
#include <vector>

#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "Util/Animation.hpp"

enum class Enemy3State {
    Move
};

enum class MoveDirection3{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Enemy3 : public Character {
public:
    Enemy3()
    : Character(RESOURCE_DIR "/Image/Character/enemy3/L1.png", 100, 10, 80), 
    m_MoveAnimation(std::make_shared<Util::Animation>(
        std::vector<std::string>{
            RESOURCE_DIR "/Image/Character/enemy3/R1.png",
            RESOURCE_DIR "/Image/Character/enemy3/R2.png",
            RESOURCE_DIR "/Image/Character/enemy3/R3.png"
        },
        true, 500, true, 0)),
    m_MoveAnimationL(std::make_shared<Util::Animation>(
        std::vector<std::string>{
            RESOURCE_DIR "/Image/Character/enemy3/L1.png",
            RESOURCE_DIR "/Image/Character/enemy3/L2.png",
            RESOURCE_DIR "/Image/Character/enemy3/L3.png"
        },
        true, 500, true, 0))
    {        
    m_Transform.translation = {-200, 200};
    SetDrawable(m_MoveAnimation);
}
    void SetTarget(std::shared_ptr<Player> player) {
        TargetPlayer = player;
    }

    float GetWidth() const { return width; }
    float GetLength() const { return length; }
    bool GetAlive() const { return alive; }

    void Move(MoveDirection3 Direction, int n){
        float deltaTime = Util::Time::GetDeltaTimeMs() / 1000.0f;
        switch (Direction){
            case MoveDirection3::UP:
                m_Transform.translation.y = m_Transform.translation.y + m_Speed * n * deltaTime;
                break;
            case MoveDirection3::DOWN:
                m_Transform.translation.y = m_Transform.translation.y - m_Speed * n * deltaTime;
                break;
            case MoveDirection3::LEFT:
                m_Transform.translation.x = m_Transform.translation.x - m_Speed * n * deltaTime;
                faceRight = false;
                break;
            case MoveDirection3::RIGHT:
                m_Transform.translation.x = m_Transform.translation.x + m_Speed * n * deltaTime;
                faceRight = true;
                break;
            default:
                break;
        }
        SetState(Enemy3State::Move);   
    }

    void Attack(){
        LOG_DEBUG("Enemy Attack!!");
        TargetPlayer->hurt(m_ATK);
    }
    void Hurt(){
        TakeDamage(TargetPlayer->GetATK());
        if (m_HP <= 0){
            Die();
        }
        else {
            repulsed();
            LOG_DEBUG("Enemy Hurt!!");
        }
    }
    void Die(){
        alive = false;
        LOG_DEBUG("Enemy Die!!");
    }
    void repulsed(){
        if (TargetPlayer->GetPosition().x > m_Transform.translation.x) {
            m_RepulseDirection = -1; // 被玩家打，往左退
        } else {
            m_RepulseDirection = 1; // 被玩家打，往右退
        }
        m_RepulseSpeed = m_Speed * 5; // 給一個初速
        m_RepulseTimer = m_RepulseDuration;
        m_IsRepulsed = true;
    }

    void Update() {
        float currentTime = Util::Time::GetElapsedTimeMs(); // 取得遊戲運行時間的方法
        float deltaTime = Util::Time::GetDeltaTimeMs() / 1000.0f;

        if (m_IsRepulsed) {
            m_Transform.translation.x += m_RepulseDirection * m_RepulseSpeed * deltaTime;
            m_RepulseTimer -= deltaTime;
    
            if (m_RepulseTimer <= 0.0f) {
                m_IsRepulsed = false; // 結束擊退
                m_RepulseSpeed = 0.0f;
            }
        }

        float playerLeft   = TargetPlayer->GetPosition().x - TargetPlayer->GetWidth() / 2; //玩家的上下左右位置
        float playerRight  = TargetPlayer->GetPosition().x + TargetPlayer->GetWidth() / 2;
        float playerBottom = TargetPlayer->GetPosition().y - TargetPlayer->GetLength() / 2;
        float playerTop    = TargetPlayer->GetPosition().y + TargetPlayer->GetLength() / 2;

        float EnemyLeft   = m_Transform.translation.x - width/2; //玩家的上下左右位置
        float EnemyRight  = m_Transform.translation.x + width/2;
        float EnemyBottom = m_Transform.translation.y - length/2;
        float EnemyTop    = m_Transform.translation.y + length/2;

        bool InRange = CheckInRange(playerLeft,playerRight,playerBottom,playerTop,EnemyLeft,EnemyRight,EnemyBottom,EnemyTop);
        bool InAtkRange = CheckInAtkRange(playerLeft,playerRight,playerBottom,EnemyLeft,EnemyRight,EnemyBottom);

        if(InAtkRange) {
            if (currentTime - m_LastAttackTime >= m_AttackCooldown) {
                Attack();
                m_LastAttackTime = currentTime;
            }
        }
        else if(!InAtkRange){
            if (playerRight < EnemyLeft) Move(MoveDirection3::LEFT,1);
            if (playerLeft > EnemyRight) Move(MoveDirection3::RIGHT,1);
            if (playerTop >= EnemyTop) Move(MoveDirection3::UP,1);
            if (playerBottom <= EnemyBottom) Move(MoveDirection3::DOWN,1);
        }

        if(TargetPlayer->GetIsAttack()){
            if(InRange) Hurt(); 
        }
    }
    bool CheckInRange(float playerLeft, float playerRight, float playerBottom, float playerTop,
                      float EnemyLeft, float EnemyRight, float EnemyBottom, float EnemyTop){
        const float epsilon = 0.01f;

        if (playerRight >= EnemyLeft - epsilon && playerLeft <= EnemyRight + epsilon &&
            playerBottom <= EnemyTop + epsilon && playerTop >= EnemyBottom - epsilon){
                return true;
            }
        else {
            return false;
        }
    }
    bool CheckInAtkRange(float playerLeft, float playerRight, float playerBottom,
                         float EnemyLeft, float EnemyRight, float EnemyBottom){
        const float epsilon = 0.01f;

        if (playerRight >= EnemyLeft - epsilon && playerLeft <= EnemyRight + epsilon &&
            (EnemyBottom <= (playerBottom + 70.0f) && EnemyBottom >= (playerBottom - 70.0f)))
            {
                return true;
                LOG_DEBUG("inAtkRange");
            }
        else {
            return false;
        }
        }
private:
    std::shared_ptr<Player> TargetPlayer;

    Enemy3State m_CurrentState = Enemy3State::Move;
    std::shared_ptr<Util::Animation> m_MoveAnimation;
    std::shared_ptr<Util::Animation> m_MoveAnimationL;
    std::size_t currentIndex = 0;

    const float length = 64.0;
    const float width = 60.0;

    bool alive = true;
    bool faceRight = false;
    bool moving = false;

    float m_AttackCooldown = 3000; // 冷卻
    float m_LastAttackTime = 0.0f;

    bool m_IsRepulsed = false;
    float m_RepulseSpeed = 0.0f; // 被擊退的速度
    float m_RepulseTimer = 0.0f; // 被擊退的持續時間
    float m_RepulseDuration = 1.0f; // 擊退持續 0.2 秒
    int m_RepulseDirection = 1; // 左推(-1)右推(+1)

    void SetState(Enemy3State newState) {
        // LOG_DEBUG(m_AttackAnimation -> GetFrameCount());
        m_CurrentState = newState;
        SetAnimation(newState);
    }
    void SetAnimation(Enemy3State newState){
        switch (newState){
            case Enemy3State::Move:
                SetDrawable(faceRight ? m_MoveAnimation : m_MoveAnimationL);
                break;
            default:
                break;            
        }
    }
};

#endif