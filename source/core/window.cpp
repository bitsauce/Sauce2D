//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/window.h>

AS_REG_SINGLETON(xdWindow, "ScriptWindow")

int xdWindow::Register(asIScriptEngine *scriptEngine)
{
	int r;

	// Fullscreen and resolution
	r = scriptEngine->RegisterObjectMethod("ScriptWindow", "void enableFullscreen()", asMETHOD(xdWindow, enableFullscreen), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptWindow", "void disableFullscreen()", asMETHOD(xdWindow, disableFullscreen), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptWindow", "array<Vector2i> @getResolutionList()", asMETHOD(xdWindow, getResolutionList), asCALL_THISCALL); AS_ASSERT

	// Window flags
	r = scriptEngine->RegisterObjectMethod("ScriptWindow", "void enableResize()", asMETHOD(xdWindow, enableResize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptWindow", "void disableResize()", asMETHOD(xdWindow, disableResize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptWindow", "bool hasFocus()", asMETHOD(xdWindow, hasFocus), asCALL_THISCALL); AS_ASSERT

	// Window actions
	r = scriptEngine->RegisterObjectMethod("ScriptWindow", "void     setPosition(const Vector2i &in)", asMETHOD(xdWindow, setPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptWindow", "Vector2i getPosition() const", asMETHOD(xdWindow, getPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptWindow", "void     setSize(const Vector2i &in)", asMETHOD(xdWindow, setSize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptWindow", "Vector2i getSize() const", asMETHOD(xdWindow, getSize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptWindow", "void     minimize()", asMETHOD(xdWindow, minimize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptWindow", "void     maximize()", asMETHOD(xdWindow, maximize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptWindow", "void     restore()", asMETHOD(xdWindow, restore), asCALL_THISCALL); AS_ASSERT

	// Dialog
	//AS_SINGLETON_FUNCTION(xdWindow, "string showSaveDialog(const string &in, const string &in, const string &in, const string &in)", showSaveDialog, app)
	//AS_SINGLETON_FUNCTION(xdWindow, "string showOpenDialog(const string &in, const string &in, const string &in, const string &in)", showOpenDialog, app)

	return r;
}