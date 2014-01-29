//     _                                     ____  ____  
//    / \  _   _ _ __ ___  _ __ __ _  __  __|___ \|  _ \ 
//   / _ \| | | |  __/ _ \|  __/ _  | \ \/ /  __) | | | |
//  / ___ \ |_| | | | (_) | | | (_| |  >  <  / __/| |_| |
// /_/   \_\__ _|_|  \___/|_|  \__ _| /_/\_\|_____|____/ 
//		MixedGraphics (C)

#include "x2d/platform.h"
#include "x2d/app.h"
#include "x2d/sfx.h"
#include "x2d/gfx.h"
#include "x2d/engine.h"
#include "x2d/math.h"

// AngelScript add-ons
#include "scripts/scriptstdstring.h"
#include "scripts/scriptbuilder.h"
#include "scripts/scripthelper.h"
#include "scripts/scriptarray.h"
#include "scripts/scriptgrid.h"

// Profiler
#include "x2d/math/profiler.h"
#include "x2d/console.h"
#include "x2d/debug.h"
#include "x2d/iosystem.h"
#include "x2d/scripts.h"
#include "x2d/assetloader.h"

#include <cmath>

#ifdef X2D_LINUX
#define MAX_PATH 256
#endif

#ifdef X2D_WINDOWS
float roundf(const float value)
{
    return value < 0.0f ? ceil(value - 0.5f) : floor(value + 0.5f);
}
#endif

// Game engine
X2DEngine* gameEngine = 0;

// gfx.h
unsigned int rgb(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return r + (g << 8) + (b << 16) + (a << 24);
}

// engine.h
X2DEngine *CreateEngine(const int flags)
{
	return new X2DEngine(flags);
}

//------------------------------------------------------------------------
// Engine
//------------------------------------------------------------------------
X2DEngine::X2DEngine(const int flags) :
	m_flags(flags),
	m_initialized(false),
	m_running(false),
	m_paused(false),
	m_framesPerSecond(0),
	m_updateFunc(0),
	m_drawFunc(0),
	m_refreshRate(30),
	m_timeStep(0.0f),
	m_toggleProfiler(false),
	app(0),
	debug(0),
	scripts(0),
	gfx(0),
	sfx(0)
{
	// Set engine
	assert(!gameEngine);
	gameEngine = this;
	m_profiler = new X2DProfiler;
}

X2DEngine::~X2DEngine()
{
#ifdef NO
	// Make sure the server is disconnected
	if(!gameServer->isDisconnected())
		gameServer->disconnect();

	// Make sure the client is disconnected
	if(!gameClient->isDisconnected())
		gameClient->disconnect();
#endif

	// Clean up AngelScript
	ScriptContext* ctx = asGetActiveContext();
	if(ctx) assert("AngelScript: There is an active context running!");
	if(scriptEngine) assert(scriptEngine->Release() == 0);

	// Clear profiler
	delete m_profiler;
	
	// Clear module
	delete scripts;
	
	// Clear GFX
	delete gfx;
	
	// Clear SFX
	delete sfx;

	// Close sockets
	//closeSockets();

	// Clear client
	//delete gameClient;

	// Clear server
	//delete gameServer;
	
	// Clear debugger
	delete debug;

	// Reset global engines
	gameEngine = 0;
	scriptEngine = 0;
}

//------------------------------------------------------------------------
// Run
//------------------------------------------------------------------------

