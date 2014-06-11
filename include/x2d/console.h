#ifndef X2D_CONSOLE_H
#define X2D_CONSOLE_H

#include <x2d/config.h>
#include <x2d/math.h>
#include <x2d/base.h>
#include <x2d/engine.h>

#define LOG(str, ...) xdEngine::GetConsole()->log(str, __VA_ARGS__)
#define ERR(str, ...) xdEngine::GetConsole()->log(str, __VA_ARGS__)

class xdDebug;
class xdFileSystem;

class XDAPI xdConsole
{
	friend class xdEngine;
public:
	AS_DECL_SINGLETON

	xdConsole();

	virtual void log(const string &msg);
	void log(const char*, ...);
	string getLog() const;

	void clear();
	void exportFile() const;

	string readBuffer();
	bool hasBuffer() const;
	void clearBuffer();

private:
	string m_log;
	string m_buffer;
	xdDebug *m_debugger;
	xdFileSystem *m_fileSystem;
};

#endif // X2D_CONSOLE_H