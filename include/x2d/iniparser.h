#ifndef X2D_INI_PARSER_H
#define X2D_INI_PARSER_H

#include <x2d/engine.h>

BEGIN_XD_NAMESPACE

class XDAPI IniFile
{
public:
	IniFile(string path);

	string getValue(const string &section, const string &key) const;
	void setValue(const string &section, const string &key, const string &value);

	bool isSection(const string &section) const;

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
};

END_XD_NAMESPACE

#endif // X2D_INI_PARSER_H