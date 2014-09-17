//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

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