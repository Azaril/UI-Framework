#pragma once

#include "Panel.h"

class CGrid : public CPanel
{
    public:
        DECLARE_FACTORY( CGrid );

    protected:
        CGrid();
        virtual ~CGrid();

        HRESULT Initialize();
};
