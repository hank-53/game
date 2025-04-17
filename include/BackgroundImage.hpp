#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Transform.hpp"
#include "Util/Logger.hpp"

#include "GameObject2.hpp"

#include <vector>
#include <string>

class BackgroundImage : public GameObject2 {

public:
    BackgroundImage() : GameObject2(
            std::make_unique<Util::Image>(RESOURCE_DIR"/Image/Background/phase0.png"), -10) {
    }

    void Set_Background(const int phase) {
        auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        temp->SetImage(ImagePath(phase));
    }

private:
    inline std::string ImagePath(const int phase) {
        return RESOURCE_DIR"/Image/Background/phase" + std::to_string(phase) + ".png";
    }
};

#endif //BACKGROUND_IMAGE_HPP
