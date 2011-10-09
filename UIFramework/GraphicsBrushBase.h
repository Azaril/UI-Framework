#pragma once

#include "GraphicsBrush.h"
#include "Factory.h"
#include "Texture.h"

class CGraphicsBrushBase : public CGraphicsBrush
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
        CGraphicsBrushBase(
            );
        
        virtual ~CGraphicsBrushBase(
            );
    
        Matrix3X2F m_Transform;
};