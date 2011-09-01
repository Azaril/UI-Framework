#pragma once

#include "BitmapSource.h"

class CImagingProvider : public CRefCountedObject
{
    public:
        virtual __checkReturn HRESULT LoadBitmapFromFile(
			__in_z const WCHAR* pPath, 
			CBitmapSource** ppBitmapSource 
			) = 0;

        virtual __checkReturn HRESULT LoadBitmapFromMemory(
			__in_bcount(DataSize) const BYTE* pData, 
			UINT32 DataSize,
			__deref_out CBitmapSource** ppBitmapSource
			) = 0;
};
