//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/extention.h>

AS_REG_REF(XIniFile)

int XIniFile::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	r = scriptEngine->RegisterObjectBehaviour("XIniFile", asBEHAVE_FACTORY, "XIniFile @f(string &in)", asFUNCTIONPR(Factory, (string&), XIniFile*), asCALL_CDECL); AS_ASSERT
		
	r = scriptEngine->RegisterObjectMethod("XIniFile", "string getValue(const string &in, const string &in) const", asMETHOD(XIniFile, getValue), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XIniFile", "void setValue(const string &in, const string &in, const string &in)", asMETHOD(XIniFile, setValue), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("XIniFile", "bool isSection(const string &in) const", asMETHOD(XIniFile, isSection), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("XIniFile", "void save()", asMETHOD(XIniFile, save), asCALL_THISCALL); AS_ASSERT

	return r;
}

XIniFile::XIniFile(string &path) :
	m_path(path)
{
	util::toAbsoluteFilePath(path);

	if(!util::fileExists(path))
		return;

	XFileReader *reader = XFileSystem::CreateFileReader(path);

	string comment, sectionName;
	while(!reader->isEOF())
	{
		string line = reader->readLine();
		if(line.empty())
			continue;
		line = line.substr(line.find_first_not_of(' '), string::npos);

		if(line.front() == ';')
		{
			comment += line.substr(1, string::npos);
		}
		else if(line.front() == '[' && line.back() == ']')
		{
			sectionName = line.substr(1, line.size()-2);

			Section &section = m_sections[sectionName];
			section.comment = comment;
			comment.clear();
		}
		else if(line.find('=') != string::npos && m_sections.find(sectionName) != m_sections.end())
		{
			string keyName = line.substr(0, line.find('='));
			string keyValue = line.substr(line.find('=') + 1, string::npos);

			Key &key = m_sections[sectionName].keys[keyName];
			key.value = keyValue;
			key.comment = comment;
			comment.clear();
		}
	}

	delete reader;
}

string XIniFile::getValue(const string &sec, const string &key) const
{
	if(isSection(sec))
	{
		const Section &section = m_sections.at(sec);
		if(section.keys.find(key) != section.keys.end())
		{
			return section.keys.at(key).value;
		}
	}
	return "";
}

void XIniFile::setValue(const string &sec, const string &key, const string &value)
{
	Section &section = m_sections[sec];
	section.keys[key].value = value;
}

bool XIniFile::isSection(const string &sec) const
{
	return m_sections.find(sec) != m_sections.end();
}

void XIniFile::save()
{
	stringstream out;
	for(map<string, Section>::iterator sectionItr = m_sections.begin(); sectionItr != m_sections.end(); ++sectionItr)
	{
		Section &section = sectionItr->second;
		if(!section.comment.empty()) {
			out << ";" << section.comment << endl;
		}
		out << "[" << sectionItr->first << "]" << endl;

		for(map<string, Key>::iterator keyItr = section.keys.begin(); keyItr != section.keys.end(); ++keyItr)
		{
			if(!keyItr->second.comment.empty()) {
				out << ";" << keyItr->second.comment << endl;
			}
			out << keyItr->first << "=" << keyItr->second.value << endl;
		}

		out << endl;
	}
	
	XFileSystem::WriteFile(m_path, out.str());
}