#pragma once

#include "ReadStream.h"

struct IResourceProvider
{
    DECLARE_ADDREF_RELEASE_INTERFACE();

    virtual __checkReturn HRESULT ReadResource(
        __in_ecount(identiferLength) WCHAR* pIdentifier,
        UINT32 identifierLength,
        __deref_out IReadStream** ppStream
        ) = 0;
};