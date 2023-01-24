#pragma once
#include "pch.h"
#include <chrono>

class Timer
{
private:
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
	bool isRunning = false;

public:
	double getDeltaTime();

	void ReStart();
	bool Start();
	bool Stop();
	
};

