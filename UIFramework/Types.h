#pragma once

#include <stdlib.h>
#include <float.h>
#include <math.h>

#if _WINDOWS

#include <crtdbg.h>

#ifdef _DEBUG

#include "Signals.h"

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#endif

#include <windows.h>
#include <D2D1.h>
#include <D2D1helper.h>

typedef D2D1_SIZE_U SizeU;
typedef D2D1_SIZE_F SizeF;
typedef D2D1_COLOR_F ColorF;
typedef D2D1_RECT_F RectF;
typedef D2D1_POINT_2F Point2F;
typedef D2D_MATRIX_3X2_F Matrix3X2;
typedef D2D1_GRADIENT_STOP GradientStop;

#else

typedef int INT32;
typedef unsigned int UINT32;
typedef float FLOAT;
typedef wchar_t WCHAR;
typedef char CHAR;
typedef UINT32 BOOL;
typedef unsigned char BYTE;

typedef INT32 HRESULT;

#ifndef NULL
#define NULL (0)
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

struct ColorF
{
    FLOAT r;
    FLOAT g;
    FLOAT b;
    FLOAT a;
};

struct Point2U
{
    UINT32 x;
    UINT32 y;
};

struct Point2F
{
    Point2F() : x(0),
                y(0)
    {
    }
    
    Point2F(FLOAT X, FLOAT Y) : x(X),
                                y(Y)
    {
    }
    
    FLOAT x;
    FLOAT y;
};

struct RectF
{
    RectF() : left(0),
              top(0),
              right(0),
              bottom(0)
    {
    }
    
    RectF(FLOAT Left, FLOAT Top, FLOAT Right, FLOAT Bottom) : left(Left),
                                                              top(Top),
                                                              right(Right),
                                                              bottom(Bottom)
    {
    }
    
    FLOAT left;
    FLOAT top;
    FLOAT right;
    FLOAT bottom;
    
};

struct RectU
{
    UINT32 left;
    UINT32 top;
    UINT32 right;
    UINT32 bottom;
};

struct SizeF
{
    SizeF() : width(0),
              height(0)
    {
    }
    
    SizeF(FLOAT Width, FLOAT Height) : width(Width),
                                       height(Height)
    {
    }
    
    FLOAT width;
    FLOAT height;
};

struct SizeU
{
    UINT32 width;
    UINT32 height;
};

struct Matrix3X2
{
    FLOAT _11;
    FLOAT _12;
    FLOAT _21;
    FLOAT _22;
    FLOAT _31;
    FLOAT _32;
    
    inline Matrix3X2 operator*(const Matrix3X2& Other) const
    {
        Matrix3X2 Result;
        
        Result._11 = _11 * Other._11 + _12 * Other._21;
        Result._12 = _11 * Other._12 + _12 * Other._22;
        Result._21 = _21 * Other._11 + _22 * Other._21;
        Result._21 = _21 * Other._12 + _22 * Other._22;
        Result._31 = _31 * Other._11 + _32 * Other._21 + Other._31;
        Result._32 = _31 * Other._12 + _32 * Other._22 + Other._32;
        
        return Result;
    }
    
    inline Point2F TransformPoint(const Point2F& Point) const
    {
        return Point2F(Point.x * _11 + Point.y * _21 + _31, Point.x * _12 + Point.y * _22 + _32);
    }

    inline FLOAT Determinant()
    {
        return (_11 * _22) - (_12 * _21);
    }
    
    inline BOOL IsInvertible()
    {
        FLOAT Det = Determinant();
        
        if(Det == 0.0f)
        {
            return FALSE;
        }
        
        FLOAT Reciprocal = 1.0f / Det;

        if(!isfinite(Reciprocal))
        {
            return FALSE;
        }
        
        return TRUE;
    }
    
    inline BOOL Invert()
    {
        FLOAT Det = Determinant();
        
        if(Det == 0.0f)
        {
            return FALSE;
        }
        
        FLOAT Reciprocal = 1.0f / Det;
        
        if(!isfinite(Reciprocal))
        {
            return FALSE;
        }
        
        Matrix3X2 Copy = *this;
        
        _11 = Copy._22 * Reciprocal;
        _12 = -Copy._12 * Reciprocal;
        _21 = -Copy._21 * Reciprocal;
        _22 = Copy._11 * Reciprocal;
        _31 = (Copy._21 * Copy._32 - Copy._31 * Copy._22) * Reciprocal;
        _32 = (Copy._32 * Copy._12 - Copy._11 * Copy._32) * Reciprocal;
        
        return TRUE;
    }
    
