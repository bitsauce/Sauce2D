#include "filesystem.h"

// Recursive directory traversal using the Win32 API
Array *FileSystem::listFiles(string &directory, const string &mask, const bool recursive) const
{
	// Create output array
	Array *files = CreateArray("string", 0);
	
	util::toAbsoluteFilePath(directory);
	
	// Folders to search
	vector<string> folders;
	folders.push_back(directory);

	do {
		// Get search path
		string searchPath = folders.back() + mask;
		folders.pop_back();
		
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
					if((fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
					{
						// Found a folder, add it to the list it we're doing a recursive search
						if(recursive) {
							folders.push_back(directory + fdata.cFileName + "/");
						}
					}
					else
					{
						// Found a file, add it to the output list
						string file(directory + fdata.cFileName);
						files->InsertLast(&file);
					}
				}
			}
			while (FindNextFile(hFind, &fdata) != 0);
		}

		FindClose(hFind);
	}
	while(!folders.empty());

	return files;
}