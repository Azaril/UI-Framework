#pragma once

#include "GraphicsBrushBase.h"

class CBitmapBrushBase : public CGraphicsBrushBase
{
    public:
        DECLARE_FACTORY1( CBitmapBrushBase, ITexture* );
    
        __override virtual ColorF GetDiffuseColor(
            ) const;
    
        __override virtual __out_opt ITexture* GetTexture(
            ) const;
    
    protected:
        CBitmapBrushBase(
            );
    
        virtual ~CBitmapBrushBase(
            );
    
        __checkReturn HRESULT Initialize(
            __in ITexture* pTexture
            );
    
        ITexture* m_pTexture;
};

