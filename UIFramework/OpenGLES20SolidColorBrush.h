#pragma once

#include "OpenGLES20Brush.h"

class COpenGLES20SolidColorBrush : public COpenGLES20Brush
{
	public:
		DECLARE_FACTORY1( COpenGLES20SolidColorBrush, const ColorF& );

        __override virtual ColorF GetDiffuseColor(
        	) const;
    
        __override virtual __out_opt ITexture* GetTexture(
            ) const;

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