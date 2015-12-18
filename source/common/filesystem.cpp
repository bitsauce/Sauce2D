//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2015 (C)

#include <x2d/engine.h>

BEGIN_CG_NAMESPACE

FileSystem *FileSystem::s_this = nullptr;

FileReader::FileReader(const string &path) :
	stream(util::getAbsoluteFilePath(path), ifstream::binary)
{
}

FileReader::~FileReader()
{
	close();
}

bool FileReader::isEOF()
{
	return stream.eof();
}

bool FileReader::isOpen()
{
	return stream.is_open();
}

void FileReader::close()
{
	stream.close();
}

bool FileReader::readBytes(char *buffer, const int count)
{
	stream.read(buffer, count);
	return !stream.eof();
}

string FileReader::readLine()
{
	string line;
	getline(stream, line);
	if(!line.empty() && line.back() == '\r') {
		line.pop_back();
	}
	return line.c_str();
}

string FileReader::readAll()
{
	stringstream ss;
	ss << stream.rdbuf();
	return ss.str();
}

FileWriter::FileWriter(const string &path) :
	stream(util::getAbsoluteFilePath(path), ofstream::binary)
{
}

FileWriter::~FileWriter()
{
	close();
}

bool FileWriter::isOpen()
{
	return stream.is_open();
}

void FileWriter::close()
{
	stream.close();
}

void FileWriter::clear()
{
	stream.clear();
}

FileSystemIterator::FileSystemIterator(string path, const string &mask, const int flags)
{
	// Create output array
	util::toAbsoluteFilePath(path);
	util::toDirectoryPath(path);

	// Get search path
	string searchPath = path + mask;
		
	// Find first file
	WIN32_FIND_DATA fdata;
	HANDLE hFind = FindFirstFile(searchPath.c_str(), &fdata);

	// Check if the search was successful
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do {
			wchar_t filename[MAX_PATH];
			mbstowcs_s(0, filename, fdata.cFileName, 1024);

			if(wcscmp(filename, L".") != 0 && wcscmp(filename, L"..") != 0)
			{
				if((fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 && (flags & DIRECTORIES) != 0)
				{
					m_files.push_back(path + fdata.cFileName + "/");
				}
				else if((flags & FILES) != 0)
				{
					m_files.push_back(path + fdata.cFileName);
				}
			}
		}
		while (FindNextFile(hFind, &fdata) != 0);
	}

	FindClose(hFind);

	m_itr = m_files.begin();
}

bool FileSystemIterator::hasNext() const
{
	return m_itr != m_files.end();
}

string &FileSystemIterator::next()
{
	string &str = *m_itr;
	++m_itr;
	return str; // *m_itr++;
}

void FileWriter::append(const char* data, const int length)
{
	stream.write(data, length);
}

void FileWriter::flush()
{
	stream.flush();
}

bool FileSystem::ReadFile(string path, string &content)
{
	util::toAbsoluteFilePath(path);
	return s_this->readFile(path, content);
}

bool FileSystem::WriteFile(string path, const string &content)
{
	MakeDir(path.substr(0, path.find_last_of('/')));
	util::toAbsoluteFilePath(path);
	return s_this->writeFile(path, content);
}

bool FileSystem::MakeDir(string path)
{
	util::toAbsoluteFilePath(path);
	return s_this->makeDir(path);
}

bool FileSystem::fileExists(string &filePath) const
{
	return util::fileExists(filePath);
}

bool FileSystem::readFile(string filePath, string &content) const
{
	FileReader fileReader(filePath);
	if(fileReader.isOpen())
	{
		content = fileReader.readAll();
		fileReader.close();
		return true;
	}
	return false;
}

bool FileSystem::writeFile(string filePath, const string content) const
{
	FileWriter fileWriter(filePath);
	fileWriter.clear();
	if(fileWriter.isOpen())
	{
		fileWriter.append(content);
		fileWriter.close();
		return true;
	}
	return false;
}

bool dirExists(const string& dirPath)
{
	DWORD attr = GetFileAttributes(dirPath.c_str());
	return attr != INVALID_FILE_ATTRIBUTES &&
		(attr & FILE_ATTRIBUTE_DIRECTORY);
}

bool FileSystem::makeDir(const string &path)
{
	if(!dirExists(path))
	{
		// Create all intermediate directories
		for(uint i = 2; i < path.size(); i++)
		{
			if(path[i] == '/')
			{
				string subPath = path.substr(0, i);
				if(!dirExists(subPath))
				{
					CreateDirectory(subPath.c_str(), 0);
				}
			}
		}
		return CreateDirectory(path.c_str(), 0) != 0;
	}
	return true;
}

END_CG_NAMESPACE
