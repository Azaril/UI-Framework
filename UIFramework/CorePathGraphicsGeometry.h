#pragma once

#include "RefCounted.h"
#include "PathGraphicsGeometry.h"
#include "CoreShapeData.h"

class CCorePathGraphicsGeometry : public CRefCountedObjectBase< CPathGraphicsGeometry >
{
    public:
        DECLARE_FACTORY1( CCorePathGraphicsGeometry, CCoreShapeData* );

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

    protected:
        CCorePathGraphicsGeometry(
            );

        virtual ~CCorePathGraphicsGeometry(
            );

        __checkReturn HRESULT Initialize(
            __in CCoreShapeData* pShapeData
            );

        CCoreShapeData* m_pShapeData;
};

