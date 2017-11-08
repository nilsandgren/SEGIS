/*
 *  ImageLoader.cpp
 *  SEGIS
 *
 *  Created by Nils Andgren on 18/5 -11.
 *  Copyright 2011 Nils Andgren. All rights reserved.
 *
 */

#include <SEGIS/ImageLoader.h>
#include <FreeImage.h>

Image *
ImageLoader::loadImage(Path & aImagePath)
{
	// image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	// pointer to the image, once loaded
	FIBITMAP *dib(0);
	// pointer to the image data
	BYTE* bits(0);
	
    const char * filename = aImagePath.generic_string().c_str();
    
	// check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	// if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	// if still unkown, return failure
	if(fif == FIF_UNKNOWN)
    {
        fprintf(stderr, "ImageLoader::loadImage - Unknown file format\n");
		return 0;
    }
    
	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if(!dib)
    {
        fprintf(stderr, "ImageLoader::loadImage - Could not load \"%s\"\n", filename);
		return 0;
    }

    UInt32 lBitsPerPixel = FreeImage_GetBPP(dib);
    
	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	UInt32 width = FreeImage_GetWidth(dib);
	UInt32 height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
    {
        fprintf(stderr, "ImageLoader::loadImage - Could not get properties from image\n");
        FreeImage_Unload(dib);
		return 0;
    }
    
    Image * lImage = new Image(width, height, bits, (lBitsPerPixel/8) * width * height);
    
    UInt32 i = height;
    UInt32 o = 0;
    UInt32 lStride = (lBitsPerPixel/8) * width;
    for ( ; o < height; o++, i--)
    {
        memcpy(lImage->mImageData + o * lStride,
               bits + (i-1) * lStride,
               lStride);
    }
    
    FreeImage_Unload(dib);
    return lImage;
    
}

ImageLoader::ImageLoader()
{
}

ImageLoader::~ImageLoader()
{
}
