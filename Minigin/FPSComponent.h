#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"
#include <string>

namespace dae
{
    class TextComponent;

    class FPSComponent final : public BaseComponent
    {
    public:
        explicit FPSComponent(GameObject* owner, std::string* task = nullptr);
        ~FPSComponent()
        {
            delete m_text;
        }

        void Update(float deltaTime) override;
        void Render() const override;

        float GetFPS() const;

    private:
        void UpdateText();

        TextComponent* m_text{};
        float m_averageFPS{ 0.0f };
        int m_frames{ 0 };

        float m_accumulatedTime{ 0.0f };
        float m_updateInterval{ 0.25f };
    };
}