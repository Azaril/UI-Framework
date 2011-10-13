#pragma once

#include "Defines.h"
#include "Point2F.h"

struct UIFRAMEWORK_API BezierSegment
{
    BezierSegment(
        const Point2F& bezierControlPoint1,
        const Point2F& bezierControlPoint2,
        const Point2F& bezierEndPoint
		)
    {
        point1 = bezierControlPoint1;
        point2 = bezierControlPoint2;
        point3 = bezierEndPoint;
    }
    
    Point2F point1;
    Point2F point2;
    Point2F point3;
};