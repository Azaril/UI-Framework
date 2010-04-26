#pragma once

#include "RefCounted.h"

class CBitmapSource : public CRefCountedObject
{
    public:
        virtual HRESULT GetSize( SizeU* pSize ) = 0;
};
