#pragma once

#include "Types.h"
#include "Collections.h"

class CMatrixStack
{
    typedef std::vector< Matrix3X2 > MatrixStack;

    public:
        CMatrixStack();
        ~CMatrixStack();

        const Matrix3X2& GetTop();

        HRESULT Push();
        HRESULT Pop();

        HRESULT LoadIdentity();
        HRESULT LoadMatrix( const Matrix3X2& Matrix );

        HRESULT MultMatrix( const Matrix3X2& Matrix );
        HRESULT MultMatrixLocal( const Matrix3X2& Matrix );

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
        Matrix3X2 m_Identity;
};
