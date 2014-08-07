#ifndef X2D_FILE_SYSTEM_H
#define X2D_FILE_SYSTEM_H

#include <x2d/config.h>
#include <x2d/math.h>
#include <x2d/console.h>
#include <x2d/scripts/array.h>

class XDAPI xdFileReader
{
public:
	virtual ~xdFileReader() {}

	virtual bool isOpen() = 0;
	virtual bool isEOF() = 0;
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
	bool writeFile(string filePath, const string content) const;

	// OS spesifics
	virtual bool fileExists(string &filePath) const;
	// NOTE TO SELF: I might want to consider making a DirectoryIterator instead of using this function
	virtual Array *listFiles(string &directory, const string &mask, const bool recursive) const		{ NOT_IMPLEMENTED_ARR(listFiles, "string") }			// Optional
	virtual Array *listFolders(string &directory, const string &mask, const bool recursive) const	{ NOT_IMPLEMENTED_ARR(listFolders, "string") }			// Optional
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
	static bool WriteFile(string path, const string &content);
	static bool MakeDir(string path);

protected:
	virtual xdFileWriter *createFileWriter(const string &filePath) = 0;
	virtual xdFileReader *createFileReader(const string &filePath) = 0;
	virtual bool makeDir(const string &path) = 0;
	static xdFileSystem *s_this;
};

#endif // X2D_FILE_SYSTEM_H