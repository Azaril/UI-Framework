#pragma once

#include "Types.h"
#include "Factory.h"

class CTransform : public CRefCountedObject
{
    public:
        virtual HRESULT TransformPoint( const Point2F& Point, Point2F* pTransformedPoint ) = 0;
        virtual HRESULT Invert( CTransform** ppTransform ) = 0;
};

class CMatrixTransform : public CTransform
{
    public:
        DECLARE_FACTORY1( CMatrixTransform, const Matrix3X2F& );

        virtual HRESULT TransformPoint( const Point2F& Point, Point2F* pTransformedPoint );
        virtual HRESULT Invert( CTransform** ppTransform );

    protected:
        HRESULT Initialize( const Matrix3X2F& Transform );

        Matrix3X2F m_Transform;
};