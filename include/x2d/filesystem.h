#ifndef X2D_FILE_SYSTEM_H
#define X2D_FILE_SYSTEM_H

#include <x2d/config.h>
#include <x2d/math.h>
#include <x2d/console.h>

class XDAPI xdFileReader
{
public:
	virtual bool isOpen() = 0;
	virtual void close() = 0;

	virtual string readLine() = 0;
	virtual string readAll() = 0;
};

class XDAPI xdFileWriter
{
public:
	virtual ~xdFileWriter() {}

	virtual bool isOpen() = 0;
	virtual void close() = 0;

	virtual void clear() = 0;
	virtual void append(const char* data, const int length) = 0;
	void append(const string &str) { append(str.data(), str.size()); }
	virtual void flush() = 0;
};

class XDAPI xdFileSystem
{
	friend class xdEngine;
public:
	AS_DECL_SINGLETON

	// File buffers
	bool readFile(string filePath, string &conent) const;
	bool writeFile(string filePath, string content) const;

	// System directories
	virtual string getWorkingDirectory() const;
	virtual string getSaveDirectory() const			{ NOT_IMPLEMENTED_RET(getExeDirectory, "") }
	virtual string getPersonalDirectory() const		{ NOT_IMPLEMENTED_RET(getExeDirectory, "") }
	virtual string getRoot() const					{ NOT_IMPLEMENTED_RET(getExeDirectory, "") }

	// OS spesifics
	virtual bool fileExists(const string &filePath) const;
	virtual vector<string> listFiles(const string &dirPath, const bool recursive = false) const		{ NOT_IMPLEMENTED_RET(listFiles, vector<string>()) }	// Optional
	virtual void removeFile(string filePath)														{ NOT_IMPLEMENTED(removeFile) }							// Optional
	virtual void removeDir(string dirPath)															{ NOT_IMPLEMENTED(removeDir) }							// Optional

	// System windows
	virtual string showSaveDialog(const string &file, const string &ext, const string &title, const string &folder) const	{ NOT_IMPLEMENTED_RET(showSaveDialog, "") }				// Optional
	virtual string showOpenDialog(const string &file, const string &ext, const string &title, const string &folder) const	{ NOT_IMPLEMENTED_RET(showOpenDialog, "") }				// Optional

	// Static factories
	static xdFileWriter *CreateFileWriter(const string &filePath) { return s_this->createFileWriter(filePath); }
	static xdFileReader *CreateFileReader(const string &filePath) { return s_this->createFileReader(filePath); }

	// Static functions
	static bool ReadFile(string path, string &content);

protected:
	virtual xdFileWriter *createFileWriter(const string &filePath) = 0;
	virtual xdFileReader *createFileReader(const string &filePath) = 0;
	static xdFileSystem *s_this;
};

#endif // X2D_FILE_SYSTEM_H