    static inline Matrix3X2 Identity()
    {
        Matrix3X2 Result;
        
        Result._11 = 1.0f;
        Result._12 = 0.0f;
        Result._21 = 0.0f;
        Result._22 = 1.0f;
        Result._31 = 0.0f;
        Result._32 = 0.0f;
        
        return Result;
    }
    
    static inline Matrix3X2 Translation(const SizeF& Size)
    {
        Matrix3X2 Result;
        
        Result._11 = 1.0f;
        Result._12 = 0.0f;
        Result._21 = 0.0f;
        Result._22 = 1.0f;
        Result._31 = Size.width;
        Result._32 = Size.height;
        
        return Result;
    }
    
    static inline Matrix3X2 Translation(FLOAT x, FLOAT y)
    {
        return Translation(SizeF(x, y));
    }
    
    static inline Matrix3X2 Scale(SizeF Size, Point2F Center = Point2F(0, 0))
    {
        Matrix3X2 Result;
        
        Result._11 = Size.width;
        Result._12 = 0.0f;
        Result._21 = 0.0f;
        Result._22 = Size.height;
        Result._31 = Center.x - Size.width * Center.x;
        Result._32 = Center.y - Size.height * Center.y;
        
        return Result;
    }
    
    static inline Matrix3X2 Scale(FLOAT x, FLOAT y, Point2F Center = Point2F(0, 0))
    {
        return Scale(SizeF(x, y), Center);
    }
};

inline Point2F operator*(const Point2F& Point, const Matrix3X2& Matrix)
{
    return Matrix.TransformPoint(Point);
}   

struct GradientStop
{
    FLOAT position;
    ColorF color;
    
};


#include <wchar.h>

inline FLOAT _wtof(const WCHAR* pStr)
{
    return wcstof(pStr, NULL);
}

#endif

struct Nothing;

inline bool operator==( const RectF& Val1, const RectF& Val2 )
{
    return (Val1.left == Val2.left && Val1.top == Val2.top && Val1.right == Val2.right && Val1.bottom == Val2.bottom);
}

inline bool operator!=( const RectF& Val1, const RectF& Val2 )
{
    return (Val1.left != Val2.left || Val1.top != Val2.top || Val1.right != Val2.right || Val1.bottom == Val2.bottom);
}

inline bool operator==( const ColorF& Val1, const ColorF& Val2 )
{
    return (Val1.r == Val2.r && Val1.g == Val2.g && Val1.b == Val2.b && Val1.a == Val2.a);
}

inline bool operator==( const Point2F& Val1, const Point2F& Val2 )
{
    return (Val1.x == Val2.x && Val1.y == Val2.y);
}

inline RectF MakeRect(SizeF Coords, SizeF Size)
{
    RectF Val(Coords.width, Coords.height, Coords.width + Size.width, Coords.height + Size.height);
    
    return Val;
}

inline RectF MakeRect(SizeF Size)
{
    RectF Val(0, 0, Size.width, Size.height);
    
    return Val;
}

