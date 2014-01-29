//     _                                     ____  ____  
//    / \  _   _ _ __ ___  _ __ __ _  __  __|___ \|  _ \ 
//   / _ \| | | |  __/ _ \|  __/ _  | \ \/ /  __) | | | |
//  / ___ \ |_| | | | (_) | | | (_| |  >  <  / __/| |_| |
// /_/   \_\__ _|_|  \___/|_|  \__ _| /_/\_\|_____|____/ 
//		MixedGraphics (C)
//		Inspired by: http://www.codeproject.com/Articles/8342/CIniFile-Class-for-C-A-robust-cross-platform-INI-f/

#include "core/ini.h"
#include "x2d/io.h"

#ifdef DEPRICATE
//
//	IniFile - Reads and writes config files
//

IniFile::IniFile(const string filePath)
{
	// Open file if any
	if(!filePath.empty())
		open(filePath);
}

IniFile::~IniFile()
{
	// Make sure to save
	if(!m_filePath.empty())
	{
		save();
		clear();
	}
}

// A function to trim whitespace from both sides of a given string
void Trim(std::string& str, const std::string & ChrsToTrim = " \t\n\r", int TrimDir = 0)
{
    size_t startIndex = str.find_first_not_of(ChrsToTrim);
    if (startIndex == std::string::npos){str.erase(); return;}
    if (TrimDir < 2) str = str.substr(startIndex, str.size()-startIndex);
    if (TrimDir != 1) str = str.substr(0, str.find_last_not_of(ChrsToTrim) + 1);
}

//
//	File operations
//

bool IniFile::open(const string &file)
{
	// Save if a file is already open
	if(!m_filePath.empty())
		save();

	// Check if file exists
	if(!isFile(file))
	{
		// Make sure the directory exists
		string directory = file.substr(0, file.find_last_of("/"));
		if(!isDir(directory))
			createDirs(directory);

		// Create file
		ofstream outFile(file.c_str(), ios::out);
		outFile.close();
		if(!isFile(file))
		{
			// Unable to write
			print("ERROR: CIniFile unable to write file!");
			return false;
		}
	}else{
		// Create an input filestream
		ifstream inFile(file.c_str());
		if(!inFile.is_open())
		{
			// Unable to read
			print("ERROR: CIniFile unable to read file!");
			return false;
		}
	
		// Read ini file
		string line;
		IniSection *section = NULL;
		while(!getline(inFile, line).eof())
		{
			// Trim whitespace from the ends
			Trim(line);

			// Make sure its not a blank line
			if(!line.empty())
			{
				// Is this line a section?
				if(line.find('[') != string::npos)
				{
					// Erase brackets
					line.erase(line.begin());
					line.erase(line.find(']'));

					// Add section
					section = addSection(line);
				}
				// Is this line a key/value?
				else if(line.find('=') != string::npos)
				{
					// Do we have a section?
					if(!section)
						continue;

					// Create section
					addProperty(section, line.substr(0, line.find('=')), line.substr(line.find('=')+1));
				}
			}
		}
	
		// Close the file
		inFile.close();
	}
	
	// Set filename
	m_filePath = file;
	return true;
}

void IniFile::save()
{	
	// Create the file
	ofstream outFile(m_filePath.c_str());
	if(!outFile.is_open())
	{
		// Unable to create file (write)
		print("ERROR: CIniFile could not be saved!");
		return;
	}

	// Write ini file
	for(list<IniSection*>::iterator it = mContent.begin(); it != mContent.end(); ++it)
	{
		// Make sure we're not writing empty sections
		IniSection *section = (*it);
		if(section->properties.size() == 0)
			continue;

		// Write out section
		outFile << "[" << section->name << "]" << endl;

		for(list<IniProperty*>::iterator it2 = section->properties.begin(); it2 != section->properties.end(); ++it2)
		{
			// Write out properties
			IniProperty *prop = (*it2);
			outFile << prop->name << "=" << prop->value << endl;
		}

		// Extra line
		outFile << endl;
	}

	outFile.close();
}

void IniFile::clear()
{
	// Open check
	if(m_filePath.empty())
	{
		print("ERROR: CIniFile no file to clear!");
		return;
	}

	// Clear all content
	for(list<IniSection*>::iterator it = mContent.begin(); it != mContent.end(); ++it)
	{
		for(list<IniProperty*>::iterator it2 = (*it)->properties.begin(); it2 != (*it)->properties.end(); ++it2)
			delete (*it2);
		(*it)->properties.clear();
		delete (*it);
	}
	mContent.clear();
}

//
//	Value functions
//

void IniFile::setValue(const string &key, const string &value)
{
	// Make sure we have valid arguments
	if(key.empty())
	{
		print("ERROR: CIniFile key cannot be empty!");
		return;
	}

	// Make sure we only have one slash
	if(count(key.begin(), key.end(), '/') != 1)
	{
		print("ERROR: CIniFile more or less than one '/' characer in key!");
		return;
	}

	// Get section and value name
	string sectionName = key.substr(0, key.find_first_of("/"));
	string valueName = key.substr(key.find_first_of("/")+1);
	
	// Get section
	IniSection *section = getSection(sectionName);
	if(!section)
		section = addSection(sectionName);

	// Set value
	if(isProperty(section, valueName))
		setProperty(section, valueName, value);
	else
		addProperty(section, valueName, value);
}

