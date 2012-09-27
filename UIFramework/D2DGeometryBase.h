#pragma once

#if defined(FRAMEWORK_D2D)

#include "GraphicsGeometry.h"
#include "Factory.h"

#include <d2d1.h>

#include "D2DUtilities.h"

template< typename T >
class CD2DGeometryBase : public T
{
    public:
        virtual __out ID2D1Geometry* GetD2DGeometry(
			)
        {
            return m_Geometry;
        }

        virtual __checkReturn HRESULT GetBounds(
			__out RectF* pBounds 
			)
        {
            HRESULT hr = S_OK;

            IFCPTR(pBounds);

            IFC(m_Geometry->GetBounds(NULL, RectFToD2DRectF(pBounds)));

        Cleanup:
            return hr;
        }

        virtual __checkReturn HRESULT GetBounds( 
			const Matrix3X2F& Transform, 
			__out RectF* pBounds 
			)
        {
            HRESULT hr = S_OK;

            IFCPTR(pBounds);

            IFC(m_Geometry->GetBounds(Matrix3X2FToD2DMatrix3X2F(Transform), RectFToD2DRectF(pBounds)));

        Cleanup:
            return hr;
        }

        virtual __checkReturn HRESULT FillContainsPoint(
			const Point2F& LocalPoint, 
			bool* pContainsPoint 
			)
        {
            HRESULT hr = S_OK;
            Matrix3X2F identity = Matrix3X2F::Identity();
            BOOL containsPoint = FALSE;

            IFCPTR(pContainsPoint);

            IFC(m_Geometry->FillContainsPoint(Point2FTOD2DPoint2F(LocalPoint), Matrix3X2FToD2DMatrix3X2F(&identity), &containsPoint));

            *pContainsPoint = containsPoint ? TRUE: FALSE;

        Cleanup:
            return hr;
        }

    protected:
        CD2DGeometryBase(
			) 
			: m_Geometry(NULL)
        {
        }

        virtual ~CD2DGeometryBase(
			)
        {
            ReleaseObject(m_Geometry);
        }

        __checkReturn HRESULT Initialize(
			__in ID2D1Geometry* pGeometry 
			)
        {
            HRESULT hr = S_OK;

            IFCPTR(pGeometry);

            m_Geometry = pGeometry;
            AddRefObject(m_Geometry);

        Cleanup:
            return hr;
        }

        ID2D1Geometry* m_Geometry;
};

#endif