#ifndef NPC1_HPP
#define NPC1_HPP

#include "GameObject2.hpp"
#include "pch.hpp"
#include "Player.hpp"

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <memory>
#include <vector>

#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include "Util/Animation.hpp"
#include "Util/Input.hpp"

class NPC1 : public GameObject2 {
    public:
        // NPC1() {
        //     SetImage(RESOURCE_DIR "/Image/Character/npc/NPC1.png");
        //     m_Transform.scale = {1.0f, 1.0f};
        // }
        NPC1(const glm::vec2& Position) 
        : GameObject2(
            std::make_unique<Util::Image>(RESOURCE_DIR "/Image/Character/npc/NPC1.png"), -9) {
            m_Transform.translation = Position;
        }
    
        void Update(std::shared_ptr<Player> player) {
            if (!player) return;
    
            float dist = glm::distance(player->GetPosition(), m_Transform.translation);
            if (dist < interactionDistance && Util::Input::IsKeyDown(Util::Keycode::J)) {
                LOG_DEBUG("Player interacted with NPC1");
                m_IsInteracted = true;
            } else {
                m_IsInteracted = false;
            }
        }
    
        bool IsInteracted() const { return m_IsInteracted; }
        void SetPosition(const glm::vec2& Position) { 
            m_Transform.translation = Position; 
        }
        void SetImage(const std::string& ImagePath) {
            LOG_DEBUG("SetImage");
            auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
            temp->SetImage(ImagePath);
        }
    
    private:
        bool m_IsInteracted = false;
        const float interactionDistance = 80.0f;
    };
    

#endif
