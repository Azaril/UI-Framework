#pragma once

#if defined(FRAMEWORK_D2D)

#include <d2d1.h>

#include "DataTypes.h"
#include "GeometrySink.h"

inline D2D1_FIGURE_BEGIN 
BeginFigureOptionsToD2DOptions(
BeginFigureOptions::Value Options
    )
{
    if ((Options & BeginFigureOptions::Hollow) == BeginFigureOptions::Hollow)
    {
        return D2D1_FIGURE_BEGIN_HOLLOW;
    }
    else
    {
        return D2D1_FIGURE_BEGIN_FILLED;
    }
}

inline D2D1_FIGURE_END 
EndFigureOptionsToD2DOptions(
    EndFigureOptions::Value Options
    )
{
    if ((Options & EndFigureOptions::Closed) == EndFigureOptions::Closed)
    {
        return D2D1_FIGURE_END_CLOSED;
    }
    else
    {
        return D2D1_FIGURE_END_OPEN;
    }
}

inline D2D1_FILL_MODE 
FillModeToD2DFillMode(
    FillMode::Value fillMode
    )
{
    switch(fillMode)
    {
        case FillMode::Winding:
            {
                return D2D1_FILL_MODE_WINDING;
            }

        case FillMode::Alternate:
        default:
            {
                return D2D1_FILL_MODE_ALTERNATE;
            }
    }
}

//
// Point2F
//
inline const D2D1_POINT_2F*
Point2FTOD2DPoint2F(
    __in_opt const Point2F* pPoint
    )
{
    return (const D2D1_POINT_2F*)pPoint;
}

inline D2D1_POINT_2F*
Point2FTOD2DPoint2F(
    __in_opt Point2F* pPoint
    )
{
    return (D2D1_POINT_2F*)pPoint;
}

inline const D2D1_POINT_2F&
Point2FTOD2DPoint2F(
    const Point2F& point
    )
{
    return *(const D2D1_POINT_2F*)&point;
}

inline D2D1_POINT_2F&
Point2FTOD2DPoint2F(
    Point2F& point
    )
{
    return *(D2D1_POINT_2F*)&point;
}

//
// Bezier Segment
//
inline const D2D1_BEZIER_SEGMENT*
BezierSegmentToD2DBezierSegment(
    __in_opt const BezierSegment* pSegment
    )
{
    return (const D2D1_BEZIER_SEGMENT*)pSegment;
}

inline D2D1_BEZIER_SEGMENT*
BezierSegmentToD2DBezierSegment(
    __in_opt BezierSegment* pSegment
    )
{
    return (D2D1_BEZIER_SEGMENT*)pSegment;
}

//
// Matrix3X2F
//
inline const D2D1_MATRIX_3X2_F*
Matrix3X2FToD2DMatrix3X2F(
    __in_opt const Matrix3X2F* pMatrix
    )
{
    return (const D2D1_MATRIX_3X2_F*)pMatrix;
}

inline D2D1_MATRIX_3X2_F*
Matrix3X2FToD2DMatrix3X2F(
    __in_opt Matrix3X2F* pMatrix
    )
{
    return (D2D1_MATRIX_3X2_F*)pMatrix;
}

inline const D2D1_MATRIX_3X2_F&
    Matrix3X2FToD2DMatrix3X2F(
    const Matrix3X2F& matrix
    )
{
    return *(const D2D1_MATRIX_3X2_F*)&matrix;
}

inline D2D1_MATRIX_3X2_F&
Matrix3X2FToD2DMatrix3X2F(
    Matrix3X2F& matrix
    )
{
    return *(D2D1_MATRIX_3X2_F*)&matrix;
}

//
// RectF
//
inline const D2D1_RECT_F*
RectFToD2DRectF(
    __in_opt const RectF* pRect
    )
{
    return (const D2D1_RECT_F*)pRect;
}

inline D2D1_RECT_F*
RectFToD2DRectF(
    __in_opt RectF* pRect
    )
{
    return (D2D1_RECT_F*)pRect;
}

inline const D2D1_RECT_F&
RectFToD2DRectF(
    const RectF& rect
    )
{
    return *(const D2D1_RECT_F*)&rect;
}

inline D2D1_RECT_F&
RectFToD2DRectF(
    RectF& rect
    )
{
    return *(D2D1_RECT_F*)&rect;
}

//
// ColorF
//
inline const D2D1_COLOR_F*
ColorFToD2DColorF(
    __in_opt const ColorF* pColor
    )
{
    return (const D2D1_COLOR_F*)pColor;
}

inline D2D1_COLOR_F*
ColorFToD2DColorF(
    __in_opt ColorF* pColor
    )
{
    return (D2D1_COLOR_F*)pColor;
}

inline const D2D1_COLOR_F&
ColorFToD2DColorF(
    const ColorF& color
    )
{
    return *(const D2D1_COLOR_F*)&color;
}

inline D2D1_COLOR_F&
ColorFToD2DColorF(
    ColorF& color
    )
{
    return *(D2D1_COLOR_F*)&color;
}

//
// GradientStop
//
inline const D2D1_GRADIENT_STOP*
    GradientStopToD2DGradientStop(
    __in_opt const GradientStop* pGradientStop
    )
{
    return (const D2D1_GRADIENT_STOP*)pGradientStop;
}

inline D2D1_GRADIENT_STOP*
    GradientStopToD2DGradientStop(
    __in_opt GradientStop* pGradientStop
    )
{
    return (D2D1_GRADIENT_STOP*)pGradientStop;
}

inline const D2D1_GRADIENT_STOP&
    GradientStopToD2DGradientStop(
    const GradientStop& gradientStop
    )
{
    return *(const D2D1_GRADIENT_STOP*)&gradientStop;
}

inline D2D1_GRADIENT_STOP&
    GradientStopToD2DGradientStop(
    GradientStop& gradientStop
    )
{
    return *(D2D1_GRADIENT_STOP*)&gradientStop;
}

#endif