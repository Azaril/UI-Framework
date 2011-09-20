#pragma once

#include "GraphicsBrush.h"
#include "Factory.h"

class COpenGLES20Brush : public CGraphicsBrush
{
    public:     
        __override virtual __checkReturn HRESULT SetTransform(
            const Matrix3X2F& Transform
            );
        
        __override virtual const Matrix3X2F& GetTransform(
            ) const;
        
        virtual const ColorF& GetDiffuseColor(
            ) = 0;
        
    protected:
        COpenGLES20Brush(
            );
        
        virtual ~COpenGLES20Brush(
            );
    
        Matrix3X2F m_Transform;
};