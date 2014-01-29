#include "main.h"
#include "NetPlatform.h"
#include <Windows.h>

// Platform independent wait for n seconds
#if PLATFORM == PLATFORM_WINDOWS
void wait(float seconds)
{
	Sleep((int)(seconds * 1000.0f));
}
#else
void wait(float seconds) 
{
	usleep((int)(seconds * 1000000.0f));
}
#endif