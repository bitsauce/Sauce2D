#ifndef X2D_TIMER_H
#define X2D_TIMER_H

#include <x2d/config.h>
#include <x2d/engine.h>
#include <x2d/console.h>
#include <x2d/math.h>
#include <x2d/base.h>

class XDAPI xdTimer
{
public:
	AS_DECL_SINGLETON

	xdTimer();
	
	// High-resolution timing
	virtual void  start()			= 0;
	virtual float getTime() const	= 0;

	// System clock
	void getTicksPerSecond();
	long getTickCount() const;

private:
	int m_ticksPerSec;
};

#endif // X2D_TIMER_H