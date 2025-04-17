#ifndef Renderer2_HPP
#define Renderer2_HPP

#include <memory>
#include <vector>
#include <queue>

#include "GameObject2.hpp"
#include "Camera.hpp"

class App;
/**
 * @class Renderer
 * @brief A class handling GameObjects' Draw()
 * @see Util::GameObject
 */
class Renderer2 {
public:
    /**
     * @brief Parameterized constructor.
     *`
     *
     * @param children The GameObject needing to be managed by Renderer.
     */
    Renderer2(const std::vector<std::shared_ptr<GameObject2>> &children)
     : m_Children(children), m_Camera(nullptr)  {}

     void SetCamera(const std::shared_ptr<Camera>& camera) {
        m_Camera = camera;
    }

    /**
     * @brief Add a child to Renderer.
     *
     * @param child The GameObject needing to be managed by Renderer.
     */
    void AddChild(const std::shared_ptr<GameObject2> &child) {
        m_Children.push_back(child);
    }

    /**
     * @brief Add children to Renderer.
     *
     * @param children The GameObjects needing to be managed by Renderer.
     */
    void AddChildren(
        const std::vector<std::shared_ptr<GameObject2>> &children) {
        m_Children.reserve(m_Children.size() + children.size());
        m_Children.insert(m_Children.end(), children.begin(), children.end());
    }

    /**
     * @brief Remove the child.
     *
     * @param child The GameObject being removed.
     */
    void RemoveChild(std::shared_ptr<GameObject2> child) {
        m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child),
                         m_Children.end());
    }

    /**
     * @brief Draw children according to their z-index.
     *
     * @note The user is not recommended to modify this function.
     */
    void Update() {
        struct StackInfo {
            std::shared_ptr<GameObject2> m_GameObject;
            Util::Transform m_ParentTransform;
        };
    
        std::vector<StackInfo> stack;
        stack.reserve(m_Children.size());
    
        for (const auto &child : m_Children) {
            stack.push_back(StackInfo{child, Util::Transform{}});
        }
    
        auto compareFunction = [](const StackInfo &a, const StackInfo &b) {
            return a.m_GameObject->GetZIndex() > b.m_GameObject->GetZIndex();
        };
        std::priority_queue<StackInfo, std::vector<StackInfo>,
                            decltype(compareFunction)>
            renderQueue(compareFunction);
    
        while (!stack.empty()) {
            auto curr = stack.back();
            stack.pop_back();
            renderQueue.push(curr);
    
            for (const auto &child : curr.m_GameObject->GetChildren()) {
                stack.push_back(
                    StackInfo{child, curr.m_GameObject->GetTransform()});
            }
        }

        glm::mat4 viewMatrix = (m_Camera) ? m_Camera->GetViewMatrix() : glm::mat4(1.0f);

        // draw all in render queue by order
        while (!renderQueue.empty()) {
            auto curr = renderQueue.top();
            renderQueue.pop();
    
            // 🔹 **讓 GameObject 套用 Camera 變換**
            curr.m_GameObject->Draw(viewMatrix);
        }
    }

private:
    std::vector<std::shared_ptr<GameObject2>> m_Children;
    std::shared_ptr<Camera> m_Camera; 
};

#endif
