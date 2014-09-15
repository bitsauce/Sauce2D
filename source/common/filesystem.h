#ifndef X2D_FILE_SYSTEM_H
#define X2D_FILE_SYSTEM_H

#include "common/config.h"
#include "scripts/scriptarray.h"

class XDAPI XFileReader
{
public:
	virtual ~XFileReader() {}

	virtual bool isOpen() = 0;
	virtual bool isEOF() = 0;
	virtual void close() = 0;

	virtual string readLine() = 0;
	virtual string readAll() = 0;
};

class XDAPI XFileWriter
{
public:
	virtual ~XFileWriter() {}

	virtual bool isOpen() = 0;
	virtual void close() = 0;

	virtual void clear() = 0;
	virtual void append(const char* data, const int length) = 0;
	void append(const string &str) { append(str.data(), str.size()); }
	virtual void flush() = 0;
};

class XDAPI XFileSystem
{
	friend class XEngine;
public:
	AS_DECL_SINGLETON

	// File buffers
	bool readFile(string filePath, string &conent) const;
	bool writeFile(string filePath, const string content) const;

	// OS spesifics
	virtual bool fileExists(string &filePath) const;
	// NOTE TO SELF: I might want to consider making a DirectoryIterator instead of using this function
	virtual XScriptArray *listFiles(string &directory, const string &mask, const bool recursive) const		{ NOT_IMPLEMENTED_RET(listFiles, 0) }			// Optional
	virtual XScriptArray *listFolders(string &directory, const string &mask, const bool recursive) const	{ NOT_IMPLEMENTED_RET(listFolders, 0) }			// Optional
	virtual bool remove(string &path)																		{ NOT_IMPLEMENTED_RET(remove, false) }			// Optional

	// System windows
	virtual string showSaveDialog(const string &file, const string &ext, const string &title, const string &folder) const	{ NOT_IMPLEMENTED_RET(showSaveDialog, "") }				// Optional
	virtual string showOpenDialog(const string &file, const string &ext, const string &title, const string &folder) const	{ NOT_IMPLEMENTED_RET(showOpenDialog, "") }				// Optional

	// Static factories
	static XFileWriter *CreateFileWriter(const string &filePath) { return s_this->createFileWriter(filePath); }
	static XFileReader *CreateFileReader(const string &filePath) { return s_this->createFileReader(filePath); }

	// Static functions
	static bool ReadFile(string path, string &content);
	static bool WriteFile(string path, const string &content);
	static bool MakeDir(string path);

protected:
	static XFileSystem *s_this;
	virtual XFileWriter *createFileWriter(const string &filePath) = 0;
	virtual XFileReader *createFileReader(const string &filePath) = 0;
	virtual bool makeDir(const string &path) = 0;
};

#endif // X2D_FILE_SYSTEM_H