namespace Color
{
    enum Value
    {
        AliceBlue = 0xF0F8FF,
        AntiqueWhite = 0xFAEBD7,
        Aqua = 0x00FFFF,
        Aquamarine = 0x7FFFD4,
        Azure = 0xF0FFFF,
        Beige = 0xF5F5DC,
        Bisque = 0xFFE4C4,
        Black = 0x000000,
        BlanchedAlmond = 0xFFEBCD,
        Blue = 0x0000FF,
        BlueViolet = 0x8A2BE2,
        Brown = 0xA52A2A,
        BurlyWood = 0xDEB887,
        CadetBlue = 0x5F9EA0,
        Chartreuse = 0x7FFF00,
        Chocolate = 0xD2691E,
        Coral = 0xFF7F50,
        CornflowerBlue = 0x6495ED,
        Cornsilk = 0xFFF8DC,
        Crimson = 0xDC143C,
        Cyan = 0x00FFFF,
        DarkBlue = 0x00008B,
        DarkCyan = 0x008B8B,
        DarkGoldenrod = 0xB8860B,
        DarkGray = 0xA9A9A9,
        DarkGreen = 0x006400,
        DarkKhaki = 0xBDB76B,
        DarkMagenta = 0x8B008B,
        DarkOliveGreen = 0x556B2F,
        DarkOrange = 0xFF8C00,
        DarkOrchid = 0x9932CC,
        DarkRed = 0x8B0000,
        DarkSalmon = 0xE9967A,
        DarkSeaGreen = 0x8FBC8F,
        DarkSlateBlue = 0x483D8B,
        DarkSlateGray = 0x2F4F4F,
        DarkTurquoise = 0x00CED1,
        DarkViolet = 0x9400D3,
        DeepPink = 0xFF1493,
        DeepSkyBlue = 0x00BFFF,
        DimGray = 0x696969,
        DodgerBlue = 0x1E90FF,
        Firebrick = 0xB22222,
        FloralWhite = 0xFFFAF0,
        ForestGreen = 0x228B22,
        Fuchsia = 0xFF00FF,
        Gainsboro = 0xDCDCDC,
        GhostWhite = 0xF8F8FF,
        Gold = 0xFFD700,
        Goldenrod = 0xDAA520,
        Gray = 0x808080,
        Green = 0x008000,
        GreenYellow = 0xADFF2F,
        Honeydew = 0xF0FFF0,
        HotPink = 0xFF69B4,
        IndianRed = 0xCD5C5C,
        Indigo = 0x4B0082,
        Ivory = 0xFFFFF0,
        Khaki = 0xF0E68C,
        Lavender = 0xE6E6FA,
        LavenderBlush = 0xFFF0F5,
        LawnGreen = 0x7CFC00,
        LemonChiffon = 0xFFFACD,
        LightBlue = 0xADD8E6,
        LightCoral = 0xF08080,
        LightCyan = 0xE0FFFF,
        LightGoldenrodYellow = 0xFAFAD2,
        LightGreen = 0x90EE90,
        LightGray = 0xD3D3D3,
        LightPink = 0xFFB6C1,
        LightSalmon = 0xFFA07A,
        LightSeaGreen = 0x20B2AA,
        LightSkyBlue = 0x87CEFA,
        LightSlateGray = 0x778899,
        LightSteelBlue = 0xB0C4DE,
        LightYellow = 0xFFFFE0,
        Lime = 0x00FF00,
        LimeGreen = 0x32CD32,
        Linen = 0xFAF0E6,
        Magenta = 0xFF00FF,
        Maroon = 0x800000,
        MediumAquamarine = 0x66CDAA,
        MediumBlue = 0x0000CD,
        MediumOrchid = 0xBA55D3,
        MediumPurple = 0x9370DB,
        MediumSeaGreen = 0x3CB371,
        MediumSlateBlue = 0x7B68EE,
        MediumSpringGreen = 0x00FA9A,
        MediumTurquoise = 0x48D1CC,
        MediumVioletRed = 0xC71585,
        MidnightBlue = 0x191970,
        MintCream = 0xF5FFFA,
        MistyRose = 0xFFE4E1,
        Moccasin = 0xFFE4B5,
        NavajoWhite = 0xFFDEAD,
        Navy = 0x000080,
        OldLace = 0xFDF5E6,
        Olive = 0x808000,
        OliveDrab = 0x6B8E23,
        Orange = 0xFFA500,
        OrangeRed = 0xFF4500,
        Orchid = 0xDA70D6,
        PaleGoldenrod = 0xEEE8AA,
        PaleGreen = 0x98FB98,
        PaleTurquoise = 0xAFEEEE,
        PaleVioletRed = 0xDB7093,
        PapayaWhip = 0xFFEFD5,
        PeachPuff = 0xFFDAB9,
        Peru = 0xCD853F,
        Pink = 0xFFC0CB,
        Plum = 0xDDA0DD,
        PowderBlue = 0xB0E0E6,
        Purple = 0x800080,
        Red = 0xFF0000,
        RosyBrown = 0xBC8F8F,
        RoyalBlue = 0x4169E1,
        SaddleBrown = 0x8B4513,
        Salmon = 0xFA8072,
        SandyBrown = 0xF4A460,
        SeaGreen = 0x2E8B57,
        SeaShell = 0xFFF5EE,
        Sienna = 0xA0522D,
        Silver = 0xC0C0C0,
        SkyBlue = 0x87CEEB,
        SlateBlue = 0x6A5ACD,
        SlateGray = 0x708090,
        Snow = 0xFFFAFA,
        SpringGreen = 0x00FF7F,
        SteelBlue = 0x4682B4,
        Tan = 0xD2B48C,
        Teal = 0x008080,
        Thistle = 0xD8BFD8,
        Tomato = 0xFF6347,
        Turquoise = 0x40E0D0,
        Violet = 0xEE82EE,
        Wheat = 0xF5DEB3,
        White = 0xFFFFFF,
        WhiteSmoke = 0xF5F5F5,
        Yellow = 0xFFFF00,
        YellowGreen = 0x9ACD32
    };
}

#ifndef ARRAYSIZE
#define ARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))
#endif