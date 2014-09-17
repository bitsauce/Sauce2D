#ifndef WIN_FILE_SYSTEM_H
#define WIN_FILE_SYSTEM_H

#include <x2d/x2d.h>

#include <string>
#include <fstream>
#include <sstream>

class FileReader : public XFileReader
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

class FileWriter : public XFileWriter
{
public:
	FileWriter(const string &filePath) :
		stream(filePath, ofstream::binary)
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

class FileSystem : public XFileSystem
{
public:

	XScriptArray *listFiles(string &directory, const string &mask, const bool recursive) const;
	XScriptArray *listFolders(string &directory, const string &mask, const bool recursive) const;
	bool remove(string &path);

private:
	XFileReader *createFileReader(const string &filePath)
	{
		return new FileReader(filePath);
	}

	XFileWriter *createFileWriter(const string &filePath)
	{
		return new FileWriter(filePath);
	}

	bool makeDir(const string &path);
};

#endif // WIN_FILE_SYSTEM_H