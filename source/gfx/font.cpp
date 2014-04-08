//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "font.h"

AS_REGISTER(Font)

int Font::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	return r;
}

Font::Font(const string &filePathOrFontName) :
	refCounter(this)
{
}

float Font::getStringWidth(const string &str)
{
	return 0.0f;
}

float Font::getStringHeight(const string &str)
{
	return 0.0f;
}