//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>

BEGIN_XD_NAMESPACE

Console *Console::s_this = nullptr;

Console::Console() :
	m_engine(0), // Set by the engine
	m_output(0)
{
	// Check singleton
	if(s_this)
	{
		THROW("Console already initialized!");
	}
	s_this = this;
}

Console::~Console()
{
	delete m_output;
}

void Console::log(const string &msg)
{
	Log(msg.c_str());
}

void Console::call_log(const char *msg, va_list args)
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
	if(Engine::isEnabled(XD_EXPORT_LOG))
	{
		m_output->append(out);
		m_output->append("\n");
		m_output->flush();
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

void Console::Log(const char *msg, ...)
{
	// Get argument lists
	va_list args;
	va_start(args, msg);

	CALL_LOG("%s", msg);

	va_end(args);
}

string Console::getLog() const
{
	return m_log;
}

void Console::clear()
{
	m_log.clear();
}

void Console::exportFile() const
{
	FileSystem::WriteFile(":/console.log", m_log);
}

string Console::readBuffer()
{
	string buffer = m_buffer;
	m_buffer.clear();
	return buffer;
}

bool Console::hasBuffer() const
{
	return m_buffer.size() > 0;
}

void Console::clearBuffer()
{
	m_buffer.clear();
}

END_XD_NAMESPACE