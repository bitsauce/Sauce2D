//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>

XFileSystem *XFileSystem::s_this = 0;

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