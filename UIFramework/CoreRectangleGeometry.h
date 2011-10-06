#pragma once

#include "Factory.h"
#include "RectangleGraphicsGeometry.h"
#include "CoreGeometry.h"

class CCoreRectangleGeometry : public CCoreGeometry< CRectangleGraphicsGeometry >
{
    public:
        DECLARE_FACTORY1( CCoreRectangleGeometry, const RectF& );

        __override virtual __checkReturn HRESULT GetBounds(
			__out RectF* pBounds 
			);

        __override virtual __checkReturn HRESULT GetBounds( 
			const Matrix3X2F& Transform,
			__out RectF* pBounds 
			);

        __override virtual __checkReturn HRESULT FillContainsPoint( 
			const Point2F& LocalPoint, 
			__out BOOL* pContainsPoint 
			);
    
        __override virtual __checkReturn HRESULT TesselateFill(
            __in ITesselationSink* pSink
            );

        __override virtual __checkReturn HRESULT TesselateStroke(
            FLOAT strokeThickness,
            __in ITesselationSink* pSink
            );

    protected:
        CCoreRectangleGeometry(
			);

        virtual ~CCoreRectangleGeometry(
			);

        __checkReturn HRESULT Initialize( 
			const RectF& rect
			);

        RectF m_Rect;
};
