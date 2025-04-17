#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "SDL_stdinc.h"
#include "pch.hpp"
#include "Character.hpp"
#include "Tile.hpp"
#include <Util/Keycode.hpp>
#include <glm/fwd.hpp>
#include <memory>
#include <vector>
#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "Util/Input.hpp"
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"
#include "GameObject2.hpp"

enum class PlayerState {
    Idle,
    Move,
    Jump,
    Attack,
    Hurt,
    Die
};

class Player : public Character {
public:
    Player()
        : Character(RESOURCE_DIR "/Image/Character/player/Idle/i0.png", 100, 20, 400), 
        m_IdleAnimation(std::make_shared<Util::Animation>(
            std::vector<std::string>{
                RESOURCE_DIR "/Image/Character/player/Idle/i0.png",
                RESOURCE_DIR "/Image/Character/player/Idle/i1.png",
            },
            true, 500, true, 500)),
        m_IdleAnimationL(std::make_shared<Util::Animation>(
            std::vector<std::string>{
                RESOURCE_DIR "/Image/Character/player/Idle/i0L.png",
                RESOURCE_DIR "/Image/Character/player/Idle/i1L.png",
            },
            true, 500, true, 500)),
        m_MoveAnimation(std::make_shared<Util::Animation>(
            std::vector<std::string>{
                RESOURCE_DIR "/Image/Character/player/Walk/w1.png",
                RESOURCE_DIR "/Image/Character/player/Walk/w2.png",
                RESOURCE_DIR "/Image/Character/player/Walk/w3.png",
            },
            true, 200, true, 0)),
        m_AttackAnimation(std::make_shared<Util::Animation>(
            std::vector<std::string>{
                RESOURCE_DIR "/Image/Character/player/Attack/a1.png",
                RESOURCE_DIR "/Image/Character/player/Attack/a2.png",
                RESOURCE_DIR "/Image/Character/player/Attack/a3.png",
                RESOURCE_DIR "/Image/Character/player/Attack/a4.png",
                RESOURCE_DIR "/Image/Character/player/Attack/a5.png",
                RESOURCE_DIR "/Image/Character/player/Attack/a6.png",
                RESOURCE_DIR "/Image/Character/player/Attack/a7.png",
                RESOURCE_DIR "/Image/Character/player/Attack/a8.png",
            },
            true, 40, true, 0)),
        m_JumpAnimation(std::make_shared<Util::Animation>(
            std::vector<std::string>{
                RESOURCE_DIR "/Image/Character/player/Jump/j1.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j2.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j3.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j4.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j5.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j6.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j7.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j8.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j9.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j10.png",
            },
            true, 60, true, 0)),
        m_JumpAnimationL(std::make_shared<Util::Animation>(
            std::vector<std::string>{
                RESOURCE_DIR "/Image/Character/player/Jump/j1L.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j2L.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j3L.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j4L.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j5L.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j6L.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j7L.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j8L.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j9L.png",
                RESOURCE_DIR "/Image/Character/player/Jump/j10L.png",
            },
            true, 60, true, 0)),    
        m_MoveAnimationL(std::make_shared<Util::Animation>(
                std::vector<std::string>{
                    RESOURCE_DIR "/Image/Character/player/Walk/w1L.png",
                    RESOURCE_DIR "/Image/Character/player/Walk/w2L.png",
                    RESOURCE_DIR "/Image/Character/player/Walk/w3L.png",
            },
            true, 200, true, 0)),
        m_AttackAnimationL(std::make_shared<Util::Animation>(
                std::vector<std::string>{
                    RESOURCE_DIR "/Image/Character/player/Attack/a1L.png",
                    RESOURCE_DIR "/Image/Character/player/Attack/a2L.png",
                    RESOURCE_DIR "/Image/Character/player/Attack/a3L.png",
                    RESOURCE_DIR "/Image/Character/player/Attack/a4L.png",
                    RESOURCE_DIR "/Image/Character/player/Attack/a5L.png",
                    RESOURCE_DIR "/Image/Character/player/Attack/a6L.png",
                    RESOURCE_DIR "/Image/Character/player/Attack/a7L.png",
                    RESOURCE_DIR "/Image/Character/player/Attack/a8L.png",
            },
            true, 40, true, 0)) {
        m_Transform.translation = {-200, 200};
        SetDrawable(m_IdleAnimation);
    }

