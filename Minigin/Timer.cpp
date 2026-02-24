#include <SDL3/SDL.h>
#include "Timer.h"

void dae::Timer::Tick()
{
    const Uint64 time{ SDL_GetPerformanceCounter() };

    //calculate delta time in seconds
    const std::uint64_t frequency = SDL_GetPerformanceFrequency();
    const double deltaTime = static_cast<double>((time - m_PreviousTime) / static_cast<double>(frequency));
    
    //store values
    m_PreviousTime = time;
    m_DeltaTime = static_cast<float>(deltaTime);
    m_TotalTime += static_cast<Uint64>(deltaTime);
}

void dae::Timer::Reset()
{
    const std::uint64_t frequency = SDL_GetPerformanceFrequency();
    m_invFrequency = 1.0 / static_cast<double>(frequency);

    m_PreviousTime = SDL_GetPerformanceCounter();
    m_DeltaTime = 0.f;
    m_TotalTime = 0;
}

Uint64 dae::Timer::getTime() const
{
    return m_TotalTime;
}

Uint64 dae::Timer::getPreviousTime() const
{
    return m_PreviousTime;
}

float dae::Timer::GetDeltaTime() const
{
    return m_DeltaTime;
}
