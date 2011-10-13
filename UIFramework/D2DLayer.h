#pragma once

#include "Layer.h"
#include "Factory.h"

#include <d2d1.h>

class CD2DLayer : public CLayer
{
    public:
        DECLARE_FACTORY1( CD2DLayer, ID2D1Layer* );

        virtual __out ID2D1Layer* GetLayer(
			);

    protected:
        CD2DLayer(
			);

        virtual ~CD2DLayer(
			);

        __checkReturn HRESULT Initialize(
			__in ID2D1Layer* pLayer 
			);

        ID2D1Layer* m_Layer;
};
