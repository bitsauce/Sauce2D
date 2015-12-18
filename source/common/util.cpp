//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2015 (C)

#include <x2d/engine.h>
#include <sstream>
#include <fstream>

BEGIN_CG_NAMESPACE

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
	(stringstream&)ss << value;    
	return ss.str();
}

string util::floatToStr(const float value)
{
	stringstream ss;
	(stringstream&)ss << value;    
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

// TODO: Fix me!
string util::getAbsoluteFilePath(const string &path)
{
	/*if(path.substr(0, 2) == ":/") {
		return Engine::getWorkingDirectory() + path.substr(2, string::npos);
	}
	if(path.substr(0, 7) == "saves:/") {
		return Engine::getSaveDirectory() + path.substr(7, string::npos);
	}*/
	return path;
}

void util::toAbsoluteFilePath(string &path)
{
	path = getAbsoluteFilePath(path);
}

void util::toDirectoryPath(string &path)
{
	if(path.back() != '/') {
		path += '/';
	}
}

bool util::fileExists(string filePath)
{
	ifstream s(getAbsoluteFilePath(filePath).c_str());
	bool open = s.is_open();
	s.close();
	return open;
}

int util::decodeUTF8(const char *encodedBuffer, unsigned int *outLength)
{
	const unsigned char *buf = (const unsigned char*)encodedBuffer;

	int value = 0;
	int length = -1;
	unsigned char byte = buf[0];
	if ((byte & 0x80) == 0)
	{
		// This is the only byte
		if (outLength) *outLength = 1;
		return byte;
	}
	else if ((byte & 0xE0) == 0xC0)
	{
		// There is one more byte
		value = int(byte & 0x1F);
		length = 2;

		// The value at this moment must not be less than 2, because 
		// that should have been encoded with one byte only.
		if (value < 2)
			length = -1;
	}
	else if ((byte & 0xF0) == 0xE0)
	{
		// There are two more bytes
		value = int(byte & 0x0F);
		length = 3;
	}
	else if ((byte & 0xF8) == 0xF0)
	{
		// There are three more bytes
		value = int(byte & 0x07);
		length = 4;
	}

	int n = 1;
	for (; n < length; n++)
	{
		byte = buf[n];
		if ((byte & 0xC0) == 0x80)
			value = (value << 6) + int(byte & 0x3F);
		else
			break;
	}

	if (n == length)
	{
		if (outLength) *outLength = (unsigned)length;
		return value;
	}

	// The byte sequence isn't a valid UTF-8 byte sequence.
	return -1;
}

int util::encodeUTF8(unsigned int value, char *outEncodedBuffer, unsigned int *outLength)
{
	unsigned char *buf = (unsigned char*)outEncodedBuffer;

	int length = -1;

	if (value <= 0x7F)
	{
		buf[0] = (uchar) value;
		if (outLength) *outLength = 1;
		return 1;
	}
	else if (value >= 0x80 && value <= 0x7FF)
	{
		// Encode it with 2 characters
		buf[0] = (uchar)(0xC0 + (value >> 6));
		length = 2;
	}
	else if (value >= 0x800 && value <= 0xD7FF || value >= 0xE000 && value <= 0xFFFF)
	{
		// Note: Values 0xD800 to 0xDFFF are not valid unicode characters
		buf[0] = (uchar)(0xE0 + (value >> 12));
		length = 3;
	}
	else if (value >= 0x10000 && value <= 0x10FFFF)
	{
		buf[0] = (uchar)(0xF0 + (value >> 18));
		length = 4;
	}

	int n = length - 1;
	for (; n > 0; n--)
	{
		buf[n] = 0x80 + (value & 0x3F);
		value >>= 6;
	}

	if (outLength) *outLength = length;
	return length;
}

int util::decodeUTF16(const char *encodedBuffer, unsigned int *outLength, util::UnicodeByteOrder byteOrder)
{
	const unsigned char *buf = (const unsigned char *)encodedBuffer;
	int value = 0;
	if (byteOrder == util::LITTLE_ENDIAN)
	{
		value += buf[0];
		value += (unsigned int)(buf[1]) << 8;
	}
	else
	{
		value += buf[1];
		value += (unsigned int)(buf[0]) << 8;
	}

	if (value < 0xD800 || value > 0xDFFF)
	{
		if (outLength) *outLength = 2;
		return value;
	}
	else if (value < 0xDC00)
	{
		// We've found the first surrogate word
		value = ((value & 0x3FF) << 10);

		// Read the second surrogate word
		int value2 = 0;
		if (byteOrder == LITTLE_ENDIAN)
		{
			value2 += buf[2];
			value2 += (unsigned int)(buf[3]) << 8;
		}
		else
		{
			value2 += buf[3];
			value2 += (unsigned int)(buf[2]) << 8;
		}

		// The second surrogate word must be in the 0xDC00 - 0xDFFF range
		if (value2 < 0xDC00 || value2 > 0xDFFF)
			return -1;

		value = value + (value2 & 0x3FF) + 0x10000;
		if (outLength) *outLength = 4;
		return value;
	}

	// It is an illegal sequence if a character in the 0xDC00-0xDFFF range comes first
	return -1;
}

int util::encodeUTF16(unsigned int value, char *outEncodedBuffer, unsigned int *outCharLength, util::UnicodeByteOrder byteOrder)
{
	if (value < 0x10000)
	{
		if (byteOrder == util::LITTLE_ENDIAN)
		{
			outEncodedBuffer[0] = (value & 0xFF);
			outEncodedBuffer[1] = ((value >> 8) & 0xFF);
		}
		else
		{
			outEncodedBuffer[1] = (value & 0xFF);
			outEncodedBuffer[2] = ((value >> 8) & 0xFF);
		}

		if (outCharLength) *outCharLength = 2;
		return 2;
	}
	else
	{
		value -= 0x10000;
		int surrogate1 = ((value >> 10) & 0x3FF) + 0xD800;
		int surrogate2 = (value & 0x3FF) + 0xDC00;

		if (byteOrder == util::LITTLE_ENDIAN)
		{
			outEncodedBuffer[0] = (surrogate1 & 0xFF);
			outEncodedBuffer[1] = ((surrogate1 >> 8) & 0xFF);
			outEncodedBuffer[2] = (surrogate2 & 0xFF);
			outEncodedBuffer[3] = ((surrogate2 >> 8) & 0xFF);
		}
		else
		{
			outEncodedBuffer[1] = (surrogate1 & 0xFF);
			outEncodedBuffer[0] = ((surrogate1 >> 8) & 0xFF);
			outEncodedBuffer[3] = (surrogate2 & 0xFF);
			outEncodedBuffer[2] = ((surrogate2 >> 8) & 0xFF);
		}

		if (outCharLength) *outCharLength = 4;
		return 4;
	}
}

END_CG_NAMESPACE
