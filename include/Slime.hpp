#ifndef Slime_HPP
#define Slime_HPP

#include "pch.hpp"
#include "Character.hpp"
#include "Player.hpp"
#include "Tile.hpp"

#include <glm/fwd.hpp>
#include <memory>
#include <vector>

#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "Util/Animation.hpp"

enum class SlimeState {
    Move
};

enum class SlimeMoveDirection{
    LEFT,
    RIGHT
};

class Slime : public Character {
public:
    Slime()
    : Character(RESOURCE_DIR "/Image/Character/enemy4/L1.png", 100, 10, 50), 
    m_MoveAnimation(std::make_shared<Util::Animation>(
        std::vector<std::string>{
            RESOURCE_DIR "/Image/Character/enemy4/R1.png",
            RESOURCE_DIR "/Image/Character/enemy4/R2.png",
            RESOURCE_DIR "/Image/Character/enemy4/R3.png",
            RESOURCE_DIR "/Image/Character/enemy4/R4.png",
            RESOURCE_DIR "/Image/Character/enemy4/R5.png",
            RESOURCE_DIR "/Image/Character/enemy4/R6.png"
        },
        true, 250, true, 0)),
    m_MoveAnimationL(std::make_shared<Util::Animation>(
        std::vector<std::string>{
            RESOURCE_DIR "/Image/Character/enemy4/L1.png",
            RESOURCE_DIR "/Image/Character/enemy4/L2.png",
            RESOURCE_DIR "/Image/Character/enemy4/L3.png",
            RESOURCE_DIR "/Image/Character/enemy4/L4.png",
            RESOURCE_DIR "/Image/Character/enemy4/L5.png",
            RESOURCE_DIR "/Image/Character/enemy4/L6.png"
        },
        true, 250, true, 0))
    {        
    m_Transform.translation = {-200, 200};
    SetDrawable(m_MoveAnimation);
}
    void SetTarget(std::shared_ptr<Player> player) {
        TargetPlayer = player;
    }

    void ApplyGravity(float deltaTime) {
        if (!m_IsOnGround) {
            m_Velocity.y += m_Gravity * deltaTime; // 模擬重力
            m_Transform.translation.y -= m_Velocity.y * deltaTime + 0.5f * m_Gravity * deltaTime * deltaTime;
    
            // 確保角色不掉出畫面底部（假設地板在 y=-200.5）
            if (m_Transform.translation.y <= -420) {
                m_Transform.translation.y = -420;
                m_Velocity.y = 0;
                m_IsOnGround = true; // 修正這裡，應該讓角色回到地面
            }

            if (m_Transform.translation.y > 530) {
                m_Transform.translation.y = 530;
            }
        }
    }

    void repulsed(){
        if (TargetPlayer->GetPosition().x > m_Transform.translation.x) {
            m_RepulseDirection = -1; // 被玩家打，往左退
        } else {
            m_RepulseDirection = 1; // 被玩家打，往右退
        }
        m_RepulseSpeed = m_Speed * 20; // 給一個初速
        m_RepulseTimer = m_RepulseDuration;
        m_IsRepulsed = true;
    }

    void HandleCollision(const Tile* tile) { //玩家跟Tile的碰撞處理
        float Left   = m_Transform.translation.x - width/2; //玩家的上下左右位置
        float Right  = m_Transform.translation.x + width/2;
        float Bottom = m_Transform.translation.y - length/2;
        float Top    = m_Transform.translation.y + length/2;

        float tileLeft   = tile->m_Transform.translation.x - tile->GetWidth() / 2;
        float tileRight  = tile->m_Transform.translation.x + tile->GetWidth() / 2;
        float tileBottom = tile->m_Transform.translation.y - tile->GetLength() / 2;
        float tileTop    = tile->m_Transform.translation.y + tile->GetLength() / 2;

        const float epsilon = 0.01f;
    
        // 判斷是否與 Tile 發生碰撞
        if (Right >= tileLeft - epsilon && Left <= tileRight + epsilon &&
            Bottom <= tileTop + epsilon && Top >= tileBottom - epsilon){
    
            // 計算角色與 Tile 的重疊區域
            float overlapLeft   = Right - tileLeft;
            float overlapRight  = tileRight - Left;
            float overlapTop    = tileTop - Bottom;
            float overlapBottom = Top - tileBottom;
    
            // 找出最小的重疊方向
            if (overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight) {
                // **角色從上方掉到 Tile 上**
                m_Transform.translation.y = tileTop + length/2;
                m_Velocity.y = 0;
                m_IsOnGround = true;
            } 
            else if (overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight) {
                // **角色頭頂撞到 Tile 底部**
                m_Transform.translation.y = tileBottom - length/2;
                m_Velocity.y = 0;
            } 
            else if (overlapLeft < overlapRight) {
                // **角色從右側撞到 Tile**
                m_Transform.translation.x = tileLeft - width/2;
            } 
            else {
                // **角色從左側撞到 Tile**
                m_Transform.translation.x = tileRight + width/2;
            }
        }
    }

