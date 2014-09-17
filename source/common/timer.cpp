//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>

AS_REG_SINGLETON(XTimer)

int XTimer::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	// Timer
	r = scriptEngine->RegisterObjectMethod("XTimer", "uint64 getTickCount() const", asMETHOD(XTimer, getTickCount), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("XTimer", "const int ticksPerSec", offsetof(XTimer, m_ticksPerSec)); AS_ASSERT
	
	return r;
}

XTimer::XTimer() :
	m_ticksPerSec(CLOCKS_PER_SEC)
{
}

long XTimer::getTickCount() const
{
	return clock();
}