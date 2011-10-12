#pragma once

#include "FileResourceProvider.h"

class CBundleFileResourceProvider : public CFileResourceProvider
{
    public:
        DECLARE_FACTORY( CBundleFileResourceProvider );

        __override virtual __checkReturn HRESULT ReadResource(
            __in_ecount(identiferLength) const WCHAR* pIdentifier,
            UINT32 identifierLength,
            __deref_out IReadStream** ppStream
            );        
    
    protected:
    	CBundleFileResourceProvider(
	    	);

    	virtual ~CBundleFileResourceProvider(
	    	);

    	__checkReturn HRESULT Initialize(
	    	);
};