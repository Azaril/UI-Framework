#pragma once

#include "Types.h"
#include "Collections.h"

class CMatrixStack
{
    typedef std::vector< Matrix3X2F > MatrixStack;

    public:
        CMatrixStack();
        ~CMatrixStack();

        const Matrix3X2F& GetTop();

        HRESULT Push();
        HRESULT Pop();

        HRESULT LoadIdentity();
        HRESULT LoadMatrix( const Matrix3X2F& Matrix );

        HRESULT MultMatrix( const Matrix3X2F& Matrix );
        HRESULT MultMatrixLocal( const Matrix3X2F& Matrix );

        HRESULT Translate( const SizeF& Translation );
        HRESULT TranslateLocal( const Point2F& Translation );

        HRESULT Rotate( const FLOAT Angle, const Point2F& Center );
        HRESULT RotateLocal( const FLOAT Angle, const Point2F& Center );

        HRESULT Scale( const SizeF& Scaling, const Point2F& Center );
        HRESULT ScaleLocal( const SizeF& Scaling, const Point2F& Center );

        HRESULT Skew( const FLOAT AngleX, const FLOAT AngleY, const Point2F& Center );
        HRESULT SkewLocal( const FLOAT AngleX, const FLOAT AngleY, const Point2F& Center );

    protected:
        MatrixStack m_MatrixStack;
        Matrix3X2F m_Identity;
};
