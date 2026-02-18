#pragma once
#include "Singleton.h"

namespace dae
{
	class Timer final : public Singleton<Timer>
	{
	public:

		void Tick();
		Uint64 getTime() const;
		Uint64 getPreviousTime() const;
		float GetDeltaTime() const;

		Timer() = default;
		~Timer() = default;
		Timer(const Timer& other) = delete;
		Timer(Timer&& other) = delete;
		Timer& operator=(const Timer& other) = delete;
		Timer& operator=(Timer&& other) = delete;

	private:

		Uint64 m_TotalTime{};
		Uint64 m_PreviousTime{};

		float m_DeltaTime{};

		Uint64 m_invFrequency{ 1 / SDL_GetPerformanceFrequency() };

	};
}