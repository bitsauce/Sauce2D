//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>

XTimer::XTimer() :
	m_ticksPerSec(CLOCKS_PER_SEC),
	m_running(false)
{
    QueryPerformanceFrequency(&m_frequency);
	m_start.QuadPart = m_end.QuadPart = 0;
}

long XTimer::getTickCount() const
{
	return clock();
}

void XTimer::start()
{
	m_running = true;
	QueryPerformanceCounter(&m_start);
}

void XTimer::stop()
{
	QueryPerformanceCounter(&m_end);
	m_running = false;
}

float XTimer::getElapsedTime() const
{
	LARGE_INTEGER end = m_end;
    if(m_running)
	{
        QueryPerformanceCounter(&end);
	}

	float startTime = m_start.QuadPart * (1000.0f / m_frequency.QuadPart);
	float endTime = end.QuadPart * (1000.0f / m_frequency.QuadPart);
    return endTime - startTime;
}