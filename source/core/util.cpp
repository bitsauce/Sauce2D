//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/util.h>
#include <x2d/engine.h>
#include <sstream>
#include <fstream>

vector<string> util::splitString(const string& src, const string& delim)
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

void util::replaceAll(string& str, string& from, string& to)
{
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

string util::toUpper(string &str, const int begin, const int end)
{
	if(begin < 0 || end < 0 || begin >= end)
		transform(str.begin(), str.end(), str.begin(), ::toupper);
	else
		transform(str.begin()+begin, str.begin()+end, str.begin(), ::toupper);
	return str;
}

string util::toLower(string &str, const int begin, const int end)
{
	if(begin < 0 || end < 0 || begin >= end)
		transform(str.begin(), str.end(), str.begin(), ::tolower);
	else
		transform(str.begin()+begin, str.begin()+end, str.begin(), ::tolower);
	return str;
}

int util::strToInt(const string &str)
{
	return atoi(str.c_str());
}

float util::strToFloat(const string &str)
{
	return (float)atof(str.c_str());
}

uchar util::strToAscii(const string &str)
{
	return str.size() != 0 ? str[0] : 0;
}

bool util::strToBool(const string &str)
{
	return (str == "true" || str == "TRUE" || str == "1") ? true : false;
}

string util::intToStr(const int value)
{
	stringstream ss;
	ss << value;    
	return ss.str();
}

string util::floatToStr(const float value)
{
	stringstream ss;
	ss << value;    
	return ss.str();
}

string util::boolToStr(const bool value)
{
	return value ? "true" : "false";
}

string util::asciiToStr(const uchar value)
{
	string s;
	s += value;
	return s;
}

string util::getAbsoluteFilePath(const string &path)
{
	if(path.substr(0, 2) == ":/") {
		return g_engine->getWorkingDirectory() + path.substr(2, string::npos);
	}
	if(path.substr(0, 7) == "saves:/") {
		return g_engine->getSaveDirectory() + path.substr(7, string::npos);
	}
	return path;
}

void util::toAbsoluteFilePath(string &path)
{
	path = getAbsoluteFilePath(path);
}

bool util::fileExists(string filePath)
{
	ifstream s(getAbsoluteFilePath(filePath).c_str());
	bool open = s.is_open();
	s.close();
	return open;
}