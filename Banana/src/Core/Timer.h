#pragma once
#include <chrono>

namespace Banana 
{
	/**
	 * @brief Simple timer/stopwatch class. The timer starts at creation and runs until it gets reset or destroyed.
	 */
	class Timer
	{
	public:
		/**
		 * @brief Creates and starts the timer.
		 */
		Timer();

		/**
		 * @brief Resets the timer.
		 */
		void Reset();

		/**
		 * @returns The ellapsed time since last reset in nanoseconds.
		 */
		float ElapsedNano();
		
		/**
		 * @returns The ellapsed time since last reset in milliseconds.
		 */
		float ElapsedMilli();
		
		/**
		 * @returns The ellapsed time since last reset in seconds.
		 */
		float ElapsedSeconds();

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_PauseStart;
	};

}