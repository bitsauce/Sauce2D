#ifndef X2D_CONSOLE_H
#define X2D_CONSOLE_H

#include "common/config.h"

class XDebugger;
class XFileSystem;
class XFileWriter;

class XDAPI XConsole
{
	friend class XEngine;
public:
	AS_DECL_SINGLETON

	XConsole();
	virtual ~XConsole();

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
	
	XDebugger *m_debugger;
	XFileSystem *m_fileSystem;
	XFileWriter *m_output;
};

#endif // X2D_CONSOLE_H