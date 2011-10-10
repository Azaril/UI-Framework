#pragma once

#include "ImagingProvider.h"
#include "Factory.h"

class CCGImagingProvider : public CImagingProvider
{
	public:
		DECLARE_FACTORY( CCGImagingProvider );

		__override virtual __checkReturn HRESULT LoadBitmapFromFile(
			__in_z const WCHAR* pPath,
			__deref_out CBitmapSource** ppBitmapSource
			);

		__override virtual __checkReturn HRESULT LoadBitmapFromMemory(
			__in_bcount(DataSize) const BYTE* pData,
			UINT32 DataSize,
			__deref_out CBitmapSource** ppBitmapSource
			);

	protected:
		CCGImagingProvider(
			);

		virtual ~CCGImagingProvider(
			);

		__checkReturn HRESULT Initialize(
			);
};