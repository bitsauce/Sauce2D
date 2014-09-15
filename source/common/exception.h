#ifndef X2D_EXCEPTION_H
#define X2D_EXCEPTION_H

#include <x2d/config.h>
#include <x2d/util.h>

class xdException
{
public:
	xdException(xdRetCode code, string msg) :
		m_errorCode(code),
		m_message(msg)
	{
	}

	xdRetCode errorCode() const
	{
		return m_errorCode;
	}

	string message() const
	{
		return m_message;
	}

private:
	string m_message;
	xdRetCode m_errorCode;
};

#endif // X2D_EXCEPTION_H