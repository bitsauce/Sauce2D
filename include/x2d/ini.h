#ifndef CORE_INI_H
#define CORE_INI_H

#include "x2d/config.h"

class XDAPI X2DSettingsFile
{
public:

};

#ifdef DEPRICATE
class X2DAPI X2DSettingsFile
{
public:
	IniFile(const string filePath = "");
	~IniFile();

	// File operations
	bool open(const string &file);
	void save();
	void clear();

	// Value
	void setValue(const string &key, const string &value);
	string value(const string &key);
	bool isValue(const string &key);
	void removeValue(const string &key);

protected:
	
	// IniFile property
	struct IniProperty
	{
		string name;
		string value;
	};
	
	// IniFile section
	struct IniSection
	{
		string name;
		list<IniProperty*> properties;
	};

	// Sections
	IniSection *addSection(const string &name);
	IniSection *getSection(const string &name);
	void removeSection(const string &name);
	bool isSection(const string &name);

	// Properties
	IniProperty *addProperty(IniSection *section, const string &name, const string &value = "");
	void setProperty(IniSection *section, const string &name, const string &value = "");
	IniProperty *getProperty(IniSection *section, const string &name);
	bool isProperty(IniSection *section, const string &name);
	void removeProperty(IniSection *section, const string &name);

private:

	// IniFile
	list<IniSection*> mContent;
	string m_filePath;

	// AngelScript
	static IniFile *asRefFactory(const string &file);
};
#endif

#endif  // CORE_INI_H