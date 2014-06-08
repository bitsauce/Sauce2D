#include "timer.h"

Timer::Timer() :
	freqency(1),
	counterStart(0)
{
}

// High-resolution timer
void Timer::start()
{
    QueryPerformanceFrequency((LARGE_INTEGER*)&freqency);
    QueryPerformanceCounter((LARGE_INTEGER*)&counterStart);
}

float Timer::getTime() const
{
    __int64 counter = 0;
    QueryPerformanceCounter((LARGE_INTEGER*)&counter);
    return float(counter-counterStart)/float(freqency);
}