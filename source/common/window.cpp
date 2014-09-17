//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>

AS_REG_SINGLETON(XWindow)

int XWindow::Register(asIScriptEngine *scriptEngine)
{
	int r;

	// Fullscreen and resolution
	r = scriptEngine->RegisterObjectMethod("XWindow", "void enableFullscreen()", asMETHOD(XWindow, enableFullscreen), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XWindow", "void disableFullscreen()", asMETHOD(XWindow, disableFullscreen), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XWindow", "bool isFullscreen() const", asMETHOD(XWindow, isFullscreen), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XWindow", "array<Vector2i> @getResolutionList() const", asMETHOD(XWindow, getResolutionList), asCALL_THISCALL); AS_ASSERT

	// Window flags
	r = scriptEngine->RegisterObjectMethod("XWindow", "void enableResize()", asMETHOD(XWindow, enableResize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XWindow", "void disableResize()", asMETHOD(XWindow, disableResize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XWindow", "bool hasFocus()", asMETHOD(XWindow, hasFocus), asCALL_THISCALL); AS_ASSERT

	// Window actions
	r = scriptEngine->RegisterObjectMethod("XWindow", "void     setPosition(const Vector2i &in)", asMETHOD(XWindow, setPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XWindow", "Vector2i getPosition() const", asMETHOD(XWindow, getPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XWindow", "void     setSize(const Vector2i &in)", asMETHOD(XWindow, setSize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XWindow", "Vector2i getSize() const", asMETHOD(XWindow, getSize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XWindow", "void     minimize()", asMETHOD(XWindow, minimize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XWindow", "void     maximize()", asMETHOD(XWindow, maximize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XWindow", "void     restore()", asMETHOD(XWindow, restore), asCALL_THISCALL); AS_ASSERT

	// Dialog
	//AS_SINGLETON_FUNCTION(XWindow, "string showSaveDialog(const string &in, const string &in, const string &in, const string &in)", showSaveDialog, app)
	//AS_SINGLETON_FUNCTION(XWindow, "string showOpenDialog(const string &in, const string &in, const string &in, const string &in)", showOpenDialog, app)

	return r;
}