#include "android_app.h"
#include "android_engine.h"

#include "x2d/scripts.h"

#include <android/asset_manager.h>

AndroidApp::AndroidApp(AppManager *manager) :
	m_manager(manager)
{
}

void AndroidApp::initEvents()
{
	iosystem::success("** Init Events **");
	m_mouseMoveFunc = getScriptFuncHandle("void mouseMove(int x, int y)");
	m_mouseMoveBeginFunc = getScriptFuncHandle("void mouseMoveBegin(int x, int y)");
}

void AndroidApp::moveMouseBegin(const int x, const int y)
{
	if(!m_mouseMoveBeginFunc) return;
	startScriptFuncCall(m_mouseMoveBeginFunc);
	addScriptFuncArg((void*)&x, 4);
	addScriptFuncArg((void*)&y, 4);
	endScriptFuncCall();
}

void AndroidApp::moveMouse(const int x, const int y)
{
	if(!m_mouseMoveFunc) return;
	startScriptFuncCall(m_mouseMoveFunc);
	addScriptFuncArg((void*)&x, 4);
	addScriptFuncArg((void*)&y, 4);
	endScriptFuncCall();
}

void AndroidApp::processEvents()
{
}

void AndroidApp::exception(X2DRetCode error, const char* message)
{
	JNIEnv *env = m_manager->android->activity->env;
	env->ThrowNew(env->FindClass("java/lang/Exception"), message);
}

ulong AndroidApp::getProgramTime()
{
	return 0;
}

ulong AndroidApp::getTickCount()
{
	return 0;
}

void AndroidApp::startHrTimer()
{
}

float AndroidApp::getHrTime()
{
}