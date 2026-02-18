#include <SDL3/SDL.h>
#include "Timer.h"

void dae::Timer::Tick()
{
    const Uint64 time{ SDL_GetPerformanceCounter() };

    //calculate delta time in seconds
    const double deltaTime = static_cast<double>((time - m_PreviousTime) * m_invFrequency);
    
    //store values
    m_PreviousTime = time;
    m_DeltaTime = static_cast<float>(deltaTime);
    m_TotalTime += static_cast<Uint64>(deltaTime);

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
