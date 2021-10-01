#include "Timer.h"

Timer::Timer()
{
	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}

double Timer::GetMilisecondsElapsed()
{
	if (is_running)
	{
		auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start);
		return elapsed.count();
	}
	else
	{
		auto elapsed = std::chrono::duration<double, std::milli>(stop - start);
		return elapsed.count();
	}
}

void Timer::Restart()
{
	is_running = true;
	start = std::chrono::high_resolution_clock::now();
}

bool Timer::Stop()
{
	if (!is_running)
		return false;
	else
	{
		stop = std::chrono::high_resolution_clock::now();
		is_running = false;
		return true;
	}
}

bool Timer::Start()
{
	if (is_running)
	{
		return false;
	}
	else
	{
		start = std::chrono::high_resolution_clock::now();
		is_running = true;
		return true;
	}
}