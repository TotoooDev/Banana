#include <Core/Timer.h>

namespace Banana
{
	Timer::Timer()
	{
		Reset();
	}

	void Timer::Reset()
	{
		m_Start = std::chrono::high_resolution_clock::now();
	}

	float Timer::ElapsedNano()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count();
	}

	float Timer::ElapsedMilli()
	{
		return ElapsedNano() * 1.0e-6;
	}

	float Timer::ElapsedSeconds()
	{
		return ElapsedNano() * 1.0e-9;
	}
}