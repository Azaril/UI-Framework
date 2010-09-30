#include "PropertyObject.h"
#include "ErrorChecking.h"
#include "BasicTypes.h"
#include "LayeredValue.h"

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

    IFC(SetValuePrivate(pProperty, pValue));

Cleanup:
    ReleaseObject(pOldValue);

    return hr;
}

HRESULT CPropertyObject::SetValueReadOnly(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;
    CObjectWithType* pOldValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    IFCEXPECT(pProperty->IsReadOnly());

    IFC(SetValuePrivate(pProperty, pValue));

Cleanup:
    ReleaseObject(pOldValue);

    return hr;
}

HRESULT CPropertyObject::SetValuePrivate(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;
    CLayeredValue* pLayeredValue = NULL;
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

        IFC(RaisePropertyChanged(pProperty));
    }
    else if(SUCCEEDED(GetLayeredValue(pProperty, &pLayeredValue)))
    {
        IFC(pLayeredValue->SetLocalValue(pValue));
    }
    else
    {
        IFC(SetValueInternal(pProperty, pValue));

        IFC(RaisePropertyChanged(pProperty));
    }

Cleanup:
    ReleaseObject(pOldValue);

    return hr;
}

HRESULT CPropertyObject::SetValueInternal(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFC(E_FAIL);

Cleanup:
    return hr;
}

HRESULT CPropertyObject::GetValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;
    CLayeredValue* pLayeredValue;

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
    else if(SUCCEEDED(GetLayeredValue(pProperty, &pLayeredValue)))
    {
        IFC(pLayeredValue->GetLocalValue(ppValue));
    }
    else
    {
        IFC(GetValueInternal(pProperty, ppValue));
    }

Cleanup:
    return hr;
}

HRESULT CPropertyObject::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFC_NOTRACE(E_FAIL);

Cleanup:
    return hr;
}

HRESULT CPropertyObject::GetValueInternal(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;

    IFC(E_FAIL);

Cleanup:
    return hr;
}

HRESULT CPropertyObject::GetEffectiveValue(CProperty* pProperty, CObjectWithType** ppValue)
{
    HRESULT hr = S_OK;
    CLayeredValue* pLayeredValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(SUCCEEDED(GetLayeredValue(pProperty, &pLayeredValue)))
    {
        IFC(pLayeredValue->GetEffectiveValue(ppValue));
    }
    else
    {
        //TODO: This will trigger yet another layered property lookup, needs optomization.
        IFC(GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}

HRESULT CPropertyObject::AddPropertyChangeListener(const PropertyChangedHandler& Handler, events::signals::connection* pConnection)
{
    HRESULT hr = S_OK;

    IFCPTR(pConnection);

    *pConnection = m_PropertyChangedSignal.connect(Handler);

Cleanup:
    return hr;
}

HRESULT CPropertyObject::RaisePropertyChanged(CProperty* pProperty)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);

    m_PropertyChangedSignal(this, pProperty);

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

//
// CObjectWithType
//
extern "C" __declspec(dllexport)
void CObjectWithType_AddRef(CObjectWithType* pObject)
{
    pObject->AddRef();
}

extern "C" __declspec(dllexport)
void CObjectWithType_Release(CObjectWithType* pObject)
{
    pObject->Release();
}

extern "C" __declspec(dllexport)
TypeIndex::Value CObjectWithType_TypeIndex()
{
    return TypeIndex::Object;
}

extern "C" __declspec(dllexport)
TypeIndex::Value CObjectWithType_GetType(CObjectWithType* pObject)
{
    return pObject->GetType();
}

//
// CProperty
//
extern "C" __declspec(dllexport)
void CProperty_AddRef(CProperty* pProperty)
{
    pProperty->AddRef();
}

extern "C" __declspec(dllexport)
void CProperty_Release(CProperty* pProperty)
{
    pProperty->Release();
}

//
// CPropertyObject
//
extern "C" __declspec(dllexport)
TypeIndex::Value CPropertyObject_TypeIndex()
{
    return TypeIndex::PropertyObject;
}

extern "C" __declspec(dllexport)
CObjectWithType* CPropertyObject_CastTo_CObjectWithType(CPropertyObject* pPropertyObject)
{
    return pPropertyObject;
}

extern "C" __declspec(dllexport)
CPropertyObject* CObjectWithType_CastTo_CPropertyObject(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::PropertyObject)) ? (CPropertyObject*)pObject : NULL;
}

extern "C" __declspec(dllexport)
HRESULT CPropertyObject_SetValue(CPropertyObject* pPropertyObject, CProperty* pProperty, CObjectWithType* pValue)
{
    return pPropertyObject->SetValue(pProperty, pValue);
}

extern "C" __declspec(dllexport)
HRESULT CPropertyObject_SetValueFloat(CPropertyObject* pPropertyObject, CProperty* pProperty, FLOAT Value)
{
    HRESULT hr = S_OK;
    CFloatValue* pValue = NULL;

    IFC(CFloatValue::Create(Value, &pValue));

    IFC(pPropertyObject->SetValue(pProperty, pValue));

Cleanup:
    ReleaseObject(pValue);

    return hr;
}

extern "C" __declspec(dllexport)
HRESULT CPropertyObject_SetValueString(CPropertyObject* pPropertyObject, CProperty* pProperty, const WCHAR* strValue)
{
    HRESULT hr = S_OK;
    CStringValue* pValue = NULL;

    IFC(CStringValue::Create(strValue, &pValue));

    IFC(pPropertyObject->SetValue(pProperty, pValue));

Cleanup:
    ReleaseObject(pValue);

    return hr;
}

extern "C" __declspec(dllexport)
HRESULT CPropertyObject_SetValueVisibility(CPropertyObject* pPropertyObject, CProperty* pProperty, const Visibility::Value Val)
{
    HRESULT hr = S_OK;
    CVisibilityValue* pValue = NULL;

    IFC(CVisibilityValue::Create(Val, &pValue));

    IFC(pPropertyObject->SetValue(pProperty, pValue));

Cleanup:
    ReleaseObject(pValue);

    return hr;
}

extern "C" __declspec(dllexport)
HRESULT CPropertyObject_GetValue(CPropertyObject* pPropertyObject, CProperty* pProperty, CObjectWithType** ppValue)
{
    return pPropertyObject->GetValue(pProperty, ppValue);
}