    float GetWidth() const { return width; }
    float GetLength() const { return length; }
    bool GetAlive() const { return alive; }

    void Move(SlimeMoveDirection Direction, int n){
        float deltaTime = Util::Time::GetDeltaTimeMs() / 1000.0f;
        switch (Direction){
            case SlimeMoveDirection::LEFT:
                m_Transform.translation.x = m_Transform.translation.x - m_Speed * n * deltaTime;
                faceRight = false;
                break;
            case SlimeMoveDirection::RIGHT:
                m_Transform.translation.x = m_Transform.translation.x + m_Speed * n * deltaTime;
                faceRight = true;
                break;
            default:
                break;
        }
        SetState(SlimeState::Move);   
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
    void Update(const std::vector<std::shared_ptr<Tile>>& tiles) {
        float currentTime = Util::Time::GetElapsedTimeMs(); // 取得遊戲運行時間的方法
        float deltaTime = Util::Time::GetDeltaTimeMs() / 1000.0f; 

        m_IsOnGround = false;

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

        float playerLeftWithoutWeapon = TargetPlayer->GetFaceRight()? playerLeft : (playerLeft + 31);
        float playerRightWithoutWeapon = TargetPlayer->GetFaceRight()? (playerRight - 31) : playerRight;

        bool InRange = CheckInRange(playerLeft,playerRight,playerBottom,playerTop,EnemyLeft,EnemyRight,EnemyBottom,EnemyTop);
        bool InAtkRange = CheckInAtkRange(playerLeftWithoutWeapon,playerRightWithoutWeapon,playerBottom,EnemyLeft,EnemyRight,EnemyBottom);

        if(InAtkRange) {
            if (currentTime - m_LastAttackTime >= m_AttackCooldown) {
                Attack();
                m_LastAttackTime = currentTime;
            }
        }
        else if(!InAtkRange){
            if (playerRightWithoutWeapon < EnemyLeft) Move(SlimeMoveDirection::LEFT,1);
            if (playerLeftWithoutWeapon > EnemyRight) Move(SlimeMoveDirection::RIGHT,1);
        }
        ApplyGravity(deltaTime);

        for (const auto& tile : tiles) {
            HandleCollision(tile.get());
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
                LOG_DEBUG("inAtkRange");
                return true;
            }
        else {
            return false;
        }
        }
private:
    std::shared_ptr<Player> TargetPlayer;

    SlimeState m_CurrentState = SlimeState::Move;
    std::shared_ptr<Util::Animation> m_MoveAnimation;
    std::shared_ptr<Util::Animation> m_MoveAnimationL;
    std::size_t currentIndex = 0;

    const float length = 87.0;
    const float width = 96.0;

    bool alive = true;
    bool faceRight = false;
    bool moving = false;

    float m_AttackCooldown = 3000; // 冷卻
    float m_LastAttackTime = 0.0f;

    bool m_IsOnGround = true;
    glm::vec2 m_Velocity = {0, 0};
    const float m_Gravity = 980.0f; 

    bool m_IsRepulsed = false;
    float m_RepulseSpeed = 0.0f; // 被擊退的速度
    float m_RepulseTimer = 0.0f; // 被擊退的持續時間
    float m_RepulseDuration = 0.2f; // 擊退持續 0.2 秒
    int m_RepulseDirection = 1; // 左推(-1)右推(+1)

    void SetState(SlimeState newState) {
        // LOG_DEBUG(m_AttackAnimation -> GetFrameCount());
        m_CurrentState = newState;
        SetAnimation(newState);
    }
    void SetAnimation(SlimeState newState){
        switch (newState){
            case SlimeState::Move:
                SetDrawable(faceRight ? m_MoveAnimation : m_MoveAnimationL);
                break;
            default:
                break;            
        }
    }
};

#endif