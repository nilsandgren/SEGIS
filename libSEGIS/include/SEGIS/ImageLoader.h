/*
 *  ImageLoader.h
 *  SEGIS
 *
 *  Created by Nils Andgren on 18/5 -11.
 *  Copyright 2011 Nils Andgren. All rights reserved.
 *
 */

#pragma once

#include "Types.h"
// Only 24-bit RGB for now
class Image
{
    public:
        Image(UInt32 aWidth,
              UInt32 aHeight,
              UInt08 * aImageData,
              UInt32 aImageDataBytes)
            : mWidth(aWidth)
            , mHeight(aHeight)
            , mImageDataBytes(aImageDataBytes)
        {
            mImageData = new UInt08[aImageDataBytes];
            memcpy(mImageData, aImageData, aImageDataBytes);
        }
        ~Image()
        {
            delete [] mImageData;
        }
        
        UInt32 mWidth;
        UInt32 mHeight;
        UInt08 * mImageData;
        UInt32 mImageDataBytes;
};

class ImageLoader
{
    public:
        static Image * loadImage(Path & aImagePath);
    
    protected:
        ImageLoader();
        ~ImageLoader();
};
