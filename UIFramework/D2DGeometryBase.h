#pragma once

#include "GraphicsGeometry.h"
#include "Factory.h"

#include <d2d1.h>

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

            IFC(m_Geometry->GetBounds(NULL, pBounds));

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

            IFC(m_Geometry->GetBounds(&Transform, pBounds));

        Cleanup:
            return hr;
        }

        virtual __checkReturn HRESULT FillContainsPoint(
			const Point2F& LocalPoint, 
			BOOL* pContainsPoint 
			)
        {
            HRESULT hr = S_OK;

            IFCPTR(pContainsPoint);

            IFC(m_Geometry->FillContainsPoint(LocalPoint, Matrix3X2F::Identity(), pContainsPoint));

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