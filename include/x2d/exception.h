#ifndef X2D_EXCEPTION_H
#define X2D_EXCEPTION_H

#include <x2d/config.h>
#include <x2d/util.h>

class xdException : public exception
{
public:
	xdException(xdRetCode code, string msg) :
		m_errorCode(code),
		m_message(msg)
	{
	}

	const char* what() const throw()
	{
		return "My exception happened";
	}

private:
	string m_message;
	xdRetCode m_errorCode;
};

#endif // X2D_EXCEPTION_H