#pragma once

#include "BitmapSource.h"
#include "ReadStream.h"

class CImagingProvider : public CRefCountedObject
{
    public:
        virtual __checkReturn HRESULT LoadBitmapFromStream(
            __in IReadStream* pStream,
            __deref_out CBitmapSource** ppBitmapSource
            ) = 0;
};