    void MoveLeft() {
        float deltaTime = Util::Time::GetDeltaTimeMs() / 1000.0f;
        if (m_Transform.translation.x - m_Speed * deltaTime < MaxLeft){
            m_Transform.translation.x = MaxLeft;
        }
        else{
            m_Transform.translation.x -= m_Speed * deltaTime;
        }
        SetState(PlayerState::Move);
    }

    void MoveRight() {
        float deltaTime = Util::Time::GetDeltaTimeMs() / 1000.0f;
        if (m_Transform.translation.x + m_Speed * deltaTime > MaxRight){
            m_Transform.translation.x = MaxRight;
        }
        else{
            m_Transform.translation.x += m_Speed * deltaTime;
        }
        SetState(PlayerState::Move);
    }

    void Jump() {
        m_Velocity.y = m_JumpVelocity; // 使用正確的跳躍速度變數
        SetState(PlayerState::Jump);
    }
    
    void ApplyGravity(float deltaTime) {
        if (!m_IsOnGround) {
            m_Velocity.y += m_Gravity * deltaTime; // 模擬重力
            m_Transform.translation.y -= m_Velocity.y * deltaTime + 0.5f * m_Gravity * deltaTime * deltaTime;
    
            // 確保角色不掉出畫面底部（假設地板在 y=-200.5）
            if (m_Transform.translation.y <= MaxBottom) {
                m_Transform.translation.y = MaxBottom;
                m_Velocity.y = 0;
                m_IsOnGround = true; // 修正這裡，應該讓角色回到地面
            }

            if (m_Transform.translation.y > MaxTop) {
                m_Transform.translation.y = MaxTop;
            }
        }
    }
    void Setboundary(float Left, float Right, float Top, float Bottom){
        MaxLeft = Left;
        MaxRight = Right;
        MaxTop = Top;
        MaxBottom = Bottom;
    }

    float GetWidth() const { return weight; }
    float GetLength() const { return height; }
    bool GetIsAttack() const { return Is_Attack; }
    bool GetAlive() const {return alive; }
    bool GetFaceRight() const {return faceRight; }

    float m_AttackCooldown = 500; // 冷卻
    float m_LastAttackTime = 0.0f;
    void Attack() {
        float currentTime = Util::Time::GetElapsedTimeMs(); // 取得遊戲運行時間的方法
        if (currentTime - m_LastAttackTime >= m_AttackCooldown) {
            SetState(PlayerState::Attack);
            m_LastAttackTime = currentTime;
            Is_Attack = true;
            LOG_DEBUG("Player Attack!!");
        }
    }
    
    void hurt(int damage){
        TakeDamage(damage);
        if (m_HP <= 0){
            Die();
        }
    }

    void Die(){
        alive = false;
    }

