#ifndef CORE_CONSOLE_H
#define CORE_CONSOLE_H

#include "x2d/platform.h"

class X2DAPI X2DConsole
{
public:

	virtual void append(const string &msg) = 0;
	virtual string log() const = 0;
	virtual void clearLog() = 0;
	virtual void exportLog(const string &filePath) = 0;

	virtual void setExportFlag(const bool val) = 0;
	virtual bool exportFlag() const = 0;

	virtual string readBuffer() = 0;
	virtual int bufferSize() const = 0;
	virtual void clearBuffer() = 0;
};

#endif // CORE_CONSOLE_H