X2DRetCode X2DEngine::init()
{
	// Make sure all components are specified
	if(!app || !gfx || !sfx || !debug || !assetLoader)
		return X2D_MissingComponent;

	// 
	if(isEnabled(X2D_Debug))
	{
		// Init debugger
		if(debug->init())
		{
			// Socket initialized, listen on port 5120
			if(debug->listen(5120))
			{
				// Try to connect to the external debugger
				if(debug->connect())
				{
					// Tell the external debugger the connection was successful
					debug->sendPacket(X2DDebug::ConnectedPacket);
				}else{
					// Failed to connect to external debugger (timed out)
				}
			}else{
				// Failed to listen on port
			}
		}else{
			// Failed to initialize socket
		}
	}
	
	// Print application message
	iosystem::success("** Aurora x2D **");
	
	// Create the script engine
	iosystem::success("** Initializing AngelScript (%s) **", ANGELSCRIPT_VERSION_STRING);
	scriptEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);

	scriptEngine->SetEngineProperty(asEP_COMPILER_WARNINGS, 0);

	// Set the message callback to receive information on errors in human readable form.
	int r = scriptEngine->SetMessageCallback(asFUNCTION(asMessageCallback), 0, asCALL_CDECL); assert( r >= 0 );

	// Register stuff
	RegisterStdString(scriptEngine);
	RegisterScriptArray(scriptEngine, true);
	RegisterGrid(); // TODO: Re-implement
	RegisterMath();
	RegisterStdStringUtils(scriptEngine);
	
	// Io system
	AS_GLOBAL_FUNCTIONPR("void print(const string &in)", iosystem::print, (const string&), void)
	AS_GLOBAL_FUNCTIONPR("void warn(const string &in)", iosystem::warn, (const string&), void)
	AS_GLOBAL_FUNCTIONPR("void success(const string &in)", iosystem::success, (const string&), void)
	AS_GLOBAL_FUNCTIONPR("void error(const string &in)", iosystem::error, (const string&), void)

	// File
	AS_GLOBAL_FUNCTION("void copyFile(string &in)", iosystem::copyFile)
	AS_GLOBAL_FUNCTION("void removeFile(string &in)", iosystem::removeFile)
	AS_GLOBAL_FUNCTION("void isFile(string &in)", iosystem::isFile)

	// Directories
	//AS_GLOBAL_FUNCTION("array<string>@ listFiles(const string &in, const string &in)", iosystem::listFiles)
	
	// Conversion
	AS_GLOBAL_FUNCTION("string itos(const int)", math::intToStr)
	AS_GLOBAL_FUNCTION("string ftos(const float)", math::floatToStr)
	AS_GLOBAL_FUNCTION("string btos(const bool)", math::boolToStr)
	AS_GLOBAL_FUNCTION("int stoi(const string &in)", math::strToInt)
	AS_GLOBAL_FUNCTION("float stof(const string &in)", math::strToFloat)
	AS_GLOBAL_FUNCTION("bool stob(const string &in)", math::strToBool)
	AS_GLOBAL_FUNCTION("string toAscii(const uint8)", math::strToAscii)
	AS_GLOBAL_FUNCTION("uint8 fromAscii(const string &in)", math::asciiToStr)
	AS_GLOBAL_FUNCTION("void setEnumValue(?&out, const int val)", math::setEnumValue);

	// Math
	AS_GLOBAL_FUNCTION("string tolower(string &in, const int begin = 0, const int end = 0)", math::toLower)
	AS_GLOBAL_FUNCTION("string toupper(string &in, const int begin = 0, const int end = 0)", math::toUpper)
	AS_GLOBAL_FUNCTION("int round(const float)", roundf)
	AS_GLOBAL_FUNCTION("int ceil(const float)", ceilf)
	AS_GLOBAL_FUNCTION("int floor(const float)", floorf)
	AS_GLOBAL_FUNCTION("int clamp(const float)", math::clampValue)
	AS_GLOBAL_FUNCTION("int max(const float)", math::maximum)
	AS_GLOBAL_FUNCTION("int min(const float)", math::minimum)
		
	AS_GLOBAL_FUNCTIONPR("float cos(const float)", cos, (const float), float)
	AS_GLOBAL_FUNCTIONPR("float sin(const float)", sin, (const float), float)
	AS_GLOBAL_FUNCTIONPR("float tan(const float)", tan, (const float), float)
	AS_GLOBAL_FUNCTIONPR("float acos(const float)", acos, (const float), float)
	AS_GLOBAL_FUNCTIONPR("float asin(const float)", asin, (const float), float)
	AS_GLOBAL_FUNCTIONPR("float atan(const float)", asin, (const float), float)
	AS_GLOBAL_FUNCTIONPR("float abs(const float)", abs, (const float), float)
	AS_GLOBAL_FUNCTIONPR("float sqrt(const float)", sqrt, (const float), float)
	AS_GLOBAL_FUNCTIONPR("float pow(const float, const float)", pow, (const float, const float), float)
	AS_GLOBAL_FUNCTION("void seedRandom(const uint)", math::seedRandom)
	AS_GLOBAL_FUNCTIONPR("int randomInt()", math::getRandomInt, (), int)
	AS_GLOBAL_FUNCTIONPR("int randomInt(const int, const int)", math::getRandomInt, (const int, const int), int)
	AS_GLOBAL_FUNCTION("float random()", math::getRandomFloat)

	// Enums
	AS_ENUM_REGISTER("MouseButton");
	AS_ENUM_VALUE("MouseButton", "LeftMouseButton", X2D_LeftMouseButton);
	AS_ENUM_VALUE("MouseButton", "RightMouseButton", X2D_RightMouseButton);
	AS_ENUM_VALUE("MouseButton", "MiddleMouseButton", X2D_MiddleMouseButton);
	
	AS_ENUM_REGISTER("VirtualKey");
	AS_ENUM_VALUE("VirtualKey", "KeyLeftButton", X2D_LeftMouse); // Left mouse button
	AS_ENUM_VALUE("VirtualKey", "KeyRightButton", X2D_RightMouse); // Right mouse button
	AS_ENUM_VALUE("VirtualKey", "KeyMiddleButton", X2D_MiddleMouse); // Any mouse button
	AS_ENUM_VALUE("VirtualKey", "KeyBackspace", X2D_KeyBackspace); // Backspace
	AS_ENUM_VALUE("VirtualKey", "KeyEscape", X2D_KeyEscape);
	AS_ENUM_VALUE("VirtualKey", "KeyTab", X2D_KeyTab);
	AS_ENUM_VALUE("VirtualKey", "KeyEnter", X2D_KeyEnter); // Enter
	AS_ENUM_VALUE("VirtualKey", "KeyShift", X2D_KeyShift);
	AS_ENUM_VALUE("VirtualKey", "KeyLeftShift", X2D_KeyLeftShift);
	AS_ENUM_VALUE("VirtualKey", "KeyRightShift", X2D_KeyRightShift);
	AS_ENUM_VALUE("VirtualKey", "KeyControl", X2D_KeyControl);
	AS_ENUM_VALUE("VirtualKey", "KeyLeftControl", X2D_KeyLeftControl);
	AS_ENUM_VALUE("VirtualKey", "KeyRightControl", X2D_KeyRightControl);
	AS_ENUM_VALUE("VirtualKey", "KeyMenu", X2D_KeyMenu); // Windows
	AS_ENUM_VALUE("VirtualKey", "KeyLeftMenu", X2D_KeyLeftMenu)
	AS_ENUM_VALUE("VirtualKey", "KeyRightMenu", X2D_KeyRightMenu)
	AS_ENUM_VALUE("VirtualKey", "KeySpace", X2D_KeySpace);
	AS_ENUM_VALUE("VirtualKey", "KeyPageUp", X2D_KeyPageUp); // Page up
	AS_ENUM_VALUE("VirtualKey", "KeyPageDown", X2D_KeyPageDown); // Page down
	AS_ENUM_VALUE("VirtualKey", "KeyEnd", X2D_KeyEnd);
	AS_ENUM_VALUE("VirtualKey", "KeyHome", X2D_KeyHome);
	AS_ENUM_VALUE("VirtualKey", "KeyLeft", X2D_KeyLeft);
	AS_ENUM_VALUE("VirtualKey", "KeyUp", X2D_KeyUp);
	AS_ENUM_VALUE("VirtualKey", "KeyRight", X2D_KeyRight);
	AS_ENUM_VALUE("VirtualKey", "KeyDown", X2D_KeyDown);
	AS_ENUM_VALUE("VirtualKey", "KeySnapshot", X2D_KeySnapshot); // Printscreen
	AS_ENUM_VALUE("VirtualKey", "KeyInsert", X2D_KeyInsert);
	AS_ENUM_VALUE("VirtualKey", "KeyDelete", X2D_KeyDelete);
	AS_ENUM_VALUE("VirtualKey", "KeyNumpad0", X2D_KeyNumpad0);
	AS_ENUM_VALUE("VirtualKey", "KeyNumpad1", X2D_KeyNumpad1);
	AS_ENUM_VALUE("VirtualKey", "KeyNumpad2", X2D_KeyNumpad2);
	AS_ENUM_VALUE("VirtualKey", "KeyNumpad3", X2D_KeyNumpad3);
	AS_ENUM_VALUE("VirtualKey", "KeyNumpad4", X2D_KeyNumpad4);
	AS_ENUM_VALUE("VirtualKey", "KeyNumpad5", X2D_KeyNumpad5);
	AS_ENUM_VALUE("VirtualKey", "KeyNumpad6", X2D_KeyNumpad6);
	AS_ENUM_VALUE("VirtualKey", "KeyNumpad7", X2D_KeyNumpad7);
	AS_ENUM_VALUE("VirtualKey", "KeyNumpad8", X2D_KeyNumpad8);
	AS_ENUM_VALUE("VirtualKey", "KeyNumpad9", X2D_KeyNumpad9);
	AS_ENUM_VALUE("VirtualKey", "KeyAdd", X2D_KeyPluss); // Numpad add
	AS_ENUM_VALUE("VirtualKey", "KeySubtract", X2D_KeyHyphen); // Numpad subtract
	AS_ENUM_VALUE("VirtualKey", "KeyDecimal", X2D_KeyPeriod); // Numpad dot
	AS_ENUM_VALUE("VirtualKey", "KeyDivide", X2D_KeySlash); // Numpad divide
	AS_ENUM_VALUE("VirtualKey", "KeyMultiply", X2D_KeyAsterisk); // Numpad multiply
	AS_ENUM_VALUE("VirtualKey", "KeyCaret", X2D_KeyCaret);
	AS_ENUM_VALUE("VirtualKey", "KeyApostrophe", X2D_KeyApostrophe);
	AS_ENUM_VALUE("VirtualKey", "KeyQuotationMark", X2D_KeyQuotationMark);
	AS_ENUM_VALUE("VirtualKey", "KeyBackslash", X2D_KeyBackslash);
	AS_ENUM_VALUE("VirtualKey", "KeySlash", X2D_KeySlash);
	AS_ENUM_VALUE("VirtualKey", "KeyAsterisk", X2D_KeyAsterisk);
	AS_ENUM_VALUE("VirtualKey", "KeyPipe", X2D_KeyPipe);
	AS_ENUM_VALUE("VirtualKey", "KeyColon", X2D_KeyColon);
	AS_ENUM_VALUE("VirtualKey", "KeySemicolon", X2D_KeySemicolon);
	AS_ENUM_VALUE("VirtualKey", "KeyTilde", X2D_KeyTilde);
	AS_ENUM_VALUE("VirtualKey", "KeyComma", X2D_KeyComma);
	AS_ENUM_VALUE("VirtualKey", "KeyGreater", X2D_KeyGreater);
	AS_ENUM_VALUE("VirtualKey", "KeyLess", X2D_KeyLess);
	AS_ENUM_VALUE("VirtualKey", "KeyEquals", X2D_KeyEquals);
	AS_ENUM_VALUE("VirtualKey", "Key0", X2D_Key0);
	AS_ENUM_VALUE("VirtualKey", "Key1", X2D_Key1);
	AS_ENUM_VALUE("VirtualKey", "Key2", X2D_Key2);
	AS_ENUM_VALUE("VirtualKey", "Key3", X2D_Key3);
	AS_ENUM_VALUE("VirtualKey", "Key4", X2D_Key4);
	AS_ENUM_VALUE("VirtualKey", "Key5", X2D_Key5);
	AS_ENUM_VALUE("VirtualKey", "Key6", X2D_Key6);
	AS_ENUM_VALUE("VirtualKey", "Key7", X2D_Key7);
	AS_ENUM_VALUE("VirtualKey", "Key8", X2D_Key8);
	AS_ENUM_VALUE("VirtualKey", "Key9", X2D_Key9);
	AS_ENUM_VALUE("VirtualKey", "KeyA", X2D_KeyA);
	AS_ENUM_VALUE("VirtualKey", "KeyB", X2D_KeyB);
	AS_ENUM_VALUE("VirtualKey", "KeyC", X2D_KeyC);
	AS_ENUM_VALUE("VirtualKey", "KeyD", X2D_KeyD);
	AS_ENUM_VALUE("VirtualKey", "KeyE", X2D_KeyE);
	AS_ENUM_VALUE("VirtualKey", "KeyF", X2D_KeyF);
	AS_ENUM_VALUE("VirtualKey", "KeyG", X2D_KeyG);
	AS_ENUM_VALUE("VirtualKey", "KeyH", X2D_KeyH);
	AS_ENUM_VALUE("VirtualKey", "KeyI", X2D_KeyI);
	AS_ENUM_VALUE("VirtualKey", "KeyJ", X2D_KeyJ);
	AS_ENUM_VALUE("VirtualKey", "KeyK", X2D_KeyK);
	AS_ENUM_VALUE("VirtualKey", "KeyL", X2D_KeyL);
	AS_ENUM_VALUE("VirtualKey", "KeyM", X2D_KeyM);
	AS_ENUM_VALUE("VirtualKey", "KeyN", X2D_KeyN);
	AS_ENUM_VALUE("VirtualKey", "KeyO", X2D_KeyO);
	AS_ENUM_VALUE("VirtualKey", "KeyP", X2D_KeyP);
	AS_ENUM_VALUE("VirtualKey", "KeyQ", X2D_KeyQ);
	AS_ENUM_VALUE("VirtualKey", "KeyR", X2D_KeyR);
	AS_ENUM_VALUE("VirtualKey", "KeyS", X2D_KeyS);
	AS_ENUM_VALUE("VirtualKey", "KeyT", X2D_KeyT);
	AS_ENUM_VALUE("VirtualKey", "KeyU", X2D_KeyU);
	AS_ENUM_VALUE("VirtualKey", "KeyV", X2D_KeyV);
	AS_ENUM_VALUE("VirtualKey", "KeyW", X2D_KeyW);
	AS_ENUM_VALUE("VirtualKey", "KeyX", X2D_KeyX);
	AS_ENUM_VALUE("VirtualKey", "KeyY", X2D_KeyY);
	AS_ENUM_VALUE("VirtualKey", "KeyZ", X2D_KeyZ);

	// Misc
	AS_GLOBAL_FUNCTION("void exit()", asExit)
	AS_GLOBAL_FUNCTION("void exec(string &in)", asExec)
	AS_GLOBAL_FUNCTION("bool recompileScripts()", asRecompile)

	// Refresh rate
	AS_SINGLETON_FUNCTION(X2DEngine, "void appSetRefreshRate(const int hz)", setRefreshRate, this)
	AS_SINGLETON_FUNCTION(X2DEngine, "int  appGetRefreshRate()", getRefreshRate, this)
	AS_SINGLETON_FUNCTION(X2DEngine, "float appGetTimeStep()", getTimeStep, this)

	// Profiler
	AS_SINGLETON_FUNCTION(X2DEngine, "void appStartProfiler()", startProfiler, this)
	AS_SINGLETON_FUNCTION(X2DEngine, "void appStopProfiler()", stopProfiler, this)
	AS_SINGLETON_FUNCTION(X2DEngine, "void appPushProfile(const string &in)", pushProfile, this)
	AS_SINGLETON_FUNCTION(X2DEngine, "void appPopProfile()", popProfile, this)
	AS_SINGLETON_FUNCTION(X2DEngine, "void appPrintProfilerResults()", printProfilerResults, this)

	// Application
	AS_SINGLETON_FUNCTION(X2DApp, "string getPlatformString()", getPlatformString, app)
	AS_SINGLETON_FUNCTION(X2DApp, "bool getKeyState(const VirtualKey key)", getKeyState, app)

	// Fullscreen and resolution
	AS_SINGLETON_FUNCTION(X2DApp, "void appEnableFullscreen()", enableFullscreen, app)
	AS_SINGLETON_FUNCTION(X2DApp, "void appDisableFullscreen()", disableFullscreen, app)
	AS_SINGLETON_FUNCTION(X2DApp, "array<vec2> @appGetResolutionList()", resolutionList, app)

	// Window flags
	AS_SINGLETON_FUNCTION(X2DApp, "void appEnableResize()", enableResize, app)
	AS_SINGLETON_FUNCTION(X2DApp, "void appDisableResize()", disableResize, app)

	// Window actions
	AS_SINGLETON_FUNCTION(X2DApp, "void appMove(const int x, const int y)", move, app)
	AS_SINGLETON_FUNCTION(X2DApp, "void appGetPos(int &out x, int &out y, bool borders = true)", pos, app)
	AS_SINGLETON_FUNCTION(X2DApp, "void appResize(const int w, const int h)", resize, app)
	AS_SINGLETON_FUNCTION(X2DApp, "void appGetSize(int &out w, int &out h)", size, app)
	AS_SINGLETON_FUNCTION(X2DApp, "void appMinimize()", minimize, app)
	AS_SINGLETON_FUNCTION(X2DApp, "void appMaximize()", maximize, app)
	AS_SINGLETON_FUNCTION(X2DApp, "void appRestore()", restore, app)

	// Dialog
	AS_SINGLETON_FUNCTION(X2DApp, "string showSaveDialog(const string &in, const string &in, const string &in, const string &in)", showSaveDialog, app)
	AS_SINGLETON_FUNCTION(X2DApp, "string showOpenDialog(const string &in, const string &in, const string &in, const string &in)", showOpenDialog, app)

	// Cursor
	AS_SINGLETON_FUNCTION(X2DApp, "void appSetCursorPos(const int x, const int y)", setCursorPos, app)
	AS_SINGLETON_FUNCTION(X2DApp, "void appGetCursorPos(int &out x, int &out y)", cursorPos, app)
	AS_SINGLETON_FUNCTION(X2DApp, "void appSetCursorLimits(const int x, const int y, const int w, const int h)", setCursorLimits, app)
	//AS_SINGLETON_FUNCTION(X2DApp, "void appShowCursor()", showCursor, app)
	//AS_SINGLETON_FUNCTION(X2DApp, "void appHideCursor()", hideCursor, app)

	// System Time
	AS_SINGLETON_FUNCTION(X2DApp, "uint64 appGetProgramTime()", getProgramTime, app)
	AS_SINGLETON_FUNCTION(X2DApp, "uint64 appGetTickCount()", getTickCount, app)
	AS_SINGLETON_FUNCTION(X2DApp, "void appRestartHrTimer()", startHrTimer, app)
	AS_SINGLETON_FUNCTION(X2DApp, "float appGetHrTime()", getHrTime, app)

	// Vsync
	AS_SINGLETON_FUNCTION(X2DApp, "void appEnableVsync()", enableVsync, app)
	AS_SINGLETON_FUNCTION(X2DApp, "void appDisableVsync()", disableVsync, app)

	// Create network managers
	//initSockets();
	//gameClient = new Client();
	//gameServer = new Server();
	