    void HandleCollision(const Tile* tile) { //玩家跟Tile的碰撞處理
        // float playerLeft   = m_Transform.translation.x - weight/2; //玩家的上下左右位置
        // float playerRight  = m_Transform.translation.x + weight/2;
        // float playerBottom = m_Transform.translation.y - height/2;
        // float playerTop    = m_Transform.translation.y + height/2;
        float playerLeft   = m_Transform.translation.x - weight; //玩家的上下左右位置
        float playerRight  = m_Transform.translation.x ;
        float playerBottom = m_Transform.translation.y - height;
        float playerTop    = m_Transform.translation.y ;

        float tileLeft   = tile->m_Transform.translation.x - tile->GetWidth() / 2;
        float tileRight  = tile->m_Transform.translation.x + tile->GetWidth() / 2;
        float tileBottom = tile->m_Transform.translation.y - tile->GetLength() / 2;
        float tileTop    = tile->m_Transform.translation.y + tile->GetLength() / 2;

        const float epsilon = 0.01f;
    
        // 判斷是否與 Tile 發生碰撞
        if (playerRight >= tileLeft - epsilon && playerLeft <= tileRight + epsilon &&
            playerBottom <= tileTop + epsilon && playerTop >= tileBottom - epsilon){
    
            // 計算角色與 Tile 的重疊區域
            float overlapLeft   = playerRight - tileLeft;
            float overlapRight  = tileRight - playerLeft;
            float overlapTop    = tileTop - playerBottom;
            float overlapBottom = playerTop - tileBottom;
    
            // 找出最小的重疊方向
            if (overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight) {
                // **角色從上方掉到 Tile 上**
                m_Transform.translation.y = tileTop + height;
                m_Velocity.y = 0;
                m_IsOnGround = true;
            } 
            else if (overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight) {
                // **角色頭頂撞到 Tile 底部**
                m_Transform.translation.y = tileBottom ;
                m_Velocity.y = 0;
            } 
            else if (overlapLeft < overlapRight) {
                // **角色從右側撞到 Tile**
                m_Transform.translation.x = tileLeft;
            } 
            else {
                // **角色從左側撞到 Tile**
                m_Transform.translation.x = tileRight + weight/2;
            }
        }
    }
    

    // void Update(const std::vector<std::shared_ptr<Tile>>& tiles) {
    //     float deltaTime = Util::Time::GetDeltaTimeMs() / 1000.0f; 

    //     // 重設角色狀態
    //     Is_Attack = false;
    //     m_IsOnGround = false;
    
    //     if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
    //         faceRight = false;
    //         MoveLeft();
    //     }
    //     if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
    //         faceRight = true;
    //         MoveRight();
    //     }
    //     if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
    //         LOG_DEBUG("Jump!!!");
    //         Jump();
    //     }
    //     if (Util::Input::IsKeyDown(Util::Keycode::J)) {
    //         Attack();
    //     }
    
    //     ApplyGravity(deltaTime);

    //     // 處理玩家與地板的碰撞
    //     for (const auto& tile : tiles) {
    //         HandleCollision(tile.get());
    //     }
    
    //     // 如果沒按移動鍵且沒攻擊，則進入 Idle
    //     if (!Util::Input::IsKeyPressed(Util::Keycode::A) &&
    //         !Util::Input::IsKeyPressed(Util::Keycode::D) &&
    //         !Util::Input::IsKeyPressed(Util::Keycode::J) &&
    //         !Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
    //         SetState(PlayerState::Idle);
    //     }
    // }
    void Update(const std::vector<std::shared_ptr<Tile>>& tiles) {
        float deltaTime = Util::Time::GetDeltaTimeMs() / 1000.0f;
        Uint32 now = SDL_GetTicks();
    
        ApplyGravity(deltaTime);
        m_IsOnGround = false;
    
        // 先處理攻擊狀態：不能被打斷
        if (m_CurrentState == PlayerState::Attack) {
            if (now - attackStartTime >= attackDuration) {
                m_CurrentState = PlayerState::Idle;
                SetDrawable(faceRight ? m_IdleAnimation : m_IdleAnimationL);
            }
            // 即使攻擊中，也需要處理地板碰撞
            for (const auto& tile : tiles) {
                HandleCollision(tile.get());
            }
            return; // 攻擊時不能移動、跳躍
        }
    
        // 判斷移動
        bool isMoving = false;
        if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
            faceRight = false;
            MoveLeft();
            isMoving = true;
        }
        if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
            faceRight = true;
            MoveRight();
            isMoving = true;
        }
    
        // 跳躍（只有在地板上才能跳）
        if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
            LOG_DEBUG("Jump!");
            Jump();
            m_CurrentState = PlayerState::Jump;
            SetDrawable(faceRight ? m_JumpAnimation : m_JumpAnimationL);
        }
    
        // 攻擊（只能在地板上攻擊）
        if (Util::Input::IsKeyDown(Util::Keycode::J)) {
            LOG_DEBUG("Attack!");
            Attack();
            m_CurrentState = PlayerState::Attack;
            attackStartTime = now;
            SetDrawable(faceRight ? m_AttackAnimation : m_AttackAnimationL);
            return;
        }
    
        // 處理玩家與地板的碰撞
        for (const auto& tile : tiles) {
            HandleCollision(tile.get());
        }
    
        // 更新狀態與動畫
        if (m_CurrentState != PlayerState::Jump) {
            if (isMoving) {
                m_CurrentState = PlayerState::Move;
                SetDrawable(faceRight ? m_MoveAnimation : m_MoveAnimationL);
            } else {
                m_CurrentState = PlayerState::Idle;
                SetDrawable(faceRight ? m_IdleAnimation : m_IdleAnimationL);
            }
        }
    
        // 落地後回到 idle
        if (m_CurrentState == PlayerState::Jump) {
            m_CurrentState = PlayerState::Idle;
            SetDrawable(faceRight ? m_IdleAnimation : m_IdleAnimationL);
        }
    }
    

