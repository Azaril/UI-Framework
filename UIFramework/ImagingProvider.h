#pragma once

#include "BitmapSource.h"

class CImagingProvider : public CRefCountedObject
{
    public:
        virtual HRESULT LoadBitmapFromFile( const WCHAR* pPath, CBitmapSource** ppBitmapSource ) = 0;
};
