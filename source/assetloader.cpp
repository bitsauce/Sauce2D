//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/              
//		Macro#byte (C)

#include "x2d/assetloader.h"
#include "x2d/gfx.h"
#include "x2d/iosystem.h"

//-------------------------------------------------------------
// PNG
//-------------------------------------------------------------

#define TEXTURE_LOAD_ERROR 0
#define PNG_SIG_SIZE 8

// Keep output clean
#pragma warning(disable : 4611) 

#include <png.h>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <fstream>

int readOffset = 0;

bool validate(const char* buffer)
{
    // Allocate a buffer of 8 bytes, where we can put the file signature.
    png_byte pngsig[PNG_SIG_SIZE];

    // Check if the read worked...
    if(buffer == 0) return false;

	// Copy signature
	memcpy(pngsig, buffer, PNG_SIG_SIZE);
	readOffset += PNG_SIG_SIZE;

    // Let libpng check the sig. If this function returns 0, everything is OK.
    return png_sig_cmp(pngsig, 0, PNG_SIG_SIZE) == 0;
}

void userReadData(png_structp pngPtr, png_bytep data, png_size_t length)
{
    png_voidp a = png_get_io_ptr(pngPtr);
	memcpy(data, (const char*)a + readOffset, length);
	readOffset += length;
}

int loadPngFromBuffer(const char* buffer, uint **data, uint &width, uint &height)
{
	readOffset = 0;

	// First, we validate our buffer
	if(!validate(buffer))
	{
		iosystem::error("Data is not valid PNG-data");
		return TEXTURE_LOAD_ERROR;
	}

	// Here we create the png read struct. The 3 NULL's at the end can be used
	// for your own custom error handling functions, but we'll just use the default.
	// if the function fails, NULL is returned. Always check the return values!
	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!pngPtr) {
		iosystem::error("Couldn't initialize png read struct");
		return TEXTURE_LOAD_ERROR;
	}

	//Here we create the png info struct.
	//Note that this time, if this function fails, we have to clean up the read struct!
	png_infop infoPtr = png_create_info_struct(pngPtr);
	if(!infoPtr) {
		iosystem::error("Couldn't initialize png info struct");
		png_destroy_read_struct(&pngPtr, (png_infopp)0, (png_infopp)0);
		return TEXTURE_LOAD_ERROR;
	}

	// Here I've defined 2 pointers up front, so I can use them in error handling.
	// I will explain these 2 later. Just making sure these get deleted on error.
	png_bytep* rowPtrs = 0;

	if(setjmp(png_jmpbuf(pngPtr)))
	{
		// An error occured, so clean up what we have allocated so far...
		png_destroy_read_struct(&pngPtr, &infoPtr,(png_infopp)0);
		delete[] rowPtrs;

		iosystem::error("An error occured while reading the PNG file");

		// Make sure you return here. libPNG will jump to here if something
		// goes wrong, and if you continue with your normal code, you might
		// End up with an infinite loop.
		return TEXTURE_LOAD_ERROR;
	}

	png_set_read_fn(pngPtr, (void*)buffer, userReadData); // Try this: (void*)buffer, if this line causes crash

	// Set the amount signature bytes we've already read:
    // We've defined PNGSIGSIZE as 8;
    png_set_sig_bytes(pngPtr, PNG_SIG_SIZE);

    // Now call png_read_info with our pngPtr as image handle, and infoPtr to receive the file info.
    png_read_info(pngPtr, infoPtr);
	png_uint_32 imgWidth =  png_get_image_width(pngPtr, infoPtr);
    png_uint_32 imgHeight = png_get_image_height(pngPtr, infoPtr);

    //bits per CHANNEL! note: not per pixel!
    png_uint_32 bitdepth   = png_get_bit_depth(pngPtr, infoPtr);

    //Number of channels
    png_uint_32 channels   = png_get_channels(pngPtr, infoPtr);

    //Color type. (RGB, RGBA, Luminance, luminance alpha... palette... etc)
    png_uint_32 color_type = png_get_color_type(pngPtr, infoPtr);

	// Here's one of the pointers we've defined in the error handler section:
    // Array of row pointers. One for every row.
    rowPtrs = new png_bytep[imgHeight];

    // Allocate image data buffer
	uchar *byteData = new uchar[imgWidth * imgHeight * bitdepth * channels / 8];

    //This is the length in bytes, of one row.
    const uint stride = imgWidth * bitdepth * channels / 8;

    // A little for-loop here to set all the row pointers to the starting
    // Adresses for every row in the buffer
    for(size_t i = 0; i < imgHeight; i++)
	{
        // Set the pointer to the data pointer + i times the row stride.
        // Notice that the row order is reversed with q.
        // This is how at least OpenGL expects it,
        // and how many other image loaders present the data.
        rowPtrs[i] = (png_bytep)byteData + (imgHeight - i-1) * stride;
    }

    // And here it is! The actuall reading of the image!
    // Read the imagedata and write it to the adresses pointed to
    // by rowptrs (in other words: our image databuffer)
    png_read_image(pngPtr, rowPtrs);
 
    // Delete the row pointers array
    delete[] (png_bytep)rowPtrs;

	if(channels == 3)
	{
		// Convert RGB to RGBA
		uint size = imgWidth * imgHeight;
		uchar *newData = new uchar[size*4];
		uint j = 0, k = 0;
		for(uint i = 0; i < size; i++)
		{
			newData[j++] = byteData[k++];
			newData[j++] = byteData[k++];
			newData[j++] = byteData[k++];
			newData[j++] = 255;
		}
		delete[] byteData;
		byteData = newData;
	}

    // Clean up the read and info structs
    png_destroy_read_struct(&pngPtr, &infoPtr,(png_infopp)0);

	// Set out data
	width = imgWidth;
	height = imgHeight;
	*data = (uint*)byteData;

	return 0;
}

