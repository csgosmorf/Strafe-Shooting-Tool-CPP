#include "Stopwatch.h"

Stopwatch::Stopwatch()
	: isRunning{ false }, prev_elapsed_time{ 0 }, starter{PIVOT}, isReset{true}
{
}

long Stopwatch::ms()
{
	return msSession() + prev_elapsed_time;
}

long Stopwatch::msSession()
{
	if (isReset) return 0;
	auto diff = (isRunning ? std::chrono::high_resolution_clock::now() : stop_time) - start_time;
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
	return (long)milliseconds.count();
}

void Stopwatch::reset()
{
	prev_elapsed_time = 0;
	isRunning = false;
	isReset = true;
}

void Stopwatch::start(bool starterType)
{
	start_time = std::chrono::high_resolution_clock::now();
	starter = starterType;
	isRunning = true;
	isReset = false;
}

void Stopwatch::stop() {
	prev_elapsed_time += msSession();
	stop_time = std::chrono::high_resolution_clock::now();
	isRunning = false;
}

void Stopwatch::renew(bool starterType)
{
	reset();
	start(starterType);
}
