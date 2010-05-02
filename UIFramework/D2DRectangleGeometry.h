#pragma once

#include "Geometry.h"
#include "Factory.h"

#include <d2d1.h>

template< typename T >
class CD2DGeometryBase : public T
{
    public:
        virtual ID2D1Geometry* GetD2DGeometry()
        {
            return m_Geometry;
        }

        virtual HRESULT FillContainsPoint( const Point2F& LocalPoint, BOOL* pContainsPoint )
        {
            HRESULT hr = S_OK;

            IFCPTR(pContainsPoint);

            IFC(m_Geometry->FillContainsPoint(LocalPoint, D2D1::Matrix3x2F::Identity(), pContainsPoint));

        Cleanup:
            return hr;
        }

    protected:
        CD2DGeometryBase() : m_Geometry(NULL)
        {
        }

        virtual ~CD2DGeometryBase()
        {
            ReleaseObject(m_Geometry);
        }

        HRESULT Initialize( ID2D1Geometry* pGeometry )
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

class CD2DRectangleGeometry : public CD2DGeometryBase< CRefCountedObjectBase< CRectangleGeometry > >
{
    public:
        DECLARE_FACTORY1( CD2DRectangleGeometry, ID2D1RectangleGeometry* );

    protected:
        CD2DRectangleGeometry();
        virtual ~CD2DRectangleGeometry();

        HRESULT Initialize( ID2D1RectangleGeometry* pRectangleGeometry );

        ID2D1RectangleGeometry* m_RectangleGeometry;
};
