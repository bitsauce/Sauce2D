//     _____                        ______             _            
//    / ____|                      |  ____|           (_)           
//   | (___   __ _ _   _  ___ ___  | |__   _ __   __ _ _ _ __   ___ 
//    \___ \ / _` | | | |/ __/ _ \ |  __| | '_ \ / _` | | '_ \ / _ \
//    ____) | (_| | |_| | (_|  __/ | |____| | | | (_| | | | | |  __/
//   |_____/ \__,_|\__,_|\___\___| |______|_| |_|\__, |_|_| |_|\___|
//                                                __/ |             
//                                               |___/              
// Made by Marcus "Bitsauce" Loo Vergara
// 2011-2016 (C)

#include <Sauce/Common.h>
#include <Sauce/Graphics.h>

BEGIN_SAUCE_NAMESPACE

Shader::Shader()
{
}

Shader::~Shader()
{
}

void *ShaderResourceDesc::create() const
{
	GraphicsContext *graphicsContext = Game::Get()->getWindow()->getGraphicsContext();
	string vertexSource, fragmentSource, geometrySource;

	FileReader *fileReader;

	fileReader = new FileReader(util::getAbsoluteFilePath(m_vertexFilePath));
	vertexSource = fileReader->readAll();
	fileReader->close();
	delete fileReader;

	fileReader = new FileReader(util::getAbsoluteFilePath(m_fragmentFilePath));
	fragmentSource = fileReader->readAll();
	fileReader->close();
	delete fileReader;

	if(util::fileExists(m_geometryFilePath))
	{
		fileReader = new FileReader(util::getAbsoluteFilePath(m_geometryFilePath));
		geometrySource = fileReader->readAll();
		fileReader->close();
		delete fileReader;
	}

	LOG("Compiling shader program: %s", getName().c_str());

	return graphicsContext->createShader(vertexSource, fragmentSource, geometrySource);
}

END_SAUCE_NAMESPACE
