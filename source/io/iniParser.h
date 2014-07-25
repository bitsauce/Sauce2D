#ifndef IO_INI_PARSER_H
#define IO_INI_PARSER_H

#include <x2d/base.h>
#include <x2d/util.h>

class IniFile
{
public:
	AS_DECL_REF

	IniFile(string &path);

	string getValue(const string &section, const string &key) const;
	void setValue(const string &section, const string &key, const string &value);

	void save();

private:

	struct Key
	{
		string comment;
		string value;
	};

	struct Section
	{
		string comment;
		map<string, Key> keys;
	};

	map<string, Section> m_sections;
	string m_path;

	static IniFile *Factory(string &path) { return new IniFile(path); }
};

#endif // IO_INI_PARSER_H