string IniFile::value(const string &key)
{
	// Make sure we have valid arguments
	if(key.empty())
	{
		print("ERROR: CIniFile key cannot be empty!");
		return "";
	}

	// Make sure we only have one slash
	if(count(key.begin(), key.end(), '/') != 1)
	{
		print("ERROR: CIniFile.value() use the '/' characer (only) once!");
		return "";
	}
	
	// Get section and value name
	string sectionName = key.substr(0, key.find_first_of("/"));
	string valueName = key.substr(key.find_first_of("/")+1);
	
	// Get section
	IniSection *section = getSection(sectionName);
	if(!section)
	{
		print("ERROR: CIniFile no section '%s'!", sectionName.c_str());
		return "";
	}

	// Get value
	IniProperty *prop = getProperty(section, valueName);
	if(!prop)
	{
		print("ERROR: CIniFile no value '%s'!", valueName.c_str());
		return "";
	}

	// Return value
	return prop->value;
}

bool IniFile::isValue(const string &key)
{
	// Make sure we have valid arguments
	if(key.empty())
	{
		print("ERROR: CIniFile key cannot be empty!");
		return false;
	}

	// Make sure we only have one slash
	if(count(key.begin(), key.end(), '/') != 1)
	{
		print("ERROR: CIniFile.value() use the '/' characer (only) once!");
		return false;
	}
	
	// Get section and value name
	string sectionName = key.substr(0, key.find_first_of("/"));
	string valueName = key.substr(key.find_first_of("/")+1);
	
	// Get section
	IniSection *section = getSection(sectionName);
	if(!section)
		return false;

	// Get value
	IniProperty *prop = getProperty(section, valueName);
	if(!prop)
		return false;

	// Return value
	return true;
}

void IniFile::removeValue(const string &key)
{
	// Make sure we have valid arguments
	if(key.empty())
	{
		print("ERROR: CIniFile key cannot be empty!");
		return;
	}

	// Make sure we only have one slash
	if(count(key.begin(), key.end(), '/') != 1)
	{
		print("ERROR: CIniFile use the '/' characer only once!");
		return;
	}
	
	// Get section and value name
	string sectionName = key.substr(0, key.find_first_of("/"));
	string valueName = key.substr(key.find_first_of("/")+1);
	
	// Get section
	IniSection *section = getSection(sectionName);
	if(!section)
	{
		print("ERROR: CIniFile no section '%s'!", sectionName.c_str());
		return;
	}
	removeProperty(section, valueName);
}

IniFile::IniSection *IniFile::addSection(const string &name)
{
	// Sanity check
	if(isSection(name))
	{
		print("ERROR: CIniFile section already exists!");
		return NULL;
	}

	// Create section
	IniSection *section = new IniSection(); 
	section->name = name;

	// Push back and return
	mContent.push_back(section);
	return section;
}

IniFile::IniSection *IniFile::getSection(const string &name)
{
	for(list<IniSection*>::iterator it = mContent.begin(); it != mContent.end(); ++it)
	{
		IniSection *section = (*it);
		if(section->name == name)
			return section;
	}
	return NULL;
}

bool IniFile::isSection(const string &name)
{
	return getSection(name) != NULL;
}

void IniFile::removeSection(const string &name)
{
	// Get section
	IniSection *section = getSection(name);
	if(!section)
	{
		print("ERROR: CIniFile no section '%s'!", name.c_str());
		return;
	}
	
	// Remove properties
	for(list<IniProperty*>::iterator it = section->properties.begin(); it != section->properties.end(); ++it)
		delete *it;
	section->properties.clear();

	// Remove section
	mContent.remove(section);
	delete section;
}

IniFile::IniProperty *IniFile::addProperty(IniSection *section, const string &name, const string &value)
{
	// Make sure the property dosen't exist from before
	if(isProperty(section, name))
	{
		print("ERROR: CIniFile property already exists!");
		return NULL;
	}
	
	// Define a new property
	IniProperty *prop = new IniProperty;
	prop->name = name;
	prop->value = value;

	// Push back and return
	section->properties.push_back(prop);
	return prop;
}

void IniFile::setProperty(IniSection *section, const string &name, const string &value)
{
	// Sanity check
	if(!section)
	{
		print("ERROR: CIniFile section == NULL!");
		return;
	}

	for(list<IniProperty*>::iterator it = section->properties.begin(); it != section->properties.end(); ++it)
	{
		// Find property
		IniProperty *prop = (*it);
		if(prop->name == name)
		{
			// Property found
			prop->value = value;
			break;
		}
	}
}

IniFile::IniProperty *IniFile::getProperty(IniSection *section, const string &name)
{
	// Sanity check
	if(!section)
	{
		print("ERROR: CIniFile section == NULL!");
		return NULL;
	}

	for(list<IniProperty*>::iterator it = section->properties.begin(); it != section->properties.end(); ++it)
	{
		// Find property
		IniProperty *prop = (*it);
		if(prop->name == name)
			return prop;
	}
	return NULL;
}

bool IniFile::isProperty(IniSection *section, const string &name)
{
	return getProperty(section, name) != NULL;
}

void IniFile::removeProperty(IniSection *section, const string &name)
{
	// Get property
	IniProperty *prop = getProperty(section, name);
	if(!prop)
	{
		print("ERROR: CIniFile no property '%s'!", name.c_str());
		return;
	}

	// Remove section
	section->properties.remove(prop);
	delete prop;
}

//
//	AngelScript
//

IniFile *IniFile::asRefFactory(const string &file)
{
	return new IniFile(file);
}
#endif