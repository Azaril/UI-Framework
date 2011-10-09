#pragma once

#include "GraphicsBrushBase.h"
#include "Texture.h"

class CLinearGradientBrushBase : public CGraphicsBrushBase
{
    public:
        DECLARE_FACTORY2( CLinearGradientBrushBase, ITexture*, const Matrix3X2F& );
    
        __override virtual ColorF GetDiffuseColor(
            ) const;
    
        __override virtual __out_opt ITexture* GetTexture(
            ) const;
    
        __override virtual void GetFinalTransform(
            Matrix3X2F& Transform
            ) const;
    
    protected:
        CLinearGradientBrushBase(
            );
    
        virtual ~CLinearGradientBrushBase(
            );
    
        __checkReturn HRESULT Initialize(
            __in ITexture* pTexture,
            const Matrix3X2F& textureToBrushTransform
            );
    
        ITexture* m_pTexture;
        Matrix3X2F m_TextureToBrushTransform;
};