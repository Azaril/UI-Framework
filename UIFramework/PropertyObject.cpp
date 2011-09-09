#include "PropertyObject.h"
#include "ErrorChecking.h"
#include "BasicTypes.h"
#include "LayeredValue.h"
#include "BindingContext.h"

__checkReturn HRESULT
CProperty::OnValueChanged( 
    __in CPropertyObject* pObjectInstance, 
    __in_opt CObjectWithType* pOldValue, 
    __in_opt CObjectWithType* pNewValue 
    )
{
    HRESULT hr = S_OK;
    OnValueChangeFunc Func = GetOnValueChangedCallback();

    if (Func != NULL)
    {
        IFC(Func(pObjectInstance, pOldValue, pNewValue));
    }

Cleanup:
    return hr;
}

CPropertyObject::CPropertyObject(
    ) 
    : m_BindingContext(NULL)
{
}

CPropertyObject::~CPropertyObject(
    )
{
    ReleaseObject(m_BindingContext);
}

__checkReturn HRESULT
CPropertyObject::SetValue(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
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

__checkReturn HRESULT 
CPropertyObject::SetValueReadOnly(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;
    CObjectWithType* pOldValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    IFC(SetValuePrivate(pProperty, pValue));

Cleanup:
    ReleaseObject(pOldValue);

    return hr;
}

__checkReturn HRESULT
CPropertyObject::SetValuePrivate(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;
    CLayeredValue* pLayeredValue = NULL;
    CObjectWithType* pOldValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    IFC(GetValue(pProperty, &pOldValue));

    if(!pOldValue || !pValue->Equals(pOldValue))
    {
        if(pProperty->IsAttached())
        {
            BOOL SetVal = FALSE;

            for(std::vector< CAttachedPropertyHolder >::iterator It = m_AttachedProperties.begin(); It != m_AttachedProperties.end(); ++It)
            {
                if(It->GetProperty() == pProperty)
                {
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
        else if(SUCCEEDED(GetLayeredValue(pProperty, &pLayeredValue)) && pLayeredValue != NULL)
        {
            IFC(pLayeredValue->SetLocalValue(pValue));
        }
        else
        {
            IFC(SetValueInternal(pProperty, pValue));

            IFC(pProperty->OnValueChanged(this, pOldValue, pValue));

            IFC(RaisePropertyChanged(pProperty));
        }
    }

Cleanup:
    ReleaseObject(pOldValue);

    return hr;
}

__checkReturn HRESULT
CPropertyObject::SetValueInternal(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;

    IFC(E_FAIL);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CPropertyObject::GetValue(
    __in CProperty* pProperty,
    __deref_out_opt CObjectWithType** ppValue
    )
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
    else if(SUCCEEDED(GetLayeredValue(pProperty, &pLayeredValue)) && pLayeredValue != NULL)
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

__checkReturn HRESULT 
CPropertyObject::GetLayeredValue(
    __in CProperty* pProperty, 
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFC_NOTRACE(E_FAIL);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CPropertyObject::GetValueInternal(
    __in CProperty* pProperty,
    __deref_out_opt CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;

    IFC(E_FAIL);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CPropertyObject::GetEffectiveValue(
    __in CProperty* pProperty, 
    __deref_out_opt CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;
    CLayeredValue* pLayeredValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if(SUCCEEDED(GetLayeredValue(pProperty, &pLayeredValue)) && pLayeredValue != NULL)
    {
        IFC(pLayeredValue->GetEffectiveValue(ppValue));
    }
    else
    {
        //TODO: This will trigger yet another layered property lookup, needs optimization.
        IFC(GetValue(pProperty, ppValue));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CPropertyObject::SetEffectiveValue(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;
    CLayeredValue* pLayeredValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    if(SUCCEEDED(GetLayeredValue(pProperty, &pLayeredValue)) && pLayeredValue != NULL)
    {
        IFC(pLayeredValue->SetEffectiveValue(pValue));
    }
    else
    {
        //TODO: This will trigger yet another layered property lookup, needs optimization.
        IFC(SetValue(pProperty, pValue));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT
CPropertyObject::AddPropertyChangeListener(
    const PropertyChangedHandler& Handler, 
    __out events::signals::connection* pConnection
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pConnection);

    *pConnection = m_PropertyChangedSignal.connect(Handler);

Cleanup:
    return hr;
}

__checkReturn HRESULT
CPropertyObject::RaisePropertyChanged(
    __in CProperty* pProperty
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);

    m_PropertyChangedSignal(this, pProperty);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CPropertyObject::SetBinding(
    __in CProperty* pProperty, 
    __in CBindingBase* pBinding
    )
{
    HRESULT hr = S_OK;

    //TODO: Provide access to binding manager?
    IFC(E_NOTIMPL);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CPropertyObject::SetBindingContext(
    __in CBindingContext* pContext
    )
{
    HRESULT hr = S_OK;

    ReleaseObject(m_BindingContext);

    m_BindingContext = pContext;

    AddRefObject(m_BindingContext);

    return hr;
}

__checkReturn HRESULT 
CPropertyObject::GetBindingContext(
    __deref_out_opt CBindingContext** ppContext
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppContext);

    *ppContext = m_BindingContext;
    AddRefObject(m_BindingContext);

Cleanup:
    return hr;
}

CAttachedPropertyHolder::CAttachedPropertyHolder(
    const CAttachedPropertyHolder& Other
    ) 
    : m_Property(Other.m_Property)
    , m_Value(Other.m_Value)
{
    AddRefObject(m_Property);
    AddRefObject(m_Value);
}

CAttachedPropertyHolder::CAttachedPropertyHolder(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
    : m_Property(pProperty)
    , m_Value(pValue)
{
    AddRefObject(m_Property);
    AddRefObject(m_Value);
}

CAttachedPropertyHolder::~CAttachedPropertyHolder(
    )
{
    ReleaseObject(m_Property);
    ReleaseObject(m_Value);
}

__checkReturn HRESULT 
CAttachedPropertyHolder::SetValue(
    __in CObjectWithType* pObject
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pObject);

    ReleaseObject(m_Value);

    m_Value = pObject;

    AddRefObject(m_Value);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CAttachedPropertyHolder::GetValue(
    __deref_out CObjectWithType** ppObject
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppObject);

    *ppObject = m_Value;
    AddRefObject(m_Value);

Cleanup:
    return hr;
}

__out CProperty* 
CAttachedPropertyHolder::GetProperty(
    )
{
    return m_Property;
}

//
// CObjectWithType
//
extern "C" UIFRAMEWORK_API
void 
CObjectWithType_AddRef(
    __in CObjectWithType* pObject
    )
{
    pObject->AddRef();
}

extern "C" UIFRAMEWORK_API
void
CObjectWithType_Release(
    __in CObjectWithType* pObject
    )
{
    pObject->Release();
}

extern "C" UIFRAMEWORK_API
TypeIndex::Value 
CObjectWithType_TypeIndex(
    )
{
    return TypeIndex::Object;
}

extern "C" UIFRAMEWORK_API
TypeIndex::Value
CObjectWithType_GetType(
    __in CObjectWithType* pObject
    )
{
    return pObject->GetType();
}

//
// CProperty
//
extern "C" UIFRAMEWORK_API
void 
CProperty_AddRef(
    __in CProperty* pProperty
    )
{
    pProperty->AddRef();
}

extern "C" UIFRAMEWORK_API
void
CProperty_Release(
    __in CProperty* pProperty
    )
{
    pProperty->Release();
}

//
// CPropertyObject
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value 
CPropertyObject_TypeIndex(
    )
{
    return TypeIndex::PropertyObject;
}

extern "C" UIFRAMEWORK_API
__out CObjectWithType* 
CPropertyObject_CastTo_CObjectWithType(
    __in CPropertyObject* pPropertyObject
    )
{
    return pPropertyObject;
}

extern "C" UIFRAMEWORK_API
__out_opt CPropertyObject* 
CObjectWithType_CastTo_CPropertyObject(
    __in CObjectWithType* pObject
    )
{
    return (pObject->IsTypeOf(TypeIndex::PropertyObject)) ? (CPropertyObject*)pObject : NULL;
}

extern "C" UIFRAMEWORK_API
__checkReturn HRESULT 
CPropertyObject_SetValue(
    __in CPropertyObject* pPropertyObject, 
    __in CProperty* pProperty,
    __in CObjectWithType* pValue
    )
{
    return pPropertyObject->SetValue(pProperty, pValue);
}

extern "C" UIFRAMEWORK_API
__checkReturn HRESULT 
CPropertyObject_SetValueFloat(
    __in CPropertyObject* pPropertyObject, 
    __in CProperty* pProperty, 
    FLOAT Value
    )
{
    HRESULT hr = S_OK;
    CFloatValue* pValue = NULL;

    IFC(CFloatValue::Create(Value, &pValue));

    IFC(pPropertyObject->SetValue(pProperty, pValue));

Cleanup:
    ReleaseObject(pValue);

    return hr;
}

extern "C" UIFRAMEWORK_API
__checkReturn HRESULT 
CPropertyObject_SetValueString(
    __in CPropertyObject* pPropertyObject, 
    __in CProperty* pProperty,
    __in_z const WCHAR* strValue
    )
{
    HRESULT hr = S_OK;
    CStringValue* pValue = NULL;

    IFC(CStringValue::Create(strValue, &pValue));

    IFC(pPropertyObject->SetValue(pProperty, pValue));

Cleanup:
    ReleaseObject(pValue);

    return hr;
}

extern "C" UIFRAMEWORK_API
__checkReturn HRESULT 
CPropertyObject_SetValueVisibility(
    __in CPropertyObject* pPropertyObject, 
    __in CProperty* pProperty, 
    const Visibility::Value Val
    )
{
    HRESULT hr = S_OK;
    CVisibilityValue* pValue = NULL;

    IFC(CVisibilityValue::Create(Val, &pValue));

    IFC(pPropertyObject->SetValue(pProperty, pValue));

Cleanup:
    ReleaseObject(pValue);

    return hr;
}

extern "C" UIFRAMEWORK_API
__checkReturn HRESULT
CPropertyObject_GetValue(
    __in CPropertyObject* pPropertyObject,
    __in CProperty* pProperty, 
    __deref_out_opt CObjectWithType** ppValue
    )
{
    return pPropertyObject->GetValue(pProperty, ppValue);
}