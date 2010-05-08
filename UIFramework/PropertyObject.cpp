#include "PropertyObject.h"
#include "ErrorChecking.h"

CPropertyObject::CPropertyObject()
{
}

CPropertyObject::~CPropertyObject()
{
}

HRESULT CPropertyObject::SetValue(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;
    CObjectWithType* pOldValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    IFCEXPECT(!pProperty->IsReadOnly());

    IFC(SetValueInternal(pProperty, pValue));

Cleanup:
    ReleaseObject(pOldValue);

    return hr;
}

HRESULT CPropertyObject::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;
    CObjectWithType* pOldValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(pProperty->IsAttached())
    {
        BOOL SetVal = FALSE;

        for(std::vector< CAttachedPropertyHolder >::iterator It = m_AttachedProperties.begin(); It != m_AttachedProperties.end(); ++It)
        {
            if(It->GetProperty() == pProperty)
            {
                IFC(It->GetValue(&pOldValue));

                IFC(It->SetValue(pValue));

                SetVal = TRUE;

                break;
            }
        }

        if(!SetVal)
        {
            m_AttachedProperties.push_back(CAttachedPropertyHolder(pProperty, pValue));
        }

        IFC(pProperty->OnValueChanged(this, pOldValue, pValue));
    }
    else
    {
        IFC(E_FAIL);
    }

Cleanup:
    ReleaseObject(pOldValue);

    return hr;
}

HRESULT CPropertyObject::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(pProperty->IsAttached())
    {
        for(std::vector< CAttachedPropertyHolder >::iterator It = m_AttachedProperties.begin(); It != m_AttachedProperties.end(); ++It)
        {
            if(It->GetProperty() == pProperty)
            {
                IFC(It->GetValue(ppValue));
                
                goto Cleanup;
            }
        }

        *ppValue = NULL;
    }
    else
    {
        IFC(E_FAIL);
    }

Cleanup:
    return hr;
}


CAttachedPropertyHolder::CAttachedPropertyHolder(const CAttachedPropertyHolder& Other) : m_Property(Other.m_Property),
                                                                                         m_Value(Other.m_Value)
{
    AddRefObject(m_Property);
    AddRefObject(m_Value);
}

CAttachedPropertyHolder::CAttachedPropertyHolder(CProperty* pProperty, CObjectWithType* pValue) : m_Property(pProperty),
                                                                                                  m_Value(pValue)
{
    AddRefObject(m_Property);
    AddRefObject(m_Value);
}

CAttachedPropertyHolder::~CAttachedPropertyHolder()
{
    ReleaseObject(m_Property);
    ReleaseObject(m_Value);
}

HRESULT CAttachedPropertyHolder::SetValue(CObjectWithType* pObject)
{
    HRESULT hr = S_OK;

    IFCPTR(pObject);

    ReleaseObject(m_Value);

    m_Value = pObject;

    AddRefObject(m_Value);

Cleanup:
    return hr;
}

HRESULT CAttachedPropertyHolder::GetValue(CObjectWithType** ppObject)
{
    HRESULT hr = S_OK;

    IFCPTR(ppObject);

    *ppObject = m_Value;
    AddRefObject(m_Value);

Cleanup:
    return hr;
}

CProperty* CAttachedPropertyHolder::GetProperty()
{
    return m_Property;
}