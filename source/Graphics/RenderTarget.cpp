//     _____                        ______             _            
//    / ____|                      |  ____|           (_)           
//   | (___   __ _ _   _  ___ ___  | |__   _ __   __ _ _ _ __   ___ 
//    \___ \ / _` | | | |/ __/ _ \ |  __| | '_ \ / _` | | '_ \ / _ \
//    ____) | (_| | |_| | (_|  __/ | |____| | | | (_| | | | | |  __/
//   |_____/ \__,_|\__,_|\___\___| |______|_| |_|\__, |_|_| |_|\___|
//                                                __/ |             
//                                               |___/              
// Made by Marcus "Bitsauce" Loo Vergara
// 2011-2018 (C)

#include <Sauce/Common.h>
#include <Sauce/graphics.h>

BEGIN_SAUCE_NAMESPACE

RenderTarget2D::RenderTarget2D(GraphicsContext *graphicsContext, const uint width, const uint height, const uint count, const PixelFormat &fmt) :
	m_width(width),
	m_height(height),
	m_textureCount(count)
{
	if(width == 0 || height == 0)
	{
		THROW("RenderTarget2D(): width and height must be greater than 0.");
	}
	
	// Allocate textures
	m_textures = new shared_ptr<Texture2D>[m_textureCount];
	for(uint i = 0; i < m_textureCount; ++i)
	{
		m_textures[i] = shared_ptr<Texture2D>(graphicsContext->createTexture(width, height, 0, fmt));
	}
}

RenderTarget2D::RenderTarget2D(GraphicsContext *graphicsContext, shared_ptr<Texture2D> target) :
	m_width(target->getWidth()),
	m_height(target->getHeight()),
	m_textureCount(1)
{
	// Set texture variables
	(m_textures = new shared_ptr<Texture2D>[1])[0] = target;
}

RenderTarget2D::~RenderTarget2D()
{
	delete[] m_textures;
}

END_SAUCE_NAMESPACE
