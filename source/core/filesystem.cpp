//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/filesystem.h>

#include <angelscript.h>

#include "scripts/scriptarray.h"

xdFileSystem *xdFileSystem::s_this = 0;

AS_REG_SINGLETON(xdFileSystem, "ScriptFileSystem")

int xdFileSystem::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	r = scriptEngine->RegisterObjectMethod("ScriptFileSystem", "bool fileExists(string &in) const", asMETHOD(xdFileSystem, fileExists), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptFileSystem", "array<string>@ listFiles(string &in, const string &in, const bool recursive = false) const", asMETHOD(xdFileSystem, listFiles), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("ScriptFileSystem", "void removeFile(string)", asMETHOD(xdFileSystem, removeFile), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptFileSystem", "void removeDir(string)", asMETHOD(xdFileSystem, removeDir), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("ScriptFileSystem", "string showSaveDialog() const", asMETHOD(xdFileSystem, showSaveDialog), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptFileSystem", "string showOpenDialog() const", asMETHOD(xdFileSystem, showOpenDialog), asCALL_THISCALL); AS_ASSERT

	return r;
}

bool xdFileSystem::ReadFile(string path, string &content)
{
	return s_this->readFile(path, content);
}

bool xdFileSystem::WriteFile(string path, const string &content)
{
	return s_this->writeFile(path, content);
}

bool xdFileSystem::fileExists(string &filePath) const
{
	return util::fileExists(filePath);
}

bool xdFileSystem::readFile(string filePath, string &content) const
{
	util::toAbsoluteFilePath(filePath);
	xdFileReader *fileReader = CreateFileReader(filePath);
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

bool xdFileSystem::writeFile(string filePath, const string content) const
{
	util::toAbsoluteFilePath(filePath);
	xdFileWriter *fileWriter = CreateFileWriter(filePath);
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

#ifdef DEPRICATED

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

// Windows headers
#include <stack>
#include <shlobj.h>
#include <objbase.h> 
#include <shobjidl.h>    
#include <shlwapi.h>
#include <knownfolders.h>
#include <propvarutil.h>
#include <propkey.h>
#include <propidl.h>
#include <strsafe.h>
#include <shtypes.h>

#include <io.h>
#include <stdio.h>
#include <stdlib.h>

string xdFileSystem::getFileName(const string &filePath) const
{
	if(filePath.find('/') == string::npos) // Missing /
		return filePath;
	if(filePath.find('.') == string::npos) // Extentionless file
		return filePath.substr(filePath.find_last_of('/')+1, filePath.size()-filePath.find_last_of('/')-1);
	return filePath.substr(filePath.find_last_of('/')+1, filePath.find_last_of('.')-filePath.find_last_of('/')-1);
}

string xdFileSystem::getFileExt(const string &filePath) const
{
	return filePath.find('.') == string::npos ? "" : filePath.substr(filePath.find_last_of('.')+1, filePath.size()-filePath.find_last_of('.')-1);
}

//------------------------------------------------------------------------
// IO functions
//------------------------------------------------------------------------

void toNativePath(string &path)
{
	// Convert all backslashes to forward-slashes
	replace(path.begin(), path.end(), '\\', '/');
}

// Get save relative path
string getSavePath(string path)
{
	// Get current users my documents path
    char docPath[MAX_PATH];
    HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, docPath);

	// No error?
	if(result != S_OK)
	{
		print("Error: Could not get the current personal path");
		return "";
	}

	// Convert to string
	string savePath = docPath;

	// Add "My Games" to the path
	savePath += "\\My Games\\";

	// Add relative path
	savePath += path;
	
	// Convert all backslashes
	// to forward-slashes
	replace(savePath.begin(), savePath.end(), '\\', '/');

	return savePath;
}

//----------------------------------------------
//	File operations
//----------------------------------------------

// Copy file
void iosystem::copyFile(const string &inFile, const string &outFile)
{
	// Open filestreams
#ifdef X2D_WINDOWS
	ifstream toCopy(inFile, ios::binary);
	ofstream copyTo(outFile, ios::binary);
#else
	ifstream toCopy(inFile.c_str(), ios::binary);
	ofstream copyTo(outFile.c_str(), ios::binary);
#endif

	// Read buffer over to the out file
	copyTo << toCopy.rdbuf();

	// Close filestreams
	toCopy.close();
	copyTo.close();
}

//---------------------------------------------------------------
// Printing functions
//---------------------------------------------------------------

void iosystem::removeFile(const string &filePath)
{
	if(isFile(filePath))
		remove(filePath.c_str());
}

void LOG(const string &msg, const xdMessageType type)
{
	// Send message to application
	xdEngine::GetConsole()->print(msg, type);

	// Send message to debugger
	if(xdEngine::IsEnabled(XD_DEBUG))
		xdEngine::GetDebugger()->sendMessage(msg, type);
}

void LOG(const string &msg)
{
	// Send to print
	print(msg, X2D_PrintMsg);
}

void LOG(const char* msg, ...)
{
	// Phrase varargs
	char out[512];
	va_list args;
	va_start(args, msg);
#ifdef USE_CTR_SECURE
		vsprintf_s(out, msg, args);
#else
		vsprintf(out, msg, args);
#endif
	va_end(args);

	// Send to overload
	print(string(out), X2D_PrintMsg);
}

void iosystem::success(const char* msg, ...)
{
	// Phrase varargs
	char out[512];
	va_list args;
	va_start(args, msg);
#ifdef USE_CTR_SECURE
		vsprintf_s(out, msg, args);
#else
		vsprintf(out, msg, args);
#endif
	va_end(args);

	// Send to overload
	print(string(out), X2D_SuccessMsg);
}

void iosystem::success(const string &msg)
{
	// Send to print
	print(msg, X2D_SuccessMsg);
}

void iosystem::warn(const char* msg, ...)
{
	// Phrase varargs
	char out[512];
	va_list args;
	va_start(args, msg);
#ifdef USE_CTR_SECURE
		vsprintf_s(out, msg, args);
#else
		vsprintf(out, msg, args);
#endif
	va_end(args);

	// Send to overload
	print(string(out), X2D_WarningMsg);
}

void iosystem::warn(const string &msg)
{
	// Send to print
	print(msg, X2D_WarningMsg);
}

void ERR(const char* msg, ...)
{
	// Phrase varargs
	char out[512];
	va_list args;
	va_start(args, msg);
#ifdef USE_CTR_SECURE
		vsprintf_s(out, msg, args);
#else
		vsprintf(out, msg, args);
#endif
	va_end(args);

	// Send to overload
	print(string(out), X2D_ErrorMsg);
}

void ERR(const string &msg)
{
	// Send to print
	print(msg, X2D_ErrorMsg);
}

string sprint(const char* msg, ...)
{
	char out[512];
	va_list args;
	va_start(args, msg);
#ifdef USE_CTR_SECURE
		vsprintf_s(out, msg, args);
#else
		vsprintf(out, msg, args);
#endif
	va_end(args);
	return out;
}

bool isDir(const string &directory)
{
	if(GetFileAttributes(directory.c_str()) == INVALID_FILE_ATTRIBUTES)
		return false;
	return true;
}

void createDirs(const string &path)
{
	// Split string by '/'
	string folder;
	vector<string> folders;
	for(uint i = 0; i < path.size(); i++)
	{
		if(path[i] == '/')
			folders.push_back(folder);
		folder += path[i];
	}

	// Create directories
	for(uint i = 0; i < folders.size(); i++)
	{
		if(GetFileAttributes(folders[i].c_str()) == INVALID_FILE_ATTRIBUTES)
			CreateDirectory(folders[i].c_str(), 0);
	}
	CreateDirectory(folder.c_str(), 0);
}

//----------------------------------------------
// Open/Save dialog
//----------------------------------------------

string showSaveDialog(const string &file, const string &ext, const string &title, const string &folder)
{
	// CoCreate the File Open Dialog object.
    IFileSaveDialog *pfsd = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfsd));
	char path[MAX_PATH] = "\0";
    if(SUCCEEDED(hr))
    {
        // Phrase filter string
		vector<string> filters = split(ext, ";");
		COMDLG_FILTERSPEC *openTypes = 0;
		uint filterCount = filters.size();
		if(!ext.empty() && filterCount > 0)
		{
			openTypes = new COMDLG_FILTERSPEC[filters.size()];
			for(uint i = 0; i < filterCount; i++)
			{
				// Set filter name
				wchar_t filterName[MAX_PATH];
				mbstowcs_s(0, filterName, filters[i].c_str(), MAX_PATH);
				openTypes->pszName = filterName;

				// Set filter spec
				string filterSpecStr = filters[i].substr(filters[i].find_first_of("(")+1,
					filters[i].find_last_of(")")-filters[i].find_first_of("(")-1);
				wchar_t filterSpec[MAX_PATH];
				mbstowcs_s(0, filterSpec, filterSpecStr.c_str(), MAX_PATH);
				openTypes->pszSpec = filterSpec;
			}

			// Set the file types to display.
			hr = pfsd->SetFileTypes(filterCount, openTypes);
		}

        if(SUCCEEDED(hr))
        {
            // Ensure the dialog only returns items that can be represented by file system paths.
            DWORD dwFlags;
            hr = pfsd->GetOptions(&dwFlags);
            if(SUCCEEDED(hr))
            {
                hr = pfsd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
				wchar_t filename[MAX_PATH];
				mbstowcs_s(0, filename, file.c_str(), MAX_PATH);
				pfsd->SetFileName(filename);
						
				string folderstr = folder;
				replace(folderstr.begin(), folderstr.end(), '/', '\\'); // toNative();
				wchar_t foldername[MAX_PATH];
				mbstowcs_s(0, foldername, folderstr.c_str(), MAX_PATH);

				IShellItem *psiFolder;
				LPWSTR wszPath = foldername;
				if(SUCCEEDED(hr))
				{
					hr = SHCreateItemFromParsingName(wszPath, NULL, IID_PPV_ARGS(&psiFolder));
					if(SUCCEEDED(hr))
						pfsd->SetFolder(psiFolder);
				}

				wchar_t titlename[MAX_PATH];
				mbstowcs_s(0, titlename, title.c_str(), MAX_PATH);
				pfsd->SetTitle(titlename);

				if(SUCCEEDED(hr))
				{
					// Now show the dialog.
					hr = pfsd->Show(NULL);
					if(SUCCEEDED(hr))
					{
						IShellItem *psiResult;
						hr = pfsd->GetResult(&psiResult);
						if(SUCCEEDED(hr))
						{
							// Get the path to the file.
							PWSTR newFileName;
							hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &newFileName);
							if(SUCCEEDED(hr))
							{
								// Create the requested file
								HANDLE hFile = CreateFileW(newFileName,
										GENERIC_READ | GENERIC_WRITE,
										FILE_SHARE_READ,
										NULL,
										CREATE_ALWAYS,  
										FILE_ATTRIBUTE_NORMAL,
										NULL);
								hr = (hFile == INVALID_HANDLE_VALUE) ? HRESULT_FROM_WIN32(GetLastError()) : S_OK;

								// Close file and get path
								if(SUCCEEDED(hr))
								{
									CloseHandle(hFile);
									wcstombs_s(0, path, newFileName, MAX_PATH);
								}
								CoTaskMemFree(newFileName);
							}
							psiResult->Release();
						}
					}
				}
            }
        }
        pfsd->Release();
    }

    return string(path);
}

