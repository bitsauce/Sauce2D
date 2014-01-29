#ifndef X2D_IO_H
#define X2D_IO_H

#include "platform.h"
#include "math.h"

namespace iosystem
{
	//bool open(const char*, char*, long*);
	X2DAPI void copyFile(const string &srcFile, const string &dstFile);
	X2DAPI void removeFile(const string &filePath);
	X2DAPI bool isFile(string filePath);
	
	//bool isValidFilePath(const string &in);
	//void setFileName(string &filePath, string &name);
	X2DAPI void setFileExt(string &filePath, string &ext);
	X2DAPI string getFileExt(const string &filePath);
	X2DAPI string getFileName(const string &filePath);

	//Array *listFiles(const string &dir, const string &mask);

	vector<string> split(const string& src, const string& delim);
}

#ifdef DEPRICATED

#include <sstream>
#include <iostream>
#include <fstream>

// Register IO
void RegisterIO();


// Filepath
void toNativePath(string &path);

// File Functions
void setFileName(string &path, string &name);
void setFileExt(string &path, string &ext);
string fileName(const string &path);
string fileExt(const string &path);

void createDirs(const string &path);

void removeFile(string &filename);
void copyFile(string &inFile, string &outFile);

X2DAPI string readFileBuffer(const string &filePath);

X2DAPI bool isFile(const string &filename);
bool isDir(const string &directory);

bool listFiles(string path, string mask, vector<string>& files);
bool listFolders(string path, vector<string>& folders);

// Misc dialog
string showSaveDialog(const string &file, const string &ext, const string &title, const string &folder);
string showOpenDialog(const string &file, const string &ext, const string &title, const string &folder);

#endif // WIN32

#endif // X2D_IO_H