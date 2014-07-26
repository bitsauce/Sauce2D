#ifndef WIN_FILE_SYSTEM_H
#define WIN_FILE_SYSTEM_H

#include <x2d/filesystem.h>
#include <x2d/util.h>

#include <string>
#include <fstream>
#include <sstream>

class FileReader : public xdFileReader
{
public:
	FileReader(const string &filePath) :
		stream(filePath, ifstream::binary)
	{
	}

	~FileReader()
	{
		close();
	}

	bool isEOF()
	{
		return stream.eof();
	}

	bool isOpen()
	{
		return stream.is_open();
	}

	void close()
	{
		stream.close();
	}

	string readLine()
	{
		string line;
		getline(stream, line);
		if(!line.empty() && line.back() == '\r') {
			line.pop_back();
		}
		return line.c_str();
	}

	string readAll()
	{
		stringstream ss;
		ss << stream.rdbuf();
		return ss.str();
	}

private:

	ifstream stream;
};

class FileWriter : public xdFileWriter
{
public:
	FileWriter(const string &filePath) :
		stream(filePath)
	{
	}

	~FileWriter()
	{
		close();
	}

	bool isOpen()
	{
		return stream.is_open();
	}

	void close()
	{
		stream.close();
	}

	void clear()
	{
		stream.clear();
	}

	void append(const char* data, const int length)
	{
		stream << data;
	}

	void flush()
	{
		stream.flush();
	}

private:
	ofstream stream;
};

class FileSystem : public xdFileSystem
{
public:

	Array *listFiles(string &directory, const string &mask, const bool recursive) const;

private:
	xdFileReader *createFileReader(const string &filePath)
	{
		return new FileReader(filePath);
	}

	xdFileWriter *createFileWriter(const string &filePath)
	{
		return new FileWriter(filePath);
	}
};

#endif // WIN_FILE_SYSTEM_H