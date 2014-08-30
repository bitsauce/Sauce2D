#ifndef X2D_CONSOLE_H
#define X2D_CONSOLE_H

#include <x2d/config.h>
#include <x2d/math.h>
#include <x2d/base.h>
#include <x2d/engine.h>

#define LOG(str, ...) xdEngine::GetConsole()->logf(str, __VA_ARGS__)
#define ERR(str, ...) xdEngine::GetConsole()->logf(str, __VA_ARGS__)

class xdDebug;
class xdFileSystem;
class xdFileWriter;

class XDAPI xdConsole
{
	friend class xdEngine;
public:
	AS_DECL_SINGLETON

	xdConsole();
	virtual ~xdConsole();

	void log(const string &msg);
	void logf(const char *msg, ...);
	string getLog() const;

	void clear();
	void exportFile() const;

	string readBuffer();
	bool hasBuffer() const;
	void clearBuffer();

protected:
	virtual void syslog(const string &msg) {}

private:
	void call_log(const char *msg, va_list args);

	string m_log;
	string m_buffer;
	
	bool m_initialized;
	
	xdDebug *m_debugger;
	xdFileSystem *m_fileSystem;
	xdFileWriter *m_output;
};

#endif // X2D_CONSOLE_H