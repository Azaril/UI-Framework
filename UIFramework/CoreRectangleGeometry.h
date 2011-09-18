#pragma once

#include "Factory.h"
#include "RectangleGraphicsGeometry.h"

class CCoreRectangleGeometry : public CRefCountedObjectBase< CRectangleGraphicsGeometry >
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
    
        const RectF& GetRectangle(
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
