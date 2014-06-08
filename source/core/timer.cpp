//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/timer.h>
#include <ctime>

AS_REG_SINGLETON(xdTimer, "ScriptTimer")

int xdTimer::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	// Timer
	r = scriptEngine->RegisterObjectMethod("ScriptTimer", "uint64 getTickCount() const", asMETHOD(xdTimer, getTickCount), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("ScriptTimer", "const int ticksPerSec", offsetof(xdTimer, m_ticksPerSec)); AS_ASSERT
	
	return r;
}

xdTimer::xdTimer() :
	m_ticksPerSec(CLOCKS_PER_SEC)
{
}

long xdTimer::getTickCount() const
{
	return clock();
}