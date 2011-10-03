#pragma once

#include "GraphicsBrush.h"
#include "Factory.h"
#include "Texture.h"

class COpenGLES20Brush : public CGraphicsBrush
{
    public:     
        __override virtual __checkReturn HRESULT SetTransform(
            const Matrix3X2F& Transform
            );
        
        __override virtual void GetTransform(
            Matrix3X2F& Transform
            ) const;
    
        virtual void GetFinalTransform(
            Matrix3X2F& Transform
            ) const;
        
        virtual ColorF GetDiffuseColor(
            ) const = 0;
    
        virtual __out_opt ITexture* GetTexture(
            ) const = 0;
        
    protected:
        COpenGLES20Brush(
            );
        
        virtual ~COpenGLES20Brush(
            );
    
        Matrix3X2F m_Transform;
};