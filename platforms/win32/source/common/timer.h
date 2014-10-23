#ifndef WIN_TIMER_H
#define WIN_TIMER_H

#include <x2d/x2d.h>

class Timer : public XTimer
{
public:
	Timer();

	void start();
	void stop();

	float getElapsedTime() const;

private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_start;
	LARGE_INTEGER m_end;
	bool m_running;
};

#endif // WIN_TIMER_H