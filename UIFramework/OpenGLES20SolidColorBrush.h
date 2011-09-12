#pragma once

#include "GraphicsBrush.h"
#include "Factory.h"

class COpenGLES20SolidColorBrush : public CGraphicsBrush
{
	public:
		DECLARE_FACTORY1( COpenGLES20SolidColorBrush, const ColorF& );
    
        __override virtual __checkReturn HRESULT SetTransform(
            const Matrix3X2F& Transform
            );

	protected:
		COpenGLES20SolidColorBrush(
			);
		
		virtual ~COpenGLES20SolidColorBrush(
			);

		__checkReturn HRESULT Initialize(
			const ColorF& Color
			);
			
		ColorF m_Color;
};