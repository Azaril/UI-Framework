#pragma once

#include "VisualContext.h"

class CVisualResource
{
    public:
        virtual INT32 AddRef() = 0;
        virtual INT32 Release() = 0;

        virtual HRESULT OnVisualAttach( CVisualAttachContext& Context ) = 0;
        virtual HRESULT OnVisualDetach( CVisualDetachContext& Context ) = 0;
};