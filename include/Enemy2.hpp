#ifndef ENEMY2_HPP
#define ENEMY2_HPP

#include "pch.hpp"
#include "Character.hpp"
#include "Player.hpp"

#include <glm/fwd.hpp>
#include <memory>
#include <vector>

#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "Util/Animation.hpp"

enum class Enemy2State {
    Move,
    Attack
};

enum class MoveDirection{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Enemy2 : public Character {
public:
    Enemy2()
    : Character(RESOURCE_DIR "/Image/Character/enemy2/A1.png", 100, 10, 100), 
    m_MoveAnimation(std::make_shared<Util::Animation>(
        std::vector<std::string>{
            RESOURCE_DIR "/Image/Character/enemy2/A1.png",
        },
        true, 50, true, 0)),
    m_AttackAnimation(std::make_shared<Util::Animation>(
        std::vector<std::string>{
            RESOURCE_DIR "/Image/Character/enemy2/A1.png",
            RESOURCE_DIR "/Image/Character/enemy2/A2.png",
            RESOURCE_DIR "/Image/Character/enemy2/A3.png",
            RESOURCE_DIR "/Image/Character/enemy2/A4.png",
            RESOURCE_DIR "/Image/Character/enemy2/A5.png",
        },
        true, 500, true, 0)),
    m_MoveAnimationL(std::make_shared<Util::Animation>(
        std::vector<std::string>{
            RESOURCE_DIR "/Image/Character/enemy2/A1R.png",
        },
        true, 50, true, 0)),
    m_AttackAnimationL(std::make_shared<Util::Animation>(
        std::vector<std::string>{
            RESOURCE_DIR "/Image/Character/enemy2/A1R.png",
            RESOURCE_DIR "/Image/Character/enemy2/A2R.png",
            RESOURCE_DIR "/Image/Character/enemy2/A3R.png",
            RESOURCE_DIR "/Image/Character/enemy2/A4R.png",
            RESOURCE_DIR "/Image/Character/enemy2/A5R.png",
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

    void Move(MoveDirection Direction){
        float deltaTime = Util::Time::GetDeltaTimeMs() / 1000.0f;
        switch (Direction){
            case MoveDirection::UP:
                m_Transform.translation.y = m_Transform.translation.y + m_Speed * deltaTime;
                break;
            case MoveDirection::DOWN:
                m_Transform.translation.y = m_Transform.translation.y - m_Speed * deltaTime;
                break;
            case MoveDirection::LEFT:
                m_Transform.translation.x = m_Transform.translation.x - m_Speed * deltaTime;
                faceRight = false;
                break;
            case MoveDirection::RIGHT:
                m_Transform.translation.x = m_Transform.translation.x + m_Speed * deltaTime;
                faceRight = true;
                break;
            default:
                break;
        }
        SetState(Enemy2State::Move);   
    }

    void Attack(){
        LOG_DEBUG("Enemy Attack!!");

        // currentIndex = (faceRight? m_AttackAnimationL->GetCurrentFrameIndex() : m_AttackAnimation->GetCurrentFrameIndex());
        // if (currentIndex == 3){TargetPlayer->hurt();}
        TargetPlayer->hurt(m_ATK);

        SetState(Enemy2State::Attack);
    }
    void Hurt(){
        TakeDamage(TargetPlayer->GetATK());
        if (m_HP <= 0){
            Die();
        }
        else LOG_DEBUG("Enemy Hurt!!");
    }
    void Die(){
        alive = false;
        LOG_DEBUG("Enemy Die!!");
    }

    void Update() {
        float currentTime = Util::Time::GetElapsedTimeMs(); // 取得遊戲運行時間的方法

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
            // if (playerRight < EnemyLeft) Move(MoveDirection::LEFT);
            // if (playerLeft > EnemyRight) Move(MoveDirection::RIGHT);
            // if (playerBottom > EnemyTop) Move(MoveDirection::UP);
            // if (playerTop < EnemyBottom) Move(MoveDirection::DOWN);
            if (playerRight < EnemyLeft) Move(MoveDirection::LEFT);
            if (playerLeft > EnemyRight) Move(MoveDirection::RIGHT);
            if (playerTop >= EnemyTop) Move(MoveDirection::UP);
            if (playerBottom <= EnemyBottom) Move(MoveDirection::DOWN);
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
            (EnemyBottom <= (playerBottom + 20.0f) && EnemyBottom >= (playerBottom - 20.0f)))
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

    Enemy2State m_CurrentState = Enemy2State::Move;
    std::shared_ptr<Util::Animation> m_MoveAnimation;
    std::shared_ptr<Util::Animation> m_AttackAnimation;
    std::shared_ptr<Util::Animation> m_MoveAnimationL;
    std::shared_ptr<Util::Animation> m_AttackAnimationL;
    std::size_t currentIndex = 0;

    const float length = 127.0;
    const float width = 192.0;

    bool alive = true;
    bool faceRight = false;
    bool moving = false;
    bool attacking = false;

    float m_AttackCooldown = 3000; // 冷卻
    float m_LastAttackTime = 0.0f;

    void SetState(Enemy2State newState) {
        // LOG_DEBUG(m_AttackAnimation -> GetFrameCount());
        if (m_CurrentState == newState) return;
        m_CurrentState = newState;
        SetAnimation(newState);
    }
    void SetAnimation(Enemy2State newState){
        switch (newState){
            case Enemy2State::Move:
                SetDrawable(faceRight ? m_MoveAnimationL : m_MoveAnimation);
                break;
            case Enemy2State::Attack:
                SetDrawable(faceRight ? m_AttackAnimationL : m_AttackAnimation);  
                break;
            default:
                break;            
        }
    }
};

#endif