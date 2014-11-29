//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>

SINGLETON_DEF(XConsole)

bool XConsole::s_initialized = false;

XConsole::XConsole() :
	m_engine(0), // Set by the engine
	m_output(0)
{
	SINGLETON_ASSERT
}

XConsole::~XConsole()
{
	delete m_output;
}

void XConsole::log(const string &msg)
{
	Log(msg.c_str());
}

void XConsole::call_log(const char *msg, va_list args)
{
	// Get string length
	int size = _vscprintf(msg, args) + 1;

	// Create out string
	string out;
	out.resize(size);
	
	// Parse varargs
#ifdef USE_CTR_SECURE
	vsprintf_s(&out[0], size, msg, args);
#else
	vsprintf(out, msg, args);
#endif

	// System log
#ifdef X2D_WINDOWS
	OutputDebugString(out.c_str());
	OutputDebugString("\n");
#endif
	
	// Append message to log file
	if(XEngine::isEnabled(XD_EXPORT_LOG))
	{
		m_output->append(out);
		m_output->append("\n");
		m_output->flush();
	}

	// Send message to debugger
	//if(m_engine->getDebugger())
	{
		//m_engine->getDebugger()->sendPacket(XD_MESSAGE_PACKET, out);
	}

	// Append to console buffer
	m_log.append(out);
	m_buffer.append(out);
}

#ifndef USE_CTR_SECURE
#define CALL_LOG(format, ...)						\
	int size = _scprintf(format, __VA_ARGS__) + 1;	\
	char *newMsg = new char[size];					\
	sprintf(newMsg, format, __VA_ARGS__);			\
	s_this->call_log(newMsg, args);					\
	delete newMsg;
#else
#define CALL_LOG(format, ...)						\
	int size = _scprintf(format, __VA_ARGS__) + 1;	\
	char *newMsg = new char[size];					\
	sprintf_s(newMsg, size, format, __VA_ARGS__);	\
	s_this->call_log(newMsg, args);					\
	delete newMsg;
#endif

void XConsole::Log(const char *msg, ...)
{
	// Get argument lists
	va_list args;
	va_start(args, msg);

	CALL_LOG("%s", msg);

	va_end(args);
}

string XConsole::getLog() const
{
	return m_log;
}

void XConsole::clear()
{
	m_log.clear();
}

void XConsole::exportFile() const
{
	XFileSystem::WriteFile(":/console.log", m_log);
}

string XConsole::readBuffer()
{
	string buffer = m_buffer;
	m_buffer.clear();
	return buffer;
}

bool XConsole::hasBuffer() const
{
	return m_buffer.size() > 0;
}

void XConsole::clearBuffer()
{
	m_buffer.clear();
}