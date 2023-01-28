#include "pch.h"
#include "Timer.h"


double Timer::getDeltaTime()
{
    if (isRunning)
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

void Timer::ReStart()
{
    isRunning = true;
    start = std::chrono::high_resolution_clock::now();
}

bool Timer::Start()
{
    if (isRunning)
    {
        return false;
    }
    else
    {
        this->start = std::chrono::high_resolution_clock::now();
        this->isRunning = true;
        return true;
    }
}

bool Timer::Stop()
{
    if (isRunning)
    {
        this->stop = std::chrono::high_resolution_clock::now();
        this->isRunning = false;
        return true;
    }
    else
    {
        return false;
    }
}