string showOpenDialog(const string &file, const string &ext, const string &title, const string &folder)
{
    // CoCreate the File Open Dialog object.
	char path[MAX_PATH] = "\0";
    IFileDialog *pfd = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL,
		CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if(SUCCEEDED(hr))
    {
		// Get the options on the dialog
		DWORD dwFlags;
		hr = pfd->GetOptions(&dwFlags);
		if(SUCCEEDED(hr))
		{
			// Only allow files
			hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
			if(SUCCEEDED(hr))
			{
				// Phrase filter string
				vector<string> filters = split(ext, ";");
				COMDLG_FILTERSPEC *openTypes = 0;
				uint filterCount = filters.size();
				if(!ext.empty() && filterCount > 0)
				{
					openTypes = new COMDLG_FILTERSPEC[filters.size()];
					for(uint i = 0; i < filterCount; i++)
					{
						// Set filter name
						wchar_t filterName[MAX_PATH];
						mbstowcs_s(0, filterName, filters[i].c_str(), MAX_PATH);
						openTypes->pszName = filterName;

						// Set filter spec
						string filterSpecStr = filters[i].substr(filters[i].find_first_of("(")+1,
							filters[i].find_last_of(")")-filters[i].find_first_of("(")-1);
						wchar_t filterSpec[MAX_PATH];
						mbstowcs_s(0, filterSpec, filterSpecStr.c_str(), MAX_PATH);
						openTypes->pszSpec = filterSpec;
					}

					// Set the file types to display.
					hr = pfd->SetFileTypes(filterCount, openTypes);
					delete openTypes;
				}

				if(SUCCEEDED(hr))
				{
					wchar_t filename[MAX_PATH];
					mbstowcs_s(0, filename, file.c_str(), MAX_PATH);
					pfd->SetFileName(filename);
						
					string folderstr = folder;
					replace(folderstr.begin(), folderstr.end(), '/', '\\'); // toNative();
					wchar_t foldername[MAX_PATH];
					mbstowcs_s(0, foldername, folderstr.c_str(), MAX_PATH);

					IShellItem *psiFolder;
					LPWSTR wszPath = foldername;
					if(SUCCEEDED(hr))
					{
						hr = SHCreateItemFromParsingName(wszPath, NULL, IID_PPV_ARGS(&psiFolder));
						if(SUCCEEDED(hr))
							pfd->SetFolder(psiFolder);
					}

					wchar_t titlename[MAX_PATH];
					mbstowcs_s(0, titlename, title.c_str(), MAX_PATH);
					pfd->SetTitle(titlename);

					if(SUCCEEDED(hr))
					{
						// Show the dialog
						hr = pfd->Show(NULL);
						if(SUCCEEDED(hr))
						{
							// Obtain the result, once the user clicks the 'Open' button.
							// The result is an IShellItem object.
							IShellItem *psiResult;
							hr = pfd->GetResult(&psiResult);
							if(SUCCEEDED(hr))
							{
								// We are just going to print out the name of the file for sample sake.
								PWSTR filePath = NULL;
								hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &filePath);
								if(SUCCEEDED(hr))
								{
									wcstombs_s(0, path, filePath, MAX_PATH);
									CoTaskMemFree(filePath);
								}
								psiResult->Release();
							}
						}
					}
				}
			}
		}
        pfd->Release();
    }
	
    return string(path);
}

