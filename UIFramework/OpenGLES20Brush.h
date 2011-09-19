#pragma once

#include "GraphicsBrush.h"
#include "Factory.h"

class COpenGLES20Brush : public CGraphicsBrush
{
    public:     
        virtual const ColorF& GetDiffuseColor(
            ) = 0;
};