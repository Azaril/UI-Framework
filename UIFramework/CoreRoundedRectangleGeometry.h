#pragma once

#include "Factory.h"
#include "RoundedRectangleGraphicsGeometry.h"
#include "CoreGeometry.h"

class CCoreRoundedRectangleGeometry : public CCoreGeometry< CRoundedRectangleGraphicsGeometry >
{
    public:
        DECLARE_FACTORY2( CCoreRoundedRectangleGeometry, const RectF&, FLOAT );

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

    protected:
        CCoreRoundedRectangleGeometry(
			);

        virtual ~CCoreRoundedRectangleGeometry(
			);

        __checkReturn HRESULT Initialize( 
			const RectF& rect,
            FLOAT cornerRadius
			);

        RectF m_Rect;
        FLOAT m_CornerRadius;
};
