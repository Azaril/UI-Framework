#pragma once

#include "ResourceProvider.h"
#include "Factory.h"

class UIFRAMEWORK_API CFileResourceProvider : public CRefCountedObjectBase< IResourceProvider >
{
    public:
        DECLARE_FACTORY( CFileResourceProvider );

        __override virtual __checkReturn HRESULT ReadResource(
            __in_ecount(identiferLength) const WCHAR* pIdentifier,
            UINT32 identifierLength,
            __deref_out IReadStream** ppStream
            );

    protected:
        CFileResourceProvider(
            );

        virtual ~CFileResourceProvider(
            );

        __checkReturn HRESULT Initialize(
            );
};