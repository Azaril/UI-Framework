#pragma once

#include "OpenGLES20Brush.h"
#include "Texture.h"

class COpenGLES20LinearGradientBrush : public COpenGLES20Brush
{
    public:
        DECLARE_FACTORY2( COpenGLES20LinearGradientBrush, ITexture*, const Matrix3X2F& );
    
        __override virtual ColorF GetDiffuseColor(
            ) const;
    
        __override virtual __out_opt ITexture* GetTexture(
            ) const;
    
        __override virtual void GetFinalTransform(
            Matrix3X2F& Transform
            ) const;
    
    protected:
        COpenGLES20LinearGradientBrush(
            );
    
        virtual ~COpenGLES20LinearGradientBrush(
            );
    
        __checkReturn HRESULT Initialize(
            __in ITexture* pTexture,
            const Matrix3X2F& textureToBrushTransform
            );
    
        ITexture* m_pTexture;
        Matrix3X2F m_TextureToBrushTransform;
};