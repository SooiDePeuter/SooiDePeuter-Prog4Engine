#pragma once
#include "Singleton.h"

namespace dae
{
	class Timer final : public Singleton<Timer>
	{
	public:

		void Tick();
		void Reset();
		uint64_t getTime() const;
		uint64_t getPreviousTime() const;
		float GetDeltaTime() const;

		Timer() = default;
		~Timer() = default;
		Timer(const Timer& other) = delete;
		Timer(Timer&& other) = delete;
		Timer& operator=(const Timer& other) = delete;
		Timer& operator=(Timer&& other) = delete;

	private:

		uint64_t m_TotalTime{};
		uint64_t m_PreviousTime{};

		float m_DeltaTime{};

		double m_invFrequency{ 0 };

	};
}