//     _____                        ______             _            
//    / ____|                      |  ____|           (_)           
//   | (___   __ _ _   _  ___ ___  | |__   _ __   __ _ _ _ __   ___ 
//    \___ \ / _` | | | |/ __/ _ \ |  __| | '_ \ / _` | | '_ \ / _ \
//    ____) | (_| | |_| | (_|  __/ | |____| | | | (_| | | | | |  __/
//   |_____/ \__,_|\__,_|\___\___| |______|_| |_|\__, |_|_| |_|\___|
//                                                __/ |             
//                                               |___/              
// Made by Marcus "Bitsauce" Loo Vergara
// 2011-2018 (C)

#include <Sauce/Common.h>

BEGIN_SAUCE_NAMESPACE

Timer::Timer() :
	m_running(false)
{
    QueryPerformanceFrequency(&m_frequency);
	m_start.QuadPart = m_end.QuadPart = 0;
}

void Timer::start()
{
	m_running = true;
	QueryPerformanceCounter(&m_start);
}

void Timer::stop()
{
	QueryPerformanceCounter(&m_end);
	m_running = false;
}

double Timer::getElapsedTime() const
{
	LARGE_INTEGER end = m_end;
    if(m_running)
	{
        QueryPerformanceCounter(&end);
	}

	double startTime = m_start.QuadPart * (1.0 / m_frequency.QuadPart);
	double endTime = end.QuadPart * (1.0 / m_frequency.QuadPart);
    return endTime - startTime;
}

SimpleTimer::SimpleTimer() :
	m_startTick(0),
	m_running(false)
{
}

void SimpleTimer::start()
{
	m_running = true;
	m_startTick = clock();
}

float SimpleTimer::stop()
{
	if(!m_running)
	{
		return 0.0f;
	}
	m_running = false;
	return getElapsedTime();
}

float SimpleTimer::getElapsedTime() const
{
	return float(clock() - m_startTick) / float(CLOCKS_PER_SEC);
}

END_SAUCE_NAMESPACE
