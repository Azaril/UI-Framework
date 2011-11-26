#pragma once

#include "Defines.h"
#include "DataTypes.h"
#include "Point2F.h"
#include "SizeF.h"
#include "RectF.h"

#include <math.h>

struct UIFRAMEWORK_API Matrix3X2F
{  
    inline Matrix3X2F operator*(
		const Matrix3X2F& Other
		) const
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
    
    inline Point2F TransformPoint(
		const Point2F& Point
		) const
    {
        return Point2F(Point.x * _11 + Point.y * _21 + _31, Point.x * _12 + Point.y * _22 + _32);
    }

    inline FLOAT Determinant(
        )
    {
        return (_11 * _22) - (_12 * _21);
    }
    
    inline bool IsInvertible(
        )
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
    
    inline bool Invert(
        )
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
    
    static inline Matrix3X2F Identity(
        )
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
    
    static inline Matrix3X2F Translation(
        const SizeF& Size
        )
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
    
    static inline Matrix3X2F Translation(
        FLOAT x, 
        FLOAT y
        )
    {
        return Translation(SizeF(x, y));
    }
    
    static inline Matrix3X2F Scale(
        const SizeF& Size, 
        const Point2F& Center = Point2F(0, 0)
        )
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
    
    static inline Matrix3X2F Scale(
        FLOAT x, 
        FLOAT y, 
        const Point2F& Center = Point2F(0, 0)
        )
    {
        return Scale(SizeF(x, y), Center);
    }
    
    static inline Matrix3X2F Rotation(
        FLOAT Angle,
        const Point2F& Center = Point2F(0, 0)
        )
    {
        FLOAT sinVal;
        FLOAT cosVal;
        Matrix3X2F Result;
        FLOAT normalizedAngle = fmod(Angle, 360.0f);
        
        if (normalizedAngle == 0.0f)
        {
            sinVal = 0.0f;
            cosVal = 1.0f;
        }
        else if(normalizedAngle == 90.0f)
        {
            sinVal = 1.0f;
            cosVal = 0.0f;
        }
        else if(normalizedAngle == 180.0f)
        {
            sinVal = 0.0f;
            cosVal = -1.0f;
        }
        else if(normalizedAngle == 270.0f)
        {
            sinVal = -1.0f;
            cosVal = 0.0f;
        }
        else
        {
            FLOAT normalizedRadians = (normalizedAngle * (3.1415927f / 180.0f));
            
            sinVal = sin(normalizedRadians);
            cosVal = cos(normalizedRadians);
        }
        
        Result._11 = cosVal;
        Result._12 = sinVal;
        Result._21 = -sinVal;
        Result._22 = cosVal;
        Result._31 = (Center.x * (1.0f - cosVal)) + (Center.y * sinVal);
        Result._32 = (Center.y * (1.0f - cosVal)) + (Center.x * sinVal);
        
        return Result;
    }
    
    static inline Matrix3X2F InferAffineMatrix(
        const Point2F& point1,
        const Point2F& point2,
        const Point2F& point3,
        const RectF& source
        )
    {
        Matrix3X2F Result = Matrix3X2F::Identity();
        FLOAT x0, y0, x1, y1, x2, y2;
        FLOAT u0, v0, u1, v1, u2, v2;
        FLOAT d;
        
        x0 = point1.x;
        y0 = point2.y;  

        x1 = point2.x;
        y1 = point2.y;

        x2 = point3.x;
        y2 = point3.y;

        u0 = source.left;
        v0 = source.top;

        u1 = u0 + source.GetWidth();
        v1 = v0;

        u2 = u0;
        v2 = v0 + source.GetHeight();

        d = u0 * (v1 - v2) - v0 * (u1 - u2) + (u1 * v2 - u2 * v1);

        d = 1.0f / d;

        FLOAT t0, t1, t2;

        t0 = v1 - v2;
        t1 = v2 - v0;
        t2 = v0 - v1;

        Result._11 = d * (x0 * t0 + x1 * t1 + x2 * t2);
        Result._12 = d * (y0 * t0 + y1 * t1 + y2 * t2);

        t0 = u2 - u1;
        t1 = u0 - u2;
        t2 = u1 - u0;

        Result._21 = d * (x0 * t0 + x1 * t1 + x2 * t2);
        Result._22 = d * (y0 * t0 + y1 * t1 + y2 * t2);

        t0 = u1 * v2 - u2 * v1;
        t1 = u2 * v0 - u0 * v2;
        t2 = u2 * v1 - u1 * v0;

        Result._31 = d * (x0 * t0 + x1 * t1 + x2 * t2);
        Result._32 = d * (y0 * t0 + y1 * t1 + y2 * t2);

        return Result;
    }

    FLOAT _11;
    FLOAT _12;
    FLOAT _21;
    FLOAT _22;
    FLOAT _31;
    FLOAT _32;
};

inline Point2F
operator*(
    const Point2F& Point, 
    const Matrix3X2F& Matrix
    )
{
    return Matrix.TransformPoint(Point);
} 