private:
    int CurrentFrameIndex = 0;
    int FrameCount = 0;
    PlayerState m_CurrentState = PlayerState::Idle;
    std::shared_ptr<Util::Animation> m_IdleAnimation;
    std::shared_ptr<Util::Animation> m_IdleAnimationL;
    std::shared_ptr<Util::Animation> m_MoveAnimation;
    std::shared_ptr<Util::Animation> m_AttackAnimation;
    std::shared_ptr<Util::Animation> m_JumpAnimation;
    std::shared_ptr<Util::Animation> m_JumpAnimationL;
    std::shared_ptr<Util::Animation> m_MoveAnimationL;
    std::shared_ptr<Util::Animation> m_AttackAnimationL;

    // std::shared_ptr<Enemy> m_TargetEnemy;

    bool alive = true;
    bool Is_Attack = false;
    Uint32 attackStartTime = 0;
    Uint32 attackDuration = 320; // 8 張圖 * 40ms

    bool faceRight = true;
    //bool m_IsOnGround = true;
    glm::vec2 m_Velocity = {0, 0};
    int coin = 0; // 金幣數

    bool m_IsOnGround = false;
    bool m_IsJumping = false;

    // 物理屬性
    const float m_Gravity = 980.0f;       // 重力加速度
    const float m_JumpVelocity = -700.0f; // 跳躍初速度

    const float height = 142.0; //角色的長寬
    const float weight = 125.0; 

    const float AttackRange = 107;

    float MaxLeft,MaxRight,MaxTop,MaxBottom;


    void SetState(PlayerState newState) {
        if (m_CurrentState == newState) return;
        m_CurrentState = newState;
        SetAnimation(newState);
    }
    void SetAnimation(PlayerState newState){
        switch (newState) {
            case PlayerState::Idle:
                if(faceRight) SetDrawable(m_IdleAnimation);
                else SetDrawable(m_IdleAnimationL);
                FrameCount = m_IdleAnimation->GetFrameCount();
                CurrentFrameIndex = m_IdleAnimation->GetCurrentFrameIndex();
                break;
            case PlayerState::Move:
                    if(faceRight) SetDrawable(m_MoveAnimation);
                    else SetDrawable(m_MoveAnimationL);
                break;
            case PlayerState::Attack:
                if(faceRight) {
                    m_AttackAnimation->SetCurrentFrame(0);
                    SetDrawable(m_AttackAnimation);
                    }
                else {
                    m_AttackAnimationL->SetCurrentFrame(0); // 撥放前重設動畫
                    SetDrawable(m_AttackAnimationL);
                    }
                FrameCount = m_AttackAnimation->GetFrameCount();
                CurrentFrameIndex = m_AttackAnimation->GetCurrentFrameIndex();
                break;
            case PlayerState::Jump:
                if(faceRight) SetDrawable(m_JumpAnimation);
                else SetDrawable(m_JumpAnimationL);
                break;
            default:
                break;
        }
    }
};

#endif // PLAYER_HPP