#pragma once

#include "GraphicsBrushBase.h"

class CSolidColorBrushBase : public CGraphicsBrushBase
{
	public:
		DECLARE_FACTORY1( CSolidColorBrushBase, const ColorF& );

        __override virtual ColorF GetDiffuseColor(
        	) const;
    
        __override virtual __out_opt ITexture* GetTexture(
            ) const;

	protected:
		CSolidColorBrushBase(
			);
		
		virtual ~CSolidColorBrushBase(
			);

		__checkReturn HRESULT Initialize(
			const ColorF& Color
			);
			
		ColorF m_Color;
};