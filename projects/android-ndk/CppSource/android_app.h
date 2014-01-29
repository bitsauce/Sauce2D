#ifndef ANDROID_APP_H
#define ANDROID_APP_H

#include "x2d/app.h"

struct AppManager;

class AndroidApp : public X2DApp
{
public:
	AndroidApp(AppManager *manager);

	// Events handling
	void initEvents();
	void moveMouseBegin(const int x, const int y);
	void moveMouse(const int x, const int y);

	// Application functions
	void processEvents();
	void exception(X2DRetCode error, const char* message);
	string getPlatformString() const { return "Android"; }
	
	// Time functions
	ulong getProgramTime();
	ulong getTickCount();
	void startHrTimer();
	float getHrTime();

private:
	AppManager *m_manager;
	void *m_mouseMoveFunc;
	void *m_mouseMoveBeginFunc;
};


#ifdef DEPRICATED
class AndroidApp : public X2DApp
{
public:

	// Application functions
	void show();
	void close();
	void initEvents();
	void processEvents();
	void exception(X2DRetCode error, const char* message);
	
	// Time functions
	ulong getProgramTime();
	ulong getTickCount();
	void startHrTimer();
	float getHrTime();

	// window functions
	void enableFullscreen();
	void disableFullscreen();
	Array* resolutionList();

	// Window flags
	void enableResize();
	void disableResize();
	bool hasFocus();

	// Window actions
	void move(const int x, const int y);
	void pos(int &x, int &y, bool borders);
	void resize(const int w, const int h);
	void size(int &w, int &h);
	void minimize();
	void maximize();
	void restore();

	// Cursor function
	void setCursorPos(const int x, const int y);
	void cursorPos(int &x, int &y) const;
	void setCursorLimits(const int x, const int y, const int w, const int h);

	// Vsync
	void enableVsync();
	void disableVsync();
};
#endif

#endif // ANDROID_APP_H