//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUITexturePanel: 
//      Implementation
//

#include <SEGIS/GUITexturePanel.h>
#include <SEGIS/ColorUtil.h>
#include <SEGIS/ImageLoader.h>

GUITexturePanel::GUITexturePanel(UInt32      aID,
                                 Rectangle2D aBounds)
    : GUIElement(aID, aBounds)
    , mTextureID(-1)
    , mWidth(aBounds.w)
    , mHeight(aBounds.h)
    , mColorFormat(kColorRGB24)
    , mMipMapMode(kNearest)
    , mPropertiesAreSet(false)
    , mPropertiesAreApplied(false)
    , mIsAutoResize(false)
    , mRotationAngle(0.0f)
    , mAutoRotate(false)
    , mImage(nullptr)
{
    mIsAnimatable = true;
}

GUITexturePanel::GUITexturePanel(UInt32         aID,
                                 Rectangle2D    aBounds,
                                 UInt32         aPadding,
                                 SizeConstraint aSizeConstraint,
                                 FillDirection  aFillDirection,
                                 Alignment      aAlignment)
    : GUIElement(aID, aBounds, aPadding, aSizeConstraint, aFillDirection, aAlignment)
    , mTextureID(-1)
    , mWidth(aBounds.w)
    , mHeight(aBounds.h)
    , mColorFormat(kColorRGB24)
    , mMipMapMode(kNearest)
    , mPropertiesAreSet(false)
    , mPropertiesAreApplied(false)
    , mIsAutoResize(false)
    , mRotationAngle(0.0f)
    , mAutoRotate(false)
    , mImage(nullptr)
{
    mIsAnimatable = true;
}

GUITexturePanel::~GUITexturePanel()
{
    if (mTextureID >= 0)
    {
        glDeleteTextures(1, (GLuint*)&mTextureID);
    }
    delete mImage;
}

Status 
GUITexturePanel::draw(void)
{
    GUIElement::draw();
    if (mPropertiesAreApplied)
    {
        UInt32 lBoxPosX = mBounds.x;
        UInt32 lBoxPosY = mBounds.y;
        UInt32 lBoxW;
        UInt32 lBoxH;

        if (mIsAutoResize)
        {
            lBoxW = mBounds.w;
            lBoxH = mBounds.h;
        }
        else
        {
            lBoxW = mWidth;
            lBoxH = mHeight;
            if (mWidth <  UInt32(mBounds.w) &&
                mHeight < UInt32(mBounds.h))
            {
                // lets center the texture
                lBoxPosX = mBounds.x + mBounds.w / 2 - mWidth / 2;
                lBoxPosY = mBounds.y + mBounds.h / 2 - mHeight / 2;
            }
        }

        glPushMatrix();

        glScalef(1, -1, 0);
        // rotate to screen coordinates
        if (mAutoRotate)
        {
            glRotatef(mRotationAngle, 0, 0, -1);
        }
        
        GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY, lBoxW, lBoxH);


        glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mTextureID);

        glBegin(GL_QUADS);
        {
            glTexCoord2i(0, 0); 
            glVertex2i(0, 0);
            glTexCoord2i(1,0); 
            glVertex2i(lBoxW, 0);
            glTexCoord2i(1,1);
            glVertex2i(lBoxW,lBoxH);
            glTexCoord2i(0,1); 
            glVertex2i(0,lBoxH);
        }
        glEnd();

        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        return kSucceeded;
    }
    else
    {
        return kFailed;
    }
}

void 
GUITexturePanel::setTextureProperties(UInt32      aWidth, 
                                      UInt32      aHeight,
                                      ColorFormat aColorFormat,
                                      MipMapMode  aMipMapMode)
{
    mWidth = aWidth;
    mHeight = aHeight;
    mColorFormat = aColorFormat;
    mMipMapMode = aMipMapMode;
    mPropertiesAreSet = true;
    mPropertiesAreApplied = false;
}

void
GUITexturePanel::setAutoResize(UInt32 aIsAutoResize)
{
    mIsAutoResize = aIsAutoResize;
}

void 
GUITexturePanel::setTextureData(UInt08* aImageData)
{
    if (!mPropertiesAreSet)
    {
        fprintf(stderr, "GUITexturePanel::setTextureData - use setTextureProperties to set up texture before setting texture data.\n");
        return;
    }

    if (mTextureID < 0)
    {
        mTextureID = mID;
        glGenTextures(1, (GLuint*)&mTextureID);
    }

    glBindTexture(GL_TEXTURE_2D, mTextureID);
    if (!mPropertiesAreApplied)
    {
        // set both min and mag filter to mMipMapMode
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mMipMapMode);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mMipMapMode);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        mPropertiesAreApplied = true;
    }

    // used to convert to formats not supported by GL
    UInt08* lConversionBuffer = 0; 
    int lGLColorFormat = GL_RGB;
    switch (mColorFormat)
    {
        case kColorRGB24 :
            lGLColorFormat = GL_RGB;
            break;
        case kColorBGR24 :
            lGLColorFormat = GL_BGR;
            break;
        case kColorYUV420 :
            // convert to kColorRGB24
            lConversionBuffer = (UInt08*) calloc(mWidth * mHeight * 3, 1);
            ColorUtil::convertYUV420toRGB24(lConversionBuffer, aImageData, mWidth, mHeight);
            lGLColorFormat = GL_RGB;
            break;
        case kColorYUV422 :
            // convert to kColorRGB24
            lConversionBuffer = (UInt08*) calloc(mWidth * mHeight * 3, 1);
            ColorUtil::convertYUV422toRGB24(lConversionBuffer, aImageData, mWidth, mHeight);
            lGLColorFormat = GL_RGB;
            break;
        case kColorYUV444 :
            // convert to kColorRGB24
            lConversionBuffer = (UInt08*) calloc(mWidth * mHeight * 3, 1);
            ColorUtil::convertYUV444toRGB24(lConversionBuffer, aImageData, mWidth, mHeight);
            lGLColorFormat = GL_RGB;
            break;
        default:
            fprintf(stderr, "GUITexturePanel::setTextureData - Unsupported colo format\n");
            return;
    }

    if (lConversionBuffer)
    {
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 
                          mWidth, mHeight, 
                          lGLColorFormat, 
                          GL_UNSIGNED_BYTE, 
                          (void*)lConversionBuffer);
        // only takes 2^n sizes:
        // glTexImage2D(GL_TEXTURE_2D, 
        //              0,               // detail level
        //              3,               // num color components
        //              mWidth, 
        //              mHeight, 
        //              0,               // border
        //              GL_RGB, 
        //              GL_UNSIGNED_BYTE, 
        //              (void*)lConversionBuffer);
        free(lConversionBuffer);
        lConversionBuffer = 0;
    }
    else
    {
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 
                          mWidth, mHeight, 
                          lGLColorFormat, 
                          GL_UNSIGNED_BYTE, 
                          (void*)aImageData);
    }
}

Status
GUITexturePanel::loadTextureData(Path & aImagePath)
{
    if (mImage)
    {
        delete mImage;
        mImage = nullptr;
    }
    mImage = ImageLoader::loadImage(aImagePath);
    if (!mImage)
    {
        return kFailed;
    }
    setTextureProperties(mImage->mWidth, 
                         mImage->mHeight,
                         kColorBGR24,
                         kLinearLinear);
    setTextureData(mImage->mImageData);
    return kSucceeded;
}

bool
GUITexturePanel::updateAnimation(void)
{
    if (mAutoRotate)
    {
        mRotationAngle += 1.0f;
        if (mRotationAngle >= 360.0)
        {
            mRotationAngle = 0.0f;
        }
        return true;
    }
    return false;
}
