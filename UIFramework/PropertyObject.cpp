#include "PropertyObject.h"
#include "ErrorChecking.h"

CPropertyObject::CPropertyObject() : m_PropertyInformation(NULL)
{
}

CPropertyObject::~CPropertyObject()
{
    ReleaseObject(m_PropertyInformation);
}

HRESULT CPropertyObject::GetPropertyInformation(CPropertyInformation** ppInformation)
{
    HRESULT hr = S_OK;

    IFCPTR(ppInformation);

    if(m_PropertyInformation == NULL)
    {
        IFC(CreatePropertyInformation(&m_PropertyInformation));
    }

    *ppInformation = m_PropertyInformation;
    AddRefObject(m_PropertyInformation);

Cleanup:
    return hr;
}
