#pragma once
#include <chrono>

namespace Banana 
{
	/**
	 * Simple timer/stopwatch class. The timer starts at creation and runs until it gets reset or destroyed.
	 */
	class Timer
	{
	public:
		/**
		 * Creates and starts the timer.
		 */
		Timer();

		/**
		 * Resets the timer.
		 */
		void Reset();

		/**
		 * The ellapsed time since last reset in nanoseconds.
		 */
		float ElapsedNano();
		
		/**
		 * The ellapsed time since last reset in milliseconds.
		 */
		float ElapsedMilli();
		
		/**
		 * The ellapsed time since last reset in seconds.
		 */
		float ElapsedSeconds();

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_PauseStart;
	};

}