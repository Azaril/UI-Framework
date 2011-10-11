#pragma once

#include "ResourceProvider.h"
#include "Factory.h"
#include "Collections.h"

class UIFRAMEWORK_API CCompositeResourceProvider : public CRefCountedObjectBase< IResourceProvider >
{
    public:
        DECLARE_FACTORY( CCompositeResourceProvider );

        __checkReturn HRESULT AddResourceProvider(
            __in IResourceProvider* pProvider
            );

        __override virtual __checkReturn HRESULT ReadResource(
            __in_ecount(identiferLength) const WCHAR* pIdentifier,
            UINT32 identifierLength,
            __deref_out IReadStream** ppStream
            );

    protected:
        CCompositeResourceProvider(
            );

        virtual ~CCompositeResourceProvider(
            );

        __checkReturn HRESULT Initialize(
            );

        vector< IResourceProvider* > m_ResourceProviders;
};

