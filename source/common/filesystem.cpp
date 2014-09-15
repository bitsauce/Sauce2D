//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "filesystem.h"

XFileSystem *XFileSystem::s_this = 0;

AS_REG_SINGLETON(XFileSystem)

int XFileSystem::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	r = scriptEngine->RegisterObjectMethod("XFileSystem", "bool fileExists(string &in) const", asMETHOD(XFileSystem, fileExists), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XFileSystem", "array<string>@ listFiles(string &in, const string &in, const bool recursive = false) const", asMETHOD(XFileSystem, listFiles), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XFileSystem", "array<string>@ listFolders(string &in, const string &in, const bool recursive = false) const", asMETHOD(XFileSystem, listFolders), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XFileSystem", "bool remove(string &in)", asMETHOD(XFileSystem, remove), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("XFileSystem", "string showSaveDialog() const", asMETHOD(XFileSystem, showSaveDialog), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XFileSystem", "string showOpenDialog() const", asMETHOD(XFileSystem, showOpenDialog), asCALL_THISCALL); AS_ASSERT

	return r;
}

bool XFileSystem::ReadFile(string path, string &content)
{
	util::toAbsoluteFilePath(path);
	return s_this->readFile(path, content);
}

bool XFileSystem::WriteFile(string path, const string &content)
{
	MakeDir(path.substr(0, path.find_last_of('/')));
	util::toAbsoluteFilePath(path);
	return s_this->writeFile(path, content);
}

bool XFileSystem::MakeDir(string path)
{
	util::toAbsoluteFilePath(path);
	return s_this->makeDir(path);
}

bool XFileSystem::fileExists(string &filePath) const
{
	return util::fileExists(filePath);
}

bool XFileSystem::readFile(string filePath, string &content) const
{
	XFileReader *fileReader = CreateFileReader(filePath);
	if(fileReader->isOpen())
	{
		content = fileReader->readAll();
		fileReader->close();

		delete fileReader;
		return true;
	}

	delete fileReader;
	return false;
}

bool XFileSystem::writeFile(string filePath, const string content) const
{
	XFileWriter *fileWriter = CreateFileWriter(filePath);
	fileWriter->clear();
	if(fileWriter->isOpen())
	{
		fileWriter->append(content);
		fileWriter->close();

		delete fileWriter;
		return true;
	}
	
	delete fileWriter;
	return false;
}