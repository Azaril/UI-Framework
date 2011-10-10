#pragma once

#include "Types.h"
#include "Collections.h"

class CMatrixStack
{
    typedef vector< Matrix3X2F > MatrixStack;

    public:
        CMatrixStack(
			);

        ~CMatrixStack(
			);

        const Matrix3X2F& GetTop();

        __checkReturn HRESULT Push(
			);

        __checkReturn HRESULT Pop(
			);

        __checkReturn HRESULT LoadIdentity(
			);

        __checkReturn HRESULT LoadMatrix(
			const Matrix3X2F& Matrix 
			);

        __checkReturn HRESULT MultMatrix( 
			const Matrix3X2F& Matrix 
			);

        __checkReturn HRESULT MultMatrixLocal(
			const Matrix3X2F& Matrix 
			);

        __checkReturn HRESULT Translate(
			const SizeF& Translation 
			);

        __checkReturn HRESULT TranslateLocal( 
			const Point2F& Translation 
			);

        __checkReturn HRESULT Rotate( 
			const FLOAT Angle, 
			const Point2F& Center 
			);

        __checkReturn HRESULT RotateLocal(
			const FLOAT Angle,
			const Point2F& Center 
			);

        __checkReturn HRESULT Scale( 
			const SizeF& Scaling, 
			const Point2F& Center 
			);

        __checkReturn HRESULT ScaleLocal( 
			const SizeF& Scaling, 
			const Point2F& Center
			);

        __checkReturn HRESULT Skew( 
			const FLOAT AngleX,
			const FLOAT AngleY,
			const Point2F& Center
			);

        __checkReturn HRESULT SkewLocal(
			const FLOAT AngleX,
			const FLOAT AngleY,
			const Point2F& Center
			);

    protected:
        MatrixStack m_MatrixStack;
        Matrix3X2F m_Identity;
};
