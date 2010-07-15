#pragma once

#include "Layer.h"
#include "Factory.h"

class CD2DLayer : public CLayer
{
    public:
        DECLARE_FACTORY1( CD2DLayer, ID2D1Layer* );

        virtual ID2D1Layer* GetLayer();

    protected:
        CD2DLayer();
        virtual ~CD2DLayer();

        HRESULT Initialize( ID2D1Layer* pLayer );

        ID2D1Layer* m_Layer;
};
