#include "Brush.h"
#include "StaticPropertyInformation.h"
#include "BasicTypes.h"

CBrush::CBrush(
    ) 
    : m_PropertyInformation(NULL)
{
}

CBrush::~CBrush(
    )
{
    ReleaseObject(m_PropertyInformation);
}

__override __checkReturn HRESULT 
CBrush::OnVisualAttach(
    CVisualAttachContext& Context
    )
{
    HRESULT hr = S_OK;

    IFC(CVisualResource::OnVisualAttach(Context));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CBrush::OnVisualDetach(
    CVisualDetachContext& Context
    )
{
    HRESULT hr = S_OK;

    IFC(CVisualResource::OnVisualDetach(Context));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CBrush::CreatePropertyInformation(
    __deref_out CPropertyInformation** ppInformation
    )
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;

    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(NULL, 0, &pStaticInformation));

    *ppInformation = pStaticInformation;
    pStaticInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);

    return hr;
}