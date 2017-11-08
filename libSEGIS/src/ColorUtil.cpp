//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  ColorUtil: 
//      Color utility functions
//

#include <SEGIS/ColorUtil.h>
#include <stdio.h>

// Constants for YUV to RGB conversion
static const Float32 kYUV_RGB[3][3][4] =
{
    {
        // Type 0 (ITU-R Rec.601)
        {298.082,      0.0,  408.583, (298.082 * 16 +      0.0 * 128 +  408.583 * 128) / -256 /*-222.921625*/},
        {298.082, -100.291, -208.120, (298.082 * 16 + -100.291 * 128 + -208.120 * 128) / -256 /* 135.575375*/},
        {298.082,  516.411,      0.0, (298.082 * 16 +  516.411 * 128 +      0.0 * 128) / -256 /*-276.835625*/}
    },
    {
        // Type 1
        {1.0, 0.0, 1.371, -175.488},
        {1.0, -0.336, -0.698, 132.352},
        {1.0, 1.732, 0.0, -221.696}
    },
    {
        // Type 2
        {1.0, 0.0, 1.540, -197.12},
        {1.0, -0.183, -0.459, 82.176},
        {1.0, 1.816, 0.0, -232.448}
    }
};

#define LIMIT_0_255(X) ((X) > 255.0f ? 255.0f : ((X) < 0.0f ? 0.0f : (X)))

#define YUV_TO_R(TYPE, Y, U, V) (((kYUV_RGB[TYPE][0][0] * Y + \
                                   kYUV_RGB[TYPE][0][1] * U + \
                                   kYUV_RGB[TYPE][0][2] * V) / 256) + \
                                   kYUV_RGB[TYPE][0][3] + 0.5f)

#define YUV_TO_G(TYPE, Y, U, V) (((kYUV_RGB[TYPE][1][0] * Y + \
                                   kYUV_RGB[TYPE][1][1] * U + \
                                   kYUV_RGB[TYPE][1][2] * V) / 256) + \
                                   kYUV_RGB[TYPE][1][3] + 0.5f)

#define YUV_TO_B(TYPE, Y, U, V) (((kYUV_RGB[TYPE][2][0] * Y + \
                                   kYUV_RGB[TYPE][2][1] * U + \
                                   kYUV_RGB[TYPE][2][2] * V) / 256) + \
                                   kYUV_RGB[TYPE][2][3] + 0.5f)

void 
ColorUtil::convertYV12toRGB24(UInt08* aDst, UInt08* aSrc, 
                                UInt32 aWidth, UInt32 aHeight)
{
    UInt08* lDstR = aDst;
    UInt08* lDstG = aDst + 1;
    UInt08* lDstB = aDst + 2;

    UInt08* lSrcY = aSrc;
    UInt08* lSrcV = aSrc + aWidth * aHeight;
    UInt08* lSrcU = lSrcV + (aWidth>>1) * (aHeight>>1);

    UInt32  mType = 0;

    for (UInt32 h = 0; h < aHeight; h+=2)
    {
        for (UInt32 w = 0; w < aWidth; w++)
        {
            const Float32 y = *lSrcY++;
            const Float32 u = *lSrcU;
            const Float32 v = *lSrcV;

            const Float32 r = YUV_TO_R(mType, y, u, v);
            const Float32 g = YUV_TO_G(mType, y, u, v);
            const Float32 b = YUV_TO_B(mType, y, u, v);

            *lDstR = (UInt08)LIMIT_0_255(r);
            *lDstG = (UInt08)LIMIT_0_255(g);
            *lDstB = (UInt08)LIMIT_0_255(b);

            lDstR += 3;
            lDstG += 3;
            lDstB += 3;

            if (w & 1)
            {
                lSrcU++;
                lSrcV++;
            }
        }

        lSrcU -= aWidth >> 1;
        lSrcV -= aWidth >> 1;

        for (UInt32 w = 0; w < aWidth; w++)
        {
            const Float32 y = *lSrcY++;
            const Float32 u = *lSrcU;
            const Float32 v = *lSrcV;

            const Float32 r = YUV_TO_R(mType, y, u, v);
            const Float32 g = YUV_TO_G(mType, y, u, v);
            const Float32 b = YUV_TO_B(mType, y, u, v);

            *lDstR = (UInt08)LIMIT_0_255(r);
            *lDstG = (UInt08)LIMIT_0_255(g);
            *lDstB = (UInt08)LIMIT_0_255(b);

            lDstR += 3;
            lDstG += 3;
            lDstB += 3;

            if (w & 1)
            {
                lSrcU++;
                lSrcV++;
            }
        }
    }
}

void 
ColorUtil::convertBGR24toRGB24(UInt08* aDst, UInt08* aSrc, 
                               UInt32 aWidth, UInt32 aHeight)
{
    UInt08* lDstR = aDst + 0;
    UInt08* lDstG = aDst + 1;
    UInt08* lDstB = aDst + 2;

    UInt08* lSrcR = aDst + 2;
    UInt08* lSrcG = aDst + 1;
    UInt08* lSrcB = aDst + 0;
    
    for (UInt32 i = 0; i < aWidth * aHeight; i++)
    {
        *lDstR = *lSrcR;
        *lDstG = *lSrcG;
        *lDstB = *lSrcB;
        lSrcR+=3;
        lSrcG+=3;
        lSrcB+=3;
        lDstR+=3;
        lDstG+=3;
        lDstB+=3;
    }
}
void 
ColorUtil::convertYUV420toRGB24(UInt08* aDst, UInt08* aSrc, 
                                UInt32 aWidth, UInt32 aHeight)
{
    UInt08* lDstR = aDst;
    UInt08* lDstG = aDst + 1;
    UInt08* lDstB = aDst + 2;

    UInt08* lSrcY = aSrc;
    UInt08* lSrcU = aSrc + aWidth * aHeight;
    UInt08* lSrcV = lSrcU + (aWidth>>1) * (aHeight>>1);

    UInt32  mType = 0;

    for (UInt32 h = 0; h < aHeight; h+=2)
    {
        for (UInt32 w = 0; w < aWidth; w++)
        {
            const Float32 y = *lSrcY++;
            const Float32 u = *lSrcU;
            const Float32 v = *lSrcV;

            const Float32 r = YUV_TO_R(mType, y, u, v);
            const Float32 g = YUV_TO_G(mType, y, u, v);
            const Float32 b = YUV_TO_B(mType, y, u, v);

            *lDstR = (UInt08)LIMIT_0_255(r);
            *lDstG = (UInt08)LIMIT_0_255(g);
            *lDstB = (UInt08)LIMIT_0_255(b);

            lDstR += 3;
            lDstG += 3;
            lDstB += 3;

            if (w & 1)
            {
                lSrcU++;
                lSrcV++;
            }
        }

        lSrcU -= aWidth >> 1;
        lSrcV -= aWidth >> 1;

        for (UInt32 w = 0; w < aWidth; w++)
        {
            const Float32 y = *lSrcY++;
            const Float32 u = *lSrcU;
            const Float32 v = *lSrcV;

            const Float32 r = YUV_TO_R(mType, y, u, v);
            const Float32 g = YUV_TO_G(mType, y, u, v);
            const Float32 b = YUV_TO_B(mType, y, u, v);

            *lDstR = (UInt08)LIMIT_0_255(r);
            *lDstG = (UInt08)LIMIT_0_255(g);
            *lDstB = (UInt08)LIMIT_0_255(b);

            lDstR += 3;
            lDstG += 3;
            lDstB += 3;

            if (w & 1)
            {
                lSrcU++;
                lSrcV++;
            }
        }
    }
}

void 
ColorUtil::convertYUV422toRGB24(UInt08* aDst, UInt08* aSrc, 
                                UInt32 aWidth, UInt32 aHeight)
{
    UInt08* lDstR = aDst;
    UInt08* lDstG = aDst + 1;
    UInt08* lDstB = aDst + 2;

    UInt08* lSrcY = aSrc;
    UInt08* lSrcU = aSrc + aWidth * aHeight;
    UInt08* lSrcV = lSrcU + (aWidth>>1) * aHeight;

    UInt32  mType = 0;

    for (UInt32 h = 0; h < aHeight; h++)
    {
        for (UInt32 w = 0; w < aWidth; w++)
        {
            const Float32 y = *lSrcY++;
            const Float32 u = *lSrcU;
            const Float32 v = *lSrcV;

            const Float32 r = YUV_TO_R(mType, y, u, v);
            const Float32 g = YUV_TO_G(mType, y, u, v);
            const Float32 b = YUV_TO_B(mType, y, u, v);

            *lDstR = (UInt08)LIMIT_0_255(r);
            *lDstG = (UInt08)LIMIT_0_255(g);
            *lDstB = (UInt08)LIMIT_0_255(b);

            lDstR += 3;
            lDstG += 3;
            lDstB += 3;

            if (w & 1)
            {
                lSrcU++;
                lSrcV++;
            }
        }
    }
}

void 
ColorUtil::convertYUV444toRGB24(UInt08* aDst, UInt08* aSrc, 
                                UInt32 aWidth, UInt32 aHeight)
{
    UInt08* lDstR = aDst;
    UInt08* lDstG = aDst + 1;
    UInt08* lDstB = aDst + 2;

    UInt08* lSrcY = aSrc;
    UInt08* lSrcU = aSrc + aWidth * aHeight;
    UInt08* lSrcV = lSrcU + aWidth * aHeight;

    UInt32  mType = 0;

    for (UInt32 h = 0; h < aHeight; h++)
    {
        for (UInt32 w = 0; w < aWidth; w++)
        {
            const Float32 y = *lSrcY++;
            const Float32 u = *lSrcU++;
            const Float32 v = *lSrcV++;

            const Float32 r = YUV_TO_R(mType, y, u, v);
            const Float32 g = YUV_TO_G(mType, y, u, v);
            const Float32 b = YUV_TO_B(mType, y, u, v);

            *lDstR = (UInt08)LIMIT_0_255(r);
            *lDstG = (UInt08)LIMIT_0_255(g);
            *lDstB = (UInt08)LIMIT_0_255(b);

            lDstR += 3;
            lDstG += 3;
            lDstB += 3;
        }
    }
}