bool listFolders(string path, vector<string>& folders)
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA fdata;
	string fullpath;
	stack<string> tfolders;
	tfolders.push(path);

	while (!tfolders.empty())
	{
		string path = tfolders.top();
		fullpath = path + "\\*";
		tfolders.pop();

		hFind = FindFirstFile(fullpath.c_str(), &fdata);

		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				wchar_t filename[1024];
				mbstowcs_s(0, filename, fdata.cFileName, 1024);

				if (wcscmp(filename, L".") != 0 &&
                    wcscmp(filename, L"..") != 0)
				{
					if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						tfolders.push(path + "/" + fdata.cFileName);
						folders.push_back(path + "/" + fdata.cFileName);
					}
				}
			}
			while(FindNextFile(hFind, &fdata) != 0);
		}

		if(GetLastError() != ERROR_NO_MORE_FILES)
		{
			FindClose(hFind);

			return false;
		}

		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
	}

	return true;
}

CScriptArray *iosystem::listFiles(const string &dir, const string &mask)
{
    // Obtain a pointer to the engine
	ScriptType *arrayType = scriptEngine->GetObjectTypeById(scriptEngine->GetTypeIdByDecl("array<string>"));

    // Create the array object
    CScriptArray *arr = new CScriptArray(0, arrayType);

	// Make sure its a valid dir
	if(!isDir(dir))
		return arr;

	// List files
	vector<string> files;
	listFiles(dir, mask, files);
	
	// Copy list to out array
	arr->Resize(files.size());
	for(uint i = 0; i < files.size(); i++)
        ((string*)arr->At(i))->assign(files[i]);

	return arr;
}

#endif