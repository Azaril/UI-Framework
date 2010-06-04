#pragma once

#include "DataTypes.h"
#include "Point2F.h"
#include "SizeF.h"

#ifdef _WINDOWS
#include <D2D1.h>
#include <D2D1helper.h>
#endif

struct Matrix3X2F
#ifdef _WINDOWS
    : D2D_MATRIX_3X2_F
#endif
{  
    inline Matrix3X2F operator*(const Matrix3X2F& Other) const
    {
        Matrix3X2F Result;

        Result._11 = _11 * Other._11 + _12 * Other._21;
        Result._12 = _11 * Other._12 + _12 * Other._22;
        Result._21 = _21 * Other._11 + _22 * Other._21;
        Result._22 = _21 * Other._12 + _22 * Other._22;
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
        
        Matrix3X2F Copy = *this;
        
        _11 = Copy._22 * Reciprocal;
        _12 = -Copy._12 * Reciprocal;
        _21 = -Copy._21 * Reciprocal;
        _22 = Copy._11 * Reciprocal;
        _31 = (Copy._21 * Copy._32 - Copy._31 * Copy._22) * Reciprocal;
        _32 = (Copy._32 * Copy._12 - Copy._11 * Copy._32) * Reciprocal;
        
        return TRUE;
    }
    
    static inline Matrix3X2F Identity()
    {
        Matrix3X2F Result;
        
        Result._11 = 1.0f;
        Result._12 = 0.0f;
        Result._21 = 0.0f;
        Result._22 = 1.0f;
        Result._31 = 0.0f;
        Result._32 = 0.0f;
        
        return Result;
    }
    
    static inline Matrix3X2F Translation(const SizeF& Size)
    {
        Matrix3X2F Result;
        
        Result._11 = 1.0f;
        Result._12 = 0.0f;
        Result._21 = 0.0f;
        Result._22 = 1.0f;
        Result._31 = Size.width;
        Result._32 = Size.height;
        
        return Result;
    }
    
    static inline Matrix3X2F Translation(FLOAT x, FLOAT y)
    {
        return Translation(SizeF(x, y));
    }
    
    static inline Matrix3X2F Scale(const SizeF& Size, const Point2F& Center = Point2F(0, 0))
    {
        Matrix3X2F Result;
        
        Result._11 = Size.width;
        Result._12 = 0.0f;
        Result._21 = 0.0f;
        Result._22 = Size.height;
        Result._31 = Center.x - Size.width * Center.x;
        Result._32 = Center.y - Size.height * Center.y;
        
        return Result;
    }
    
    static inline Matrix3X2F Scale(FLOAT x, FLOAT y, const Point2F& Center = Point2F(0, 0))
    {
        return Scale(SizeF(x, y), Center);
    }

#ifndef _WINDOWS
    FLOAT _11;
    FLOAT _12;
    FLOAT _21;
    FLOAT _22;
    FLOAT _31;
    FLOAT _32;
#endif
};

inline Point2F operator*(const Point2F& Point, const Matrix3X2F& Matrix)
{
    return Matrix.TransformPoint(Point);
} 