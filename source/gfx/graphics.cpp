//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/graphics.h>
#include "texture.h"
#include "pixmap.h"

Texture* Graphics::CreateTexture(const Pixmap &pixmap)
{
	return gfx->createTexture(pixmap);
}

Texture* Graphics::CreateTexture(const string &filePath)
{
	//return gfx->createTexture(AssetLoader::LoadPixmap(filePath));
	return gfx->createTexture(Pixmap(0, 0));
}
	
Texture* Graphics::CreateTexture(const int width, const int height)
{
	return gfx->createTexture(Pixmap(width, height));
}
	
Texture* Graphics::CreateTexture(const Texture &texture)
{
	return gfx->createTexture(texture.getPixmap());
}