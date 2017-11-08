//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  Types: 
//      Some basic types
//

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Defines.h"
#include "SEGISGL.h"
#include <string>
#include <boost/filesystem.hpp>

typedef int8_t SInt08;
typedef uint8_t UInt08;
typedef int16_t SInt16;
typedef uint16_t UInt16;
typedef int32_t SInt32;
typedef uint32_t UInt32;

typedef float Float32;
typedef double Float64;

typedef Float64 TimeStamp;

typedef std::string String;
typedef std::wstring WString;

typedef boost::filesystem::path Path;

typedef enum
{
    kSucceeded,
    kFailed,
    kInformative,
    kBadArgument
} Status;

typedef enum
{
    kNumberInteger,
    kNumberFloat
} NumberType;

typedef struct 
{
    Float64 x;
    Float64 y;
    Float64 z;
} Vector3D;

typedef struct 
{
    SInt32 x;
    SInt32 y;
} Point2D;

typedef struct 
{
    UInt08 r;
    UInt08 g;
    UInt08 b;
} Color;

typedef struct
{
    Vector3D p1;    
    Vector3D p2;    
    Vector3D p3;    
    Vector3D p4;    
} Rectangle3D;

typedef struct
{
    SInt32 x;    
    SInt32 y;    
    SInt32 w;    
    SInt32 h;    
} Rectangle2D;

typedef struct
{
    Float32 x;    
    Float32 y;    
    Float32 w;    
    Float32 h;    
} Rectangle2f;

typedef struct 
{
    Vector3D viewPos;      // View position
    Vector3D viewDir;      // View direction vector
    Vector3D viewUp;       // View up direction
    Vector3D rotPoint;     // Point to rotate about
    Float64  focalLength;  // Focal Length along view direction
    Float64  aperture;     // gCamera aperture
    Float64  eyeSep;       // Eye separation
    SInt32   screenWidth;  // current window/screen width
    SInt32   screenHeight; // current window/screen height
} Camera;

enum ColorFormat
{
    kColorRGB24 =  0x0000,
    kColorBGR24 =  0x0001,
    // other rgb   0x00XX
    kColorYUV420 = 0x1000,
    kColorYUV422 = 0x1001,
    kColorYUV444 = 0x1002,
    kColorYV12   = 0x1003
    // other yuv   0x10XX
};

enum MipMapMode
{
    kNearest        = GL_NEAREST,
    kLinear         = GL_LINEAR,
    kNearestNearest = GL_NEAREST_MIPMAP_NEAREST,
    kLinearNearest  = GL_NEAREST_MIPMAP_NEAREST,
    kNearestLinear  = GL_NEAREST_MIPMAP_LINEAR,
    kLinearLinear   = GL_LINEAR_MIPMAP_LINEAR
};
