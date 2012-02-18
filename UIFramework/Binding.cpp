#include "Binding.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

CBinding::CBinding(
    )
{
}

CBinding::~CBinding(
    )
{
}

__checkReturn HRESULT 
CBinding::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

    IFC(CSourcedBinding::Initialize(pProviders));

Cleanup:
    return hr;
}