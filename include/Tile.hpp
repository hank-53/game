#ifndef TILE_HPP
#define TILE_HPP

#include "GameObject2.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Transform.hpp"

#include <memory>
#include <string>
#include "Util/Logger.hpp"
#include "SDL_rect.h"


class Tile : public GameObject2
{
public:
    Tile(const glm::vec2& Position) 
        : GameObject2(
            std::make_unique<Util::Image>(RESOURCE_DIR"/Image/Background/tile.png"), -9) {
        m_Transform.translation = Position;
        }

    void SetImage(const std::string& ImagePath) {
        LOG_DEBUG("SetImage");
        auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        temp->SetImage(ImagePath);
    }

    void SetPosition(const glm::vec2& Position) { 
        m_Transform.translation = Position; 
    }

    void SetLengthWidth(float Length, float Width){
        this->length = Length;
        this->width = Width;
    }

    glm::vec2 GetPosition() const { return m_Transform.translation; }

    float GetWidth() const { return width; }
    float GetLength() const { return length; }

protected:
    std::string m_ImagePath;
    // Util::Transform m_Transform;

    float length;
    float width;

};

#endif //TILE_HPP