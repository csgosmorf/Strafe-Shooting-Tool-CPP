#pragma once
#ifndef STOPWATCH_H
#define STOPWATCH_H
#include <chrono>

//namespace cr = std::chrono;
//typedef std::chrono::high_resolution_clock clock;
//typedef std::chrono::time_point<std::chrono::steady_clock> time;

class Stopwatch
{
public:
	static const bool PIVOT = true;
	static const bool GUN = false;
	bool isRunning;
	bool isReset;
	bool starter;
	Stopwatch();
	long ms();
	void reset();
	void start(bool starterType);
	void stop();
	void renew(bool starterType);
private:
	std::chrono::time_point<std::chrono::steady_clock> start_time;
	std::chrono::time_point<std::chrono::steady_clock> stop_time;
	long prev_elapsed_time;
	long msSession();
};


#endif