#ifdef DEPRICATED
	// Register console functionality
	AS_SINGLETON_FUNCTION(X2DConsole, "void conAppend(const string &in)", append, console)
	AS_SINGLETON_FUNCTION(X2DConsole, "string conGetLog()", log, console)
	AS_SINGLETON_FUNCTION(X2DConsole, "void conClearLog()", clearLog, console)
	AS_SINGLETON_FUNCTION(X2DConsole, "void conExportLog(const string &in)", exportLog, console)
	AS_SINGLETON_FUNCTION(X2DConsole, "string conReadBuffer()", readBuffer, console)
	AS_SINGLETON_FUNCTION(X2DConsole, "int conBufferSize()", bufferSize, console)
	AS_SINGLETON_FUNCTION(X2DConsole, "void conClearBuffer()", clearBuffer, console)
#endif
	
	// Register sound functionality
	AS_SINGLETON_FUNCTION(X2DSound, "int sfxCreateSource()", createSource, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxDeleteSource(const int)", deleteSource, sfx)
	
	AS_SINGLETON_FUNCTION(X2DSound, "int sfxPlaySource(const int)", playSource, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "int sfxStopSource(const int)", stopSource, sfx)
	
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetSourceBuffer(const int sourceId, const int bufferId)", setSourceBuffer, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetSourcePosition(const int sourceId, const float x, const float y, const float z)", setSourcePosition, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetSourceVelocity(const int sourceId, const float x, const float y, const float z)", setSourceVelocity, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetSourceLooping(const int sourceId, const bool looping)", setSourceLooping, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetSourceGain(const int sourceId, const float gain)", setSourceGain, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetSourcePitch(const int sourceId, const float pitch)", setSourcePitch, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetSourceMaxDist(const int sourceId, const float dist)", setSourceMaxDist, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetSourceMinDist(const int sourceId, const float dist)", setSourceMinDist, sfx)
	
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetListenerPosition(const float x, const float y, const float z)", setListenerPosition, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetListenerVelocity(const float x, const float y, const float z)", setListenerVelocity, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "void sfxSetListenerOrientation(const float x, const float y, const float z)", setListenerOrientation, sfx)
	
	AS_SINGLETON_FUNCTION(X2DSound, "int sfxLoadFile(const string &in filePath)", loadFile, sfx)
	AS_SINGLETON_FUNCTION(X2DSound, "int sfxDeleteBuffer(const int)", deleteBuffer, sfx)
	
	// Enums
	AS_ENUM_REGISTER("BlendFunc")
	AS_ENUM_VALUE("BlendFunc", "ZeroBlend", X2D_ZeroBlend)
	AS_ENUM_VALUE("BlendFunc", "OneBlend", X2D_OneBlend)
	AS_ENUM_VALUE("BlendFunc", "SrcColorBlend", X2D_SrcColorBlend)
	AS_ENUM_VALUE("BlendFunc", "OneMinusSrcColorBlend", X2D_OneMinusSrcColorBlend)
	AS_ENUM_VALUE("BlendFunc", "SrcAlphaBlend", X2D_SrcAlphaBlend)
	AS_ENUM_VALUE("BlendFunc", "OneMinusSrcAlphaBlend", X2D_OneMinusSrcAlphaBlend)
	AS_ENUM_VALUE("BlendFunc", "DstAlphaBlend", X2D_DstAlphaBlend)
	AS_ENUM_VALUE("BlendFunc", "OneMinusDstAlphaBlend", X2D_OneMinusDstAlphaBlend)
	AS_ENUM_VALUE("BlendFunc", "DstColorBlend", X2D_DstColorBlend)
	AS_ENUM_VALUE("BlendFunc", "OneMinusDstColorBlend", X2D_OneMinusDstColorBlend)
	AS_ENUM_VALUE("BlendFunc", "SrcAlphaSaturate", X2D_SrcAlphaSaturate)
	
	AS_ENUM_REGISTER("DrawMode")
	AS_ENUM_VALUE("DrawMode", "DrawPoints", X2D_DrawPoints)
	AS_ENUM_VALUE("DrawMode", "DrawLines", X2D_DrawLines)
	AS_ENUM_VALUE("DrawMode", "DrawLineLoop", X2D_DrawLineLoop)
	AS_ENUM_VALUE("DrawMode", "DrawTriangles", X2D_DrawTriangles)
	AS_ENUM_VALUE("DrawMode", "DrawTriangleStrip", X2D_DrawTriangleStrip)
	AS_ENUM_VALUE("DrawMode", "DrawTriangleFan", X2D_DrawTriangleFan)
	
	AS_ENUM_REGISTER("VertexBufferMode")
	AS_ENUM_VALUE("VertexBufferMode", "StreamBuffer", X2D_StreamBuffer)
	AS_ENUM_VALUE("VertexBufferMode", "StaticBuffer", X2D_StaticBuffer)
	AS_ENUM_VALUE("VertexBufferMode", "DynamicBuffer", X2D_DynamicBuffer)
	
	AS_ENUM_REGISTER("TextureFilter")
	AS_ENUM_VALUE("TextureFilter", "NearestFilter", X2D_NearestFilter)
	AS_ENUM_VALUE("TextureFilter", "BilinearFilter", X2D_BilinearFilter)

	// Projection functions
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxSetViewport(const vec2i pos, const vec2i size)", setViewport, (const vec2i, const vec2i), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxSetViewport(const int x, const int y, const int w, const int h)", setViewport, (const int, const int, const int, const int), void, gfx)
	//AS_SINGLETON_FUNCTION(X2DRender, "void gfxGetViewport()", gfxGetViewport, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetOrthoProjection(const float l, const float r, const float b, const float t, const float n, const float f)", setOrthoProjection, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxGetOrthoProjection(float &out l, float &out r, float &out b, float &out t, float &out n, float &out f)", getOrthoProjection, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "int  gfxGetDrawCallCount()", getDrawCallCount, gfx)

	// State modifiers
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetFont(const uint font)", setFont, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxSetTexCoord(const float x, const float y)", setTexCoord, (const float, const float), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxSetTexCoord(const vec2)", setTexCoord, (const vec2), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxSetColor(const vec4 color)", setColor, (const vec4), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxSetColor(const float, const float, const float, const float a = 1.0f)", setColor, (const float, const float, const float, const float), void, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetTexture(const int)", setTexture, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetDepth(const int depth)", setDepth, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxSetBlendFunc(const BlendFunc, const BlendFunc)", setBlendFunc, (const X2DBlendFunc, const X2DBlendFunc), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxSetBlendFuncSep(const BlendFunc, const BlendFunc, const BlendFunc, const BlendFunc)", setBlendFunc, (const X2DBlendFunc, const X2DBlendFunc, const X2DBlendFunc, const X2DBlendFunc), void, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetShader(const int shader)", setShader, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxResetDepth()", resetDepth, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxResetColor()", resetColor, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxResetTexture()", resetTexture, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxResetBlendFunc()", resetBlendFunc, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxResetShader()", resetShader, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxReset()", reset, gfx)

	// Font
	AS_SINGLETON_FUNCTION(X2DRender, "int  gfxLoadFont(const string &in, const uint size)", loadFont, gfx)
	//AS_SINGLETON_FUNCTION(X2DRender, "void gfxClearFont(const uint fontId)", clearFont, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "float gfxGetStringWidth(const string &in)", stringWidth, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "float gfxGetStringHeight(const string &in)", stringHeight, gfx)

	// Drawing functions
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxBegin(const DrawMode = DrawTriangles)", begin, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxEnd()", end, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxAddText(const vec2 pos, const string &in)", addText, (const vec2, const string&), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxAddText(const float x, const float y, const string &in)", addText, (const float, const float, const string&), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxAddVertex(const vec2 pos)", addVertex, (const vec2), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxAddVertex(const float x, const float y)", addVertex, (const float, const float), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxAddRect(const vec2 pos, const vec2 size, const vec2 coord0, const vec2 coord1, const bool center = false)", addRect, (const vec2, const vec2, const vec2, const vec2, const bool), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxAddRect(const float, const float, const float, const float, const float, const float, const float, const float, const bool center = false)", addRect, (const float, const float, const float, const float, const float, const float, const float, const float, const bool), void, gfx)

	// Clipping
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxBeginClip(const float x, const float y, const float width, const float height)", beginClip, (const float, const float, const float, const float), void, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxEndClip()", endClip, gfx)

	// Wireframe
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxEnableWireframe()", enableWireframe, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxDisableWireframe()", disableWireframe, gfx)

	// Transform
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxPushTransform(const float px, const float py,"
															"const float rx = 0.0f, const float ry = 0.0f, const float rz = 0.0f, const float ang = 0.0f,"
															"const float sx = 1.0f, const float sy = 1.0f)", pushTransform,
															(const float, const float, const float, const float, const float, const float, const float, const float), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxPushTransform(const vec2 pos, const vec4 rot = vec4(0.0f), const vec2 scale = vec2(1.0f))", pushTransform,
															(const vec2, const vec4, const vec2), void, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxPopTransform()", popTransform, gfx)

	// Texture processing
	AS_SINGLETON_FUNCTION(X2DRender, "int  gfxCreateTexture(const int w, const int h, const TextureFilter filter = NearestFilter, const bool mipmap = false)", createTexture, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxDeleteTexture(const int)", deleteTexture, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetTextureFilter(const int, const TextureFilter filter = NearestFilter, const bool mipmap = false)", setTextureFilter, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetTextureData(const int, const int, const int, const array<uint> &in, const bool mipmap = false)", setTextureData, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxClearTexture(const int)", clearTexture, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "array<uint> @gfxGetTextureData(const int)", getTextureData, gfx)
	//AS_SINGLETON_FUNCTION(X2DRender, "array<uint> gfxReadPixels(const int x, const int y, const int w, const int h)", copyToTexture, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxGetTextureSize(const int, int &out w, int &out h)", getTextureSize, gfx)

	// Image loading/saving
	AS_SINGLETON_FUNCTION(X2DRender, "int gfxLoadImage(const string &in, const TextureFilter filter = NearestFilter, const bool mipmap = false)", loadImage, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "array<uint> @gfxLoadImageData(const string &in, int &out, int &out)", loadImageData, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxExportTexture(const int, const string &in)", exportTexture, gfx)

	// Texture atlasing
	//AS_SINGLETON_FUNCTION(X2DRender, "int  gfxLoadImageIntoAtlas(const string &in)", loadImageIntoAtlas, gfx)

	// Shader
	AS_SINGLETON_FUNCTION(X2DRender, "int  gfxLoadShader(const string &in, const string &in)", loadShader, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniform1i(const string &in, const int)", setUniform1i, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniform2i(const string &in, const int, const int)", setUniform2i, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniform3i(const string &in, const int, const int, const int)", setUniform3i, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniform4i(const string &in, const int, const int, const int, const int)", setUniform4i, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniform1f(const string &in, const float)", setUniform1f, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniform2f(const string &in, const float, const float)", setUniform2f, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniform3f(const string &in, const float, const float, const float)", setUniform3f, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniform4f(const string &in, const float, const float, const float, const float)", setUniform4f, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniformTexture(const string &in, const int)", setUniformTexture, gfx)

	// Frame buffers
	AS_SINGLETON_FUNCTION(X2DRender, "int  gfxCreateFrameBuffer()", createFrameBuffer, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxBindFrameBuffer(const int fboId)", bindFrameBuffer, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxAttachTextureToFrameBuffer(const int textureId)", attachTextureToFrameBuffer, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxClearFrameBuffer()", clearFrameBuffer, gfx)

	// Vertex buffers
	AS_SINGLETON_FUNCTION(X2DRender, "int  gfxCreateVertexBuffer()", createVertexBuffer, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxBindVertexBuffer(const int vboId)", bindVertexBuffer, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetVertexBufferData(const array<float> &in, const VertexBufferMode mode)", setVertexBufferData, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetVertexBufferSubData(const int offset, const array<float> &in)", setVertexBufferSubData, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxDrawVertexBuffer(const int mode, const int begin, const int count)", drawVertexBuffer, gfx)

	// Create managers
	scripts = new ScriptManager();

	assetLoader->loadPlugins();

	// Compile the AngelScript module
	iosystem::success("** Compiling AngelScript Module **");
	r = asCompileModule("GameModule");
	if(r < 0)
	{
		// Module compilation failed, exit application
		return X2D_CompileError;
	}
	
	// Create the game module
	ScriptModule *mod = scriptEngine->GetModule("GameModule");
	scripts->setModule(mod);

	// Find the function that is to be called.
	iosystem::success("** Entering Main **");
	ScriptFunction *mainFunc = mod->GetFunctionByDecl("void main()");
	m_updateFunc = mod->GetFunctionByDecl("void update()");
	m_drawFunc = mod->GetFunctionByDecl("void draw()");
	if(!mainFunc || !m_updateFunc || !m_drawFunc) {
		// The function couldn't be found, exit the application
		string except("Could not find 'void main()'. Please make sure the function exists.");
		app->exception(X2D_MissingMain, except.c_str());
		return X2D_MissingMain;
	}

	// Create our context and run main
	ScriptContext* ctx = scripts->createScriptContext();
	r = ctx->Prepare(mainFunc); assert(r >= 0);
	r = ctx->Execute(); assert(r >= 0);
	if(r != asEXECUTION_FINISHED)
	{
		// Get exception section and line
		const char *sectionName;
		int line;
		ctx->GetExceptionLineNumber(&line, &sectionName);
		
		// Format output string
		iosystem::error("Run-Time exception '%s' occured in function '%s' in file '%s:%i'",
			ctx->GetExceptionString(), ctx->GetExceptionFunction()->GetDeclaration(), sectionName, line);
	}
	r = ctx->Release(); assert(r >= 0);

	// Engine initialized
	iosystem::success("** x2D Initialized **");
	if(isEnabled(X2D_Debug))
		debug->sendPacket(X2DDebug::InitializedPacket);

	m_initialized = true;

	// Return no error
	return X2D_OK;
}

/*void X2DEngine::setState(const X2DState state)
{
	m_state = state;
}*/

// Profiler
void X2DEngine::startProfiler()
{
	if(!m_profiler->isActive())
		m_toggleProfiler = true;
}

void X2DEngine::stopProfiler()
{
	if(m_profiler->isActive())
		m_toggleProfiler = true;
}

void X2DEngine::pushProfile(const string &name)
{
	if(m_profiler->isActive())
		m_profiler->pushProfile(name);
}

void X2DEngine::popProfile()
{
	if(m_profiler->isActive())
		m_profiler->popProfile();
}

void X2DEngine::printProfilerResults()
{
	m_profiler->printResults();
}

// Refresh rate
void X2DEngine::setRefreshRate(const int hz)
{
	if(hz == 0) {
		iosystem::error("appSetRefreshRate() refresh rate cannot be 0");
		return;
	}
	m_timeStep = 1.0f/hz;
	m_refreshRate = hz;
}

int X2DEngine::getRefreshRate()
{
	return m_refreshRate;
}

float X2DEngine::getTimeStep()
{
	return m_timeStep;
}

// Game loop
void X2DEngine::draw()
{
	// Start draw
	m_profiler->pushProfile("Draw");
	gfx->beginDraw();

	ScriptContext *ctx = scripts->createScriptContext();
	int r = ctx->Prepare(m_drawFunc); assert(r >= 0);
	r = ctx->Execute(); assert(r >= 0);
	r = ctx->Release(); assert(r >= 0);

	// End draw
	gfx->endDraw();
	m_profiler->popProfile();
}

void X2DEngine::update()
{
	m_profiler->pushProfile("Update");

	ScriptContext *ctx = scripts->createScriptContext();
	int r = ctx->Prepare(m_updateFunc); assert(r >= 0);
	r = ctx->Execute(); assert(r >= 0);
	if(r != asEXECUTION_FINISHED)
	{
		// Get exception section and line
		const char *sectionName;
		int line;
		ctx->GetExceptionLineNumber(&line, &sectionName);
		
		// Format output string
		iosystem::error("Run-Time exception '%s' occured in function '%s' in file '%s:%i'",
			ctx->GetExceptionString(), ctx->GetExceptionFunction()->GetDeclaration(), sectionName, line);
	}
	r = ctx->Release(); assert(r >= 0);
	
	m_profiler->popProfile();
}

X2DRetCode X2DEngine::run()
{
	assert(m_initialized);

	// Setup game loop
	app->startHrTimer();
	float prevTime = app->getHrTime();
	float acc = 0.0f;
	m_timeStep = 1.0f/m_refreshRate;

	// Fps sampling
	const int numFpsSamples = 8;
	float fpsSamples[numFpsSamples];
	int currFpsSample = 0;

	m_running = true;

	// Game loop
	while(m_running)
    {
		// Toggle profiler
		if(m_toggleProfiler) {
			if(!m_profiler->isActive())
				m_profiler->start();
			else
				m_profiler->stop();
			m_toggleProfiler = false;
		}
		m_profiler->pushProfile("Game Loop");

		// Process game events
		app->processEvents();

		// Check if game is paused or out of focus
		if(m_paused)// || !m_focus)
			continue;

		// Calculate time delta
		const float currentTime = app->getHrTime();
		float deltaTime = currentTime - prevTime;
		prevTime = currentTime;
		
		// Avoid spiral of death
		if(deltaTime > 0.25f)
			deltaTime = 0.25f;
		
		// Apply time delta to acc
		acc += deltaTime;
		while(acc >= m_timeStep)
		{
			// Update the game
			update();
			acc -= m_timeStep;
		}

		// Draw the game
		draw();

		// Calculate fps
		if(deltaTime != 0.0f) fpsSamples[currFpsSample++] = 1.0f/deltaTime;
		if(currFpsSample >= numFpsSamples) {
			float fps = 0.0f;
			for(int i = 0; i < numFpsSamples; i++) fps += fpsSamples[i];
			m_framesPerSecond = int(fps/numFpsSamples);
			currFpsSample = 0;
		}
		
		m_profiler->popProfile();
	}

	// Return OK
	return X2D_OK;
}