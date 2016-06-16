#ifndef SAUCE_INI_PARSER_H
#define SAUCE_INI_PARSER_H

#include <Sauce/Common.h>

BEGIN_SAUCE_NAMESPACE

class SAUCE_API IniFile
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

END_SAUCE_NAMESPACE

#endif // SAUCE_INI_PARSER_H
