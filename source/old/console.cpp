//     _                                     ____  ____  
//    / \  _   _ _ __ ___  _ __ __ _  __  __|___ \|  _ \ 
//   / _ \| | | |  __/ _ \|  __/ _  | \ \/ /  __) | | | |
//  / ___ \ |_| | | | (_) | | | (_| |  >  <  / __/| |_| |
// /_/   \_\__ _|_|  \___/|_|  \__ _| /_/\_\|_____|____/ 
//		MixedGraphics (C)

#include "console.h"
#include "x2d/app.h"

//---------------------------------------------------
// Console
// The interface for reading application output
//---------------------------------------------------
Console::Console()
{
	// Clear previous log
	//if(gameEngine->app->isFlag(Application::ExportConsoleFlag))
	//	m_outFile.open(getRelativePath("console.txt"), ios::trunc);
}

Console::~Console()
{
	// Close the out file
	//m_outFile.close();
}

//-----------------------------------------------------------
// Methods
//-----------------------------------------------------------

void Console::append(const string &msg)
{
	// Add to console and buffer
	m_log += msg;
	m_buffer += msg;
	
	// Append to output file
	if(gameEngine->isFlag(X2D_ExportLog))
		m_outFile << msg;
}

string Console::log() const
{
	return m_log;
}

void Console::clearLog()
{
	m_outFile.clear();
	m_log.clear();
	m_buffer.clear();
}

void Console::exportLog(const string &filePath)
{
	gameEngine->addFlag(X2D_ExportLog);
#ifdef WIN32
	m_outFile.open(filePath, ios::trunc);
#elif ANDROID
	m_outFile.open(filePath.c_str(), ios::trunc);
#endif
	m_outFile << m_log;
	m_outFile.close();
}

int Console::bufferSize() const
{
	return m_buffer.size();
}

string Console::readBuffer()
{
	string temp = m_buffer;
	m_buffer.clear();
	return temp;
}

void Console::clearBuffer()
{
	m_buffer.clear();
}