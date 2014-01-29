#ifndef ANDROID_DEBUG_H
#define ANDROID_DEBUG_H

#include "x2d/debug.h"

class AndroidDebug : public X2DDebug
{
public:
	void print(const string &msg);
};

#endif // ANDROID_DEBUG_H