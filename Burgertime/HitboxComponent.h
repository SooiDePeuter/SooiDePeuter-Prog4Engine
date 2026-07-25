#include "BaseComponent.h"
#include "GameObject.h"
#include <string>

namespace dae
{
    class HitboxComponent final : public BaseComponent
    {
    public:
        void Update(float deltaTime)
        {
            deltaTime = 10;
        }
        void Render() const{}

        //takes dimensions of owner
        explicit HitboxComponent(GameObject* owner)
            :BaseComponent(owner)
        {
            m_PosX = owner->GetPosition().x;
            m_PosY = owner->GetPosition().y;
            m_Width = owner->GetTextureWidth();
            m_Height = owner->GetTextureHeight();
        }

        explicit HitboxComponent(GameObject* owner, float posX, float posY, float width, float height)
            :BaseComponent(owner), m_PosX{ posX }, m_PosY{ posY }, m_Width{ width }, m_Height{ height }
        {
        }

        float PosX() const
        {
            return m_Owner->GetPosition().x + m_PosX;
        }

        float PosY() const
        {
            return m_Owner->GetPosition().y + m_PosY;
        }

        float Width() const
        {
            return m_Width;
        }

        float Height() const
        {
            return m_Height;
        }

        bool IsOverlapping(const HitboxComponent& other) const
        {
            if (PosX() + Width() < other.PosX())
            {
                return false;
            }
            if (PosY() + Height() < other.PosY())
            {
                return false;
            }
            if (other.PosX() + other.Width() < PosX())
            {
                return false;
            }
            if (other.PosY() + other.Height() < PosY())
            {
                return false;
            }

            return true;
        }

    private:
        float m_PosX{};
        float m_PosY{};
        float m_Width{};
        float m_Height{};
    };
}