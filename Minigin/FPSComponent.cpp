#include "FPSComponent.h"

#include "GameObject.h"
#include "TextComponent.h"
#include "Timer.h"
#include <cmath>
#include <sstream>
#include <iomanip>

dae::FPSComponent::FPSComponent(GameObject* owner)
    : BaseComponent(owner)
{

}

void dae::FPSComponent::Update(float deltaTime)
{
    if (!m_IsActive)
    {
        return;
    }

    deltaTime = Timer::GetInstance().GetDeltaTime();
    if (deltaTime <= 0.0f)
    {
        return;
    }
       
    m_frames++;
    m_accumulatedTime += deltaTime;

    if (m_accumulatedTime >= m_updateInterval)
    {
        m_averageFPS = static_cast<float>(m_frames) / m_accumulatedTime;

        m_frames = 0;
        m_accumulatedTime = 0.0f;

        UpdateText();
    }
}

void dae::FPSComponent::Render() const
{
}

void dae::FPSComponent::UpdateText()
{
    if (!m_text)
    {
        m_text = GetOwner()->GetComponent<TextComponent>();
        if (!m_text)
        {
            return;
        }
    }
 
    std::ostringstream ss;
    ss << "FPS: " << std::fixed << std::setprecision(1) << m_averageFPS;
    m_text->SetText(ss.str());
}

float dae::FPSComponent::GetFPS() const
{
    return m_averageFPS;
}