#pragma once
#include <chrono>

namespace Banana 
{
	// Really simple timer/stopwatch class, might add a few other methods in the future
	class Timer
	{
	public:
		Timer();
		void Reset();
		float ElapsedNano();
		float ElapsedMilli();
		float ElapsedSeconds();

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_PauseStart;
	};

}