#ifndef WIN_TIMER_H
#define WIN_TIMER_H

#include <x2d/timer.h>

class Timer : public xdTimer
{
public:
	Timer();

	void start();
	float getTime() const;

private:
	__int64 freqency;
	__int64 counterStart;
};

#endif // WIN_TIMER_H