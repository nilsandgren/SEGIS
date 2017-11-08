//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIBitmapPanel: 
//      Implementation
//

#include <SEGIS/GUIBitmapPanel.h>
#include <SEGIS/ColorUtil.h>

GUIBitmapPanel::GUIBitmapPanel(UInt32      aID,
                               Rectangle2D aBounds)
    : GUIElement(aID, aBounds)
    , mWidth(aBounds.w)
    , mHeight(aBounds.h)
    , mColorFormat(kColorRGB24)
    , mPropertiesAreSet(false)
    , mYUVBuf(0)
    , mRGBBuf(0)
    , mZoomFactor(-1.0f)
{
    mIsAnimatable = false;
}

GUIBitmapPanel::GUIBitmapPanel(UInt32         aID,
                               Rectangle2D    aBounds,
                               UInt32         aPadding,
                               SizeConstraint aSizeConstraint,
                               FillDirection  aFillDirection,
                               Alignment      aAlignment)
    : GUIElement(aID, aBounds, aPadding, aSizeConstraint, aFillDirection, aAlignment)
    , mWidth(aBounds.w)
    , mHeight(aBounds.h)
    , mColorFormat(kColorRGB24)
    , mPropertiesAreSet(false)
    , mYUVBuf(0)
    , mRGBBuf(0)
    , mZoomFactor(-1.0f)
{
    mIsAnimatable = false;
}

GUIBitmapPanel::~GUIBitmapPanel()
{
    if (mYUVBuf)
    {
        free(mYUVBuf);
        mYUVBuf = 0;
    }
    if (mRGBBuf)
    {
        free(mRGBBuf);
        mRGBBuf = 0;
    }
}

Status 
GUIBitmapPanel::draw(void)
{
    GUIElement::draw();

    if (mPropertiesAreSet)
    {
        UInt32  lBoxPosX = mBounds.x;
        UInt32  lBoxPosY = mBounds.y;
        UInt32  lBoxW = mWidth;
        UInt32  lBoxH = mHeight;
        Float32 lZoomFactor = 1.0f;

        if (mZoomFactor <= 0.0f)
        {
            // auto resize
            if (mBounds.w > mWidth &&
                mBounds.h > mHeight)
            {
                UInt32 lIntZoom = min(mBounds.w/mWidth, mBounds.h/mHeight);
                if (!lIntZoom)
                {
                    lIntZoom = 1;
                }
                lZoomFactor = (Float32) lIntZoom;
            }
            else if (mBounds.w && mBounds.h) // no zero division please
            {
                UInt32 lInvIntZoom = max(mWidth/mBounds.w, mHeight/mBounds.h) + 1;
                if (!lInvIntZoom)
                {
                    lInvIntZoom = 1;
                }
                lZoomFactor = 1.0f / (Float32)lInvIntZoom;
            }
            else
            {
                lZoomFactor = 1.0f;
            }
        }
        else
        {
            lZoomFactor = mZoomFactor;
        }

        lBoxW = UInt32(lBoxW * lZoomFactor);
        lBoxH = UInt32(lBoxH * lZoomFactor);

        if (lBoxW < UInt32(mBounds.w))
        {
            lBoxPosX = lBoxPosX + mBounds.w / 2 - lBoxW / 2;
        }
        if (lBoxH < UInt32(mBounds.h))
        {
            lBoxPosY = lBoxPosY + mBounds.h / 2 - lBoxH / 2;
        }

        if (mRGBBuf)
        {
            glPushMatrix();
            GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY, lBoxW, lBoxH);
            glRasterPos2i(0, lBoxH);
            GLboolean lRasterOK;
            glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID, &lRasterOK);
            if (lRasterOK)
            {
                glPixelZoom(lZoomFactor, -lZoomFactor);
                glDrawPixels(mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, mRGBBuf);
            }
            else
            {
                fprintf(stderr, "GUIBitmapPanel::draw - bad raster position\n");
            }
            glPopMatrix();
        }

        return kSucceeded;
    }
    else
    {
        // no drawing until we are good to go
        return kFailed;
    }
}

void 
GUIBitmapPanel::setBitmapProperties(UInt32      aWidth, 
                                    UInt32      aHeight,
                                    ColorFormat aColorFormat)
{
    mWidth = aWidth;
    mHeight = aHeight;
    mColorFormat = aColorFormat;

    if (kColorRGB24  != mColorFormat &&
        kColorYUV420 != mColorFormat &&
        kColorYUV422 != mColorFormat &&
        kColorYUV444 != mColorFormat &&
        kColorYV12   != mColorFormat)
    {
        fprintf(stderr, "Unsupported color format\n");
        return;
    }

    if (mColorFormat == kColorYUV420 ||
        mColorFormat == kColorYV12)
    {
        if (mYUVBuf)
        {
            free(mYUVBuf);
            mYUVBuf = 0;
        }
        mYUVBuf = (UInt08*) malloc (mWidth * mHeight + mWidth * (mHeight >> 1));
        if (mRGBBuf)
        {
            free(mRGBBuf);
            mRGBBuf = 0;
        }
        mRGBBuf = (UInt08*) malloc (mWidth * mHeight * 3);
    }
    else if (mColorFormat == kColorYUV422)
    {
        if (mYUVBuf)
        {
            free(mYUVBuf);
            mYUVBuf = 0;
        }
        mYUVBuf = (UInt08*) malloc (mWidth * mHeight * 2);
        if (mRGBBuf)
        {
            free(mRGBBuf);
            mRGBBuf = 0;
        }
        mRGBBuf = (UInt08*) malloc (mWidth * mHeight * 3);
    }
    else if (mColorFormat == kColorYUV444)
    {
        if (mYUVBuf)
        {
            free(mYUVBuf);
            mYUVBuf = 0;
        }
        mYUVBuf = (UInt08*) malloc (mWidth * mHeight * 3);
        if (mRGBBuf)
        {
            free(mRGBBuf);
            mRGBBuf = 0;
        }
        mRGBBuf = (UInt08*) malloc (mWidth * mHeight * 3);
    }
    else if (mColorFormat == kColorRGB24)
    {
        if (mYUVBuf)
        {
            free(mYUVBuf);
            mYUVBuf = 0;
        }
        if (mRGBBuf)
        {
            free(mRGBBuf);
            mRGBBuf = 0;
        }
        mRGBBuf = (UInt08*) malloc (mWidth * mHeight * 3);
    }
    mPropertiesAreSet = true;
}

void 
GUIBitmapPanel::setBitmapData(UInt08* aImageData)
{
    if (!mPropertiesAreSet)
    {
        fprintf(stderr, "GUIBitmapPanel::setBitmapData - use setBitmapProperties to set up bitmap before setting data.\n");
        return;
    }

    if (kColorYUV420 == mColorFormat)
    {
        ColorUtil::convertYUV420toRGB24(mRGBBuf, aImageData, mWidth, mHeight);
    }
    else if (kColorYUV422 == mColorFormat)
    {
        ColorUtil::convertYUV422toRGB24(mRGBBuf, aImageData, mWidth, mHeight);
    }
    else if (kColorYUV444 == mColorFormat)
    {
        ColorUtil::convertYUV444toRGB24(mRGBBuf, aImageData, mWidth, mHeight);
    }
    else if (kColorYV12 == mColorFormat)
    {
        ColorUtil::convertYV12toRGB24(mRGBBuf, aImageData, mWidth, mHeight);
    }
    else
    {
        memcpy(mRGBBuf, aImageData, mWidth * mHeight * 3);
    }
}

void
GUIBitmapPanel::setZoomFactor(Float32 aZoomFactor)
{
    mZoomFactor = aZoomFactor;
    if (aZoomFactor <= 0.0f)
    {
        mZoomFactor = -1.0f;
    }
}

UInt32 
GUIBitmapPanel::getMinimumSafeWidth(void)
{
    if (mZoomFactor > 0.0f)
    {
        return mZoomFactor * mWidth;
    }
    else
    {
        return 0;
    }
}

UInt32
GUIBitmapPanel::getMinimumSafeHeight(void)
{
    if (mZoomFactor > 0.0f)
    {
        return mZoomFactor * mHeight;
    }
    else
    {
        return 0;
    }
}
