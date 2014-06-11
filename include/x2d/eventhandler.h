#ifndef X2D_EVENT_HANDLER_H
#define X2D_EVENT_HANDLER_H

#include <x2d/config.h>

class XDAPI xdEventHandler
{
	virtual void processEvents() = 0;
};

#endif // X2D_EVENT_HANDLER_H