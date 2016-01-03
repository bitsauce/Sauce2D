#ifndef CGF_INI_PARSER_H
#define CGF_INI_PARSER_H

#include <CGF/Common.h>

BEGIN_CGF_NAMESPACE

class CGF_API IniFile
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

END_CGF_NAMESPACE

#endif // CGF_INI_PARSER_H