int savePng(const char* filePath, uint *data, const uint width, const uint height)
{
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep row;
	
	// Open file for writing (binary mode)
	fp = fopen(filePath, "wb");
	if(fp == 0)
	{
		iosystem::error("Could not open file '%s' for writing", filePath);
		fclose(fp);
		return -1;
	}

	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr == 0)
	{
		iosystem::error("Could not allocate write struct");
		png_destroy_write_struct(&png_ptr, (png_infopp)0);
		fclose(fp);
		return -2;
	}

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == 0)
	{
		iosystem::error("Could not allocate info struct");
		png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
		png_destroy_write_struct(&png_ptr, (png_infopp)0);
		fclose(fp);
		return -3;
	}

	// Setup Exception handling
	if(setjmp(png_jmpbuf(png_ptr)))
	{
		iosystem::error("Error during png creation");
		png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
		png_destroy_write_struct(&png_ptr, (png_infopp)0);
		fclose(fp);
		return -4;
	}

	png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, width, height,
			8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row (4 bytes per pixel - RGBA)
	uint size = 4*width*sizeof(png_byte);
	row = (png_bytep)new uchar[size];

	// Write image data
	for(uint y = height-1; y >= 0; y--)
	{
		memcpy(row, data + y*width, size);
		png_write_row(png_ptr, row);
	}

	// End write
	png_write_end(png_ptr, 0);

	// Clean up
	fclose(fp);
	png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	png_destroy_write_struct(&png_ptr, (png_infopp)0);
	delete[] row;

	return 0;
}

//-------------------------------------------------------------
// BMP
//-------------------------------------------------------------

#pragma pack(push)
#pragma pack(1)

// Taken out of wingdi.h and rephrased
struct BitmapInfoHeader
{
	ulong  size;
	long   width;
	long   height;
	ushort planes;
	ushort bitCount;
	ulong  compression;
	ulong  sizeImage;
	long   xPelsPerMeter;
	long   yPelsPerMeter;
	ulong  clrUsed;
	ulong  clrImportant;
};

// Taken out of wingdi.h and rephrased
struct BitmapFileHeader
{
	ushort type;
	ulong  size;
	ushort reserved1;
	ushort reserved2;
	ulong  offBits;
};

#pragma pack(pop)

void readBytes(void *dest, void *buffer, int length)
{
	memcpy(dest, (const char*)buffer + readOffset, length);
	readOffset += length;
}

int loadBmpFromBuffer(const char *buffer, uint **pixels, uint &width, uint &height)
{
	// Create bitmap headers
	BitmapFileHeader bitmapFileHeader;	// Our bitmap file header
	BitmapInfoHeader bitmapInfoHeader;	// Our bitmap info header

	// Validate our buffer
	if(buffer == 0)
		return -1;
	readOffset = 0;

	// Read the bitmap file header
	readBytes(&bitmapFileHeader, (void*)buffer, sizeof(BitmapFileHeader));

	// Verify that this is a bmp file by check bitmap id
	if(bitmapFileHeader.type != 0x4D42)
		return -2; // Not a valid bitmap header

	// Read the bitmap info header
	readBytes(&bitmapInfoHeader, (void*)buffer, sizeof(BitmapInfoHeader));

	// Move file point to the begging of bitmap data
	readOffset = bitmapFileHeader.offBits;
	
	// Create texture data
	uint *data = new uint[bitmapInfoHeader.width*bitmapInfoHeader.height];

	// Setup temp row buffer
	int paddedRow = (bitmapInfoHeader.width*3 + 3) & (~3);
	uchar *rowData = new uchar[paddedRow];

	// Read texture data
	uint pos = 0;
	for(uint i = 0; i < (uint)bitmapInfoHeader.height; i++)
	{
		// Read row by row
		readBytes(rowData, (void*)buffer, paddedRow);
		for(uint j = 0; j < (uint)bitmapInfoHeader.width*3; j += 3)
		{
			// Read BGR as RGB
			data[pos++] = rgb(rowData[j+2], rowData[j+1], rowData[j]);
		}
	}

	// Delete temp row buffer
	delete[] rowData;

	// Set out values
	width = bitmapInfoHeader.width;
	height = bitmapInfoHeader.height;
	*pixels = data;

	// Return texture id
	return 0;
}

int saveBmp(const string &filePath, uint *data, const uint width, const uint height)
{
	return 0;
}

/*void TextureLoader::saveBitmapTexture(const int texId, const string &filePath)
{
	// Get texture pixel data
	if(texId < 0) return;
	if(gameEngine->gfx->Textures.find(texId) == gameEngine->gfx->Textures.end()) return;

	// Get texture and bind
	GfxManager::Texture *texture = &gameEngine->gfx->Textures[texId];
	glBindTexture(GL_TEXTURE_2D, texId);

	// Read texture data
	uchar *texData = new uchar[texture->width*texture->height*4];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create file
	ofstream file;
	file.open(filePath, ios::out | ios::trunc | ios::binary);

	// Create header
	BITMAPFILEHEADER header;
	memset(&header, 0, sizeof (BITMAPFILEHEADER));
	header.bfType = 0x4D42;
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;
	header.bfSize = sizeof(BITMAPFILEHEADER);
	header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	BITMAPINFOHEADER info;
	memset(&info, 0, sizeof(BITMAPINFOHEADER));
	info.biSize = sizeof(BITMAPINFOHEADER);
	info.biWidth = texture->width;
	info.biHeight = texture->height;
	info.biPlanes = 1;
	info.biBitCount = 24;
	info.biCompression = BI_RGB;
	
	// Calculate padding
	int bpp = (info.biBitCount / 8);
	long pitch = (info.biWidth * bpp);
	if(pitch % 4 != 0)
	{
		pitch += 4 - (pitch % 4);
	}
	long padding = pitch - (info.biWidth * bpp);

	info.biSizeImage = (info.biHeight * info.biWidth * bpp) + (padding*info.biHeight);
	info.biXPelsPerMeter = 2400;
	info.biYPelsPerMeter = 2400;   
	info.biClrUsed = 0;
	info.biClrImportant = 0;

	file.write((char*)(&header), sizeof(BITMAPFILEHEADER));
	file.write((char*)(&info), sizeof(BITMAPINFOHEADER));

	// Write data
	unsigned char* data = new unsigned char[bpp];
	for(uint i = 0; i < info.biSizeImage; i += bpp)
	{
		// Calculate coordinates
		int y = int(float(i)/pitch);
		int x = (i-(pitch*y))/bpp;

		// Write pixel
		data[0] = texData[i*4+2]; // B
		data[1] = texData[i*4+1]; // G
		data[2] = texData[i*4];   // R

		// Write out data
		file.write((char*)data, bpp);

		// Write padding
		if(x == info.biWidth-1)
		{
			for(int j = 0; j < padding; j++)
				file.write("\0", 1);
			i += padding;
		}
	}

	delete data;
	file.close();
}*/

// Load image from assets
int X2DAssetLoader::loadAssetAsImage(const char* assetPath, uint** data, uint &width, uint &height, const X2DImageFormat format)
{
	int r = X2D_OK;
	if(format == UnknownImage)
	{
		// Get extention
		string ext(assetPath);
		ext = iosystem::getFileExt(ext);
		ext = math::toLower(ext);

		// Load asset as a image
		const char *assetData = 0;
		loadAsset(assetPath, &assetData);
		if(ext == "png")
			loadPngFromBuffer(assetData, data, width, height);
		else if(ext == "bmp")
			loadBmpFromBuffer(assetData, data, width, height);
		else
			r = X2D_InvalidImageFormat;
		delete[] assetData;
	}
	return r;
}

int X2DAssetLoader::saveImage(const char* filePath, uint *data, const uint width, const uint height, const X2DImageFormat format)
{
	if(format == UnknownImage)
	{
		// Get extention
		string ext(filePath);
		ext = iosystem::getFileExt(ext);
		ext = math::toLower(ext);

		// Load file as a image
		if(ext == "png")
			savePng(filePath, data, width, height);
		else if(ext == "bmp")
			saveBmp(filePath, data, width, height);
		else
			return X2D_InvalidImageFormat;
	}
	return X2D_OK;
}