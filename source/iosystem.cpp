//     _                                     ____  ____  
//    / \  _   _ _ __ ___  _ __ __ _  __  __|___ \|  _ \ 
//   / _ \| | | |  __/ _ \|  __/ _  | \ \/ /  __) | | | |
//  / ___ \ |_| | | | (_) | | | (_| |  >  <  / __/| |_| |
// /_/   \_\__ _|_|  \___/|_|  \__ _| /_/\_\|_____|____/ 
//		MixedGraphics (C)

#include "x2d/iosystem.h"
#include "x2d/app.h"
#include "x2d/console.h"
#include "x2d/debug.h"
#include "x2d/engine.h"
#include "x2d/assetloader.h"
#include "scripts/scriptarray.h"

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sys/stat.h>

#ifdef DEPRICATED
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
#endif

// Sets the file extension of a string path
void iosystem::setFileExt(string &path, string &ext)
{
	if(ext.size() > 0 && ext[0] != '.')
		ext.insert(0, "."); // png => .png

	if(string::npos != path.find('.'))
	{
		// If string already contains an extension, remove it
		path = path.substr(0, path.find_last_of('.'));
	}

	path += ext;
}

string iosystem::getFileName(const string &path)
{
	if(string::npos == path.find('/'))
		return path;
	if(string::npos != path.find('.'))
		return path.substr(path.find_last_of('/')+1, path.find_last_of('.')-path.find_last_of('/')-1);
	else
		return path.substr(path.find_last_of('/')+1, path.size()-path.find_last_of('/')-1);
}

string iosystem::getFileExt(const string &path)
{
	return string::npos == path.find('.') ? "" : path.substr(path.find_last_of('.')+1, path.size()-path.find_last_of('.')-1);
}

bool iosystem::isFile(string fileName)
{
	gameEngine->assetLoader->transformFilePath(fileName);

#ifdef X2D_WINDOWS
	// This is the only correct way to tell if a file exists
	// since ifstream s(filename); will fail if the file is
	// all ready in use
	struct _stat buff;
	return (_stat(fileName.c_str(), &buff) == 0 && buff.st_mode & S_IFREG);
#else
	ifstream s(fileName.c_str());
	bool open = s.is_open();
	s.close();
	return open;
#endif
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

void iosystem::print(const string &msg, const X2DMessageType type)
{
	// Send message to debugger log
#if defined(X2D_WINDOWS)
	OutputDebugString((msg + "\n").c_str());
#elif defined(X2D_ANDROID)
#endif

	// Send message to debugger
	if(gameEngine->isEnabled(X2D_Debug))
		gameEngine->debug->sendMessage(msg, type);
}

void iosystem::print(const string &msg)
{
	// Send to print
	print(msg, X2D_PrintMsg);
}

void iosystem::print(const char* msg, ...)
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

void iosystem::error(const char* msg, ...)
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

void iosystem::error(const string &msg)
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

vector<string> iosystem::split(const string& src, const string& delim)
{	
	vector<string> result;
	string::size_type startPos = 0, endPos = 0;
	do{
		endPos = src.find_first_of(delim,startPos);
		string::size_type length = endPos - startPos;
		if(length != 0)
			result.push_back(src.substr(startPos,length));
		startPos = endPos + 1;
	} while(endPos != string::npos);
	return result;
}

#ifdef DEPRICATED
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

// Recursive directory traversal using the Win32 API
bool listFiles(string path, string mask, vector<string>& files)
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA fdata;
	string fullpath;
	stack<string> folders;
	folders.push(path);
	files.clear();

	while(!folders.empty())
	{
		string path = folders.top();
		fullpath = path + "\\" + mask;
		folders.pop();

		hFind = FindFirstFile(fullpath.c_str(), &fdata);

		if(hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				wchar_t filename[1024];
				mbstowcs_s(0, filename, fdata.cFileName, 1024);

				if(wcscmp(filename, L".") != 0 &&
                    wcscmp(filename, L"..") != 0)
				{
					if(fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						folders.push(path + "/" + fdata.cFileName);
					}
					else
					{
						files.push_back(path + "/" + fdata.cFileName);
					}
				}
			}
			while (FindNextFile(hFind, &fdata) != 0);
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

void replaceAll(string& str, string& from, string& to)
{
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

#endif