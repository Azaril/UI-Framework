#include "PropertyObject.h"
#include "ErrorChecking.h"
#include "BasicTypes.h"
#include "LayeredValue.h"
#include "BindingContext.h"
#include "LayeredValue.h"

class CAttachedPropertyHolder
{
    public:
        CAttachedPropertyHolder(
            __in CProperty* pProperty
            );

        ~CAttachedPropertyHolder(
            );

        __out CProperty* GetProperty(
            );

        __out CLayeredValue* GetLayeredValue(
            );

    protected:
        CProperty* m_Property;
        CLayeredValue m_Value;
};

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
    : m_pProviders(NULL)
    , m_BindingContext(NULL)
#ifdef FRAMEWORK_DEBUG
    , m_InitializeCalled(false)
#endif
{
}

CPropertyObject::~CPropertyObject(
    )
{
#ifdef FRAMEWORK_DEBUG
    ASSERT(m_InitializeCalled);
#endif

    for(vector< CAttachedPropertyHolder* >::iterator It = m_AttachedProperties.begin(); It != m_AttachedProperties.end(); ++It)
    {
        CAttachedPropertyHolder* pPropertyHolder = (*It);

        delete pPropertyHolder;
    }

    ReleaseObject(m_BindingContext);
    ReleaseObject(m_pProviders);
}

__checkReturn HRESULT 
CPropertyObject::Initialize(
    __in CProviders* pProviders
    )
{
    HRESULT hr = S_OK;

#ifdef FRAMEWORK_DEBUG
    m_InitializeCalled = true;
#endif

    SetObject(m_pProviders, pProviders);

    return hr;
}

__checkReturn HRESULT
CPropertyObject::SetValue(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    IFCEXPECT(!pProperty->IsReadOnly());

    IFC(SetValueToLayer(EffectiveValue::Local, pProperty, pValue));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CPropertyObject::SetValue(
    __in CProperty* pProperty,
    const FLOAT& value
    )
{
    HRESULT hr = S_OK;
    CFloatValue* pFloatValue = NULL;

    IFC(CFloatValue::Create(value, &pFloatValue));

    IFC(SetValue(pProperty, pFloatValue));

Cleanup:
    ReleaseObject(pFloatValue);

    return hr;
}

__checkReturn HRESULT 
CPropertyObject::SetValue(
    __in CProperty* pProperty,
    const RectF& value
    )
{
    HRESULT hr = S_OK;
    CRectFValue* pRectFValue = NULL;

    IFC(CRectFValue::Create(value, &pRectFValue));

    IFC(SetValue(pProperty, pRectFValue));

Cleanup:
    ReleaseObject(pRectFValue);

    return hr;
}

__checkReturn HRESULT 
CPropertyObject::SetValueReadOnly(
    __in CProperty* pProperty, 
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    IFC(SetValueToLayer(EffectiveValue::Local, pProperty, pValue));

Cleanup:
    return hr;
}

__out CProviders*
CPropertyObject::GetProviders(
    )
{
#ifdef FRAMEWORK_DEBUG
    ASSERT(m_InitializeCalled);
#endif

    return m_pProviders;
}

__checkReturn HRESULT
CPropertyObject::SetValueToLayer(
    EffectiveValue::Value layer,
    __in CProperty* pProperty,
    __in CObjectWithType* pValue
    )
{
    HRESULT hr = S_OK;
    CLayeredValue* pLayeredValue = NULL;
    bool setEffectiveValue = false;
    CObjectWithType* pPreviousEffectiveValue = NULL;
    CObjectWithType* pConvertedType = NULL;
    CObjectWithType* pNewValue = NULL;

    if (pValue->IsTypeOf(pProperty->GetType()))
    {
        pNewValue = pValue;
    }
    else
    {
        CProviders* pProviders = GetProviders();
                    
        CTypeConverter* pTypeConverter = pProviders->GetTypeConverter();
        IFCPTR(pTypeConverter);

        CConversionContext Context(this, pProperty, pProviders);

        IFC(pTypeConverter->Convert(&Context, pValue, &pConvertedType));

        pNewValue = pConvertedType;
    }

    if (pProperty->IsAttached())
    {
        IFC(GetOrCreateAttachedLayeredValue(pProperty, &pLayeredValue));
    }
    else
    {
        IFC(GetLayeredValue(pProperty, &pLayeredValue));
    }

    IFC(pLayeredValue->SetValueToLayer(layer, pNewValue, &setEffectiveValue, &pPreviousEffectiveValue));

    if (setEffectiveValue)
    {
        IFC(pProperty->OnValueChanged(this, pPreviousEffectiveValue, pNewValue));

        IFC(RaisePropertyChanged(pProperty));
    }

Cleanup:
    ReleaseObject(pPreviousEffectiveValue);
    ReleaseObject(pConvertedType);

    return hr;
}

__checkReturn HRESULT
CPropertyObject::GetOrCreateAttachedLayeredValue(
    __in CProperty* pProperty,
    __out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;
    CAttachedPropertyHolder* pPropertyHolder = NULL;

    for(vector< CAttachedPropertyHolder* >::iterator It = m_AttachedProperties.begin(); It != m_AttachedProperties.end(); ++It)
    {
        if((*It)->GetProperty() == pProperty)
        {
            pPropertyHolder = (*It);

            break;
        }
    }

    if(pPropertyHolder == NULL)
    {
        pPropertyHolder = new CAttachedPropertyHolder(pProperty);
        IFCOOM(pPropertyHolder);

        m_AttachedProperties.push_back(pPropertyHolder);
    }

    *ppLayeredValue = pPropertyHolder->GetLayeredValue();

Cleanup:
    return hr;
}

__checkReturn HRESULT
CPropertyObject::ClearValueFromLayer(
    EffectiveValue::Value layer,
    __in CProperty* pProperty
    )
{
    HRESULT hr = S_OK;
    CLayeredValue* pLayeredValue = NULL;
    bool setEffectiveValue = false;
    CObjectWithType* pPreviousEffectiveValue = NULL;

    if (pProperty->IsAttached())
    {
        IFC(GetOrCreateAttachedLayeredValue(pProperty, &pLayeredValue));
    }
    else
    {
        IFC(GetLayeredValue(pProperty, &pLayeredValue));
    }

    IFC(pLayeredValue->ClearValueFromLayer(layer, &setEffectiveValue, &pPreviousEffectiveValue));

    if (setEffectiveValue)
    {
        IFC(pProperty->OnValueChanged(this, pPreviousEffectiveValue, NULL));

        IFC(RaisePropertyChanged(pProperty));
    }

Cleanup:
    ReleaseObject(pPreviousEffectiveValue);

    return hr;
}

__checkReturn HRESULT 
CPropertyObject::GetValue(
    __in CProperty* pProperty,
    __deref_out_opt CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    IFC(GetValueFromLayer(EffectiveValue::Local, pProperty, ppValue));

Cleanup:
    return hr;
}

__checkReturn HRESULT
CPropertyObject::GetValueFromLayer(
    EffectiveValue::Value layer,
    __in CProperty* pProperty,
    __deref_out_opt CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;
    CLayeredValue* pLayeredValue;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    if (pProperty->IsAttached())
    {
        //TODO: Make this not allocate? Return default value?
        IFC(GetOrCreateAttachedLayeredValue(pProperty, &pLayeredValue));
    }
    else
    {
        IFC(GetLayeredValue(pProperty, &pLayeredValue));
    }

    IFC(pLayeredValue->GetValueFromLayer(layer, ppValue));

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
CPropertyObject::GetEffectiveValue(
    __in CProperty* pProperty, 
    __deref_out_opt CObjectWithType** ppValue
    )
{
    HRESULT hr = S_OK;
    CLayeredValue* pLayeredValue = NULL;

    IFCPTR(pProperty);
    IFCPTR(ppValue);

    IFC(GetLayeredValue(pProperty, &pLayeredValue));

    IFC(pLayeredValue->GetEffectiveValue(pProperty, ppValue));

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
    __in CProperty* pProperty
    )
    : m_Property(pProperty)
{
    AddRefObject(m_Property);
}

CAttachedPropertyHolder::~CAttachedPropertyHolder(
    )
{
    ReleaseObject(m_Property);
}

__out CProperty* 
CAttachedPropertyHolder::GetProperty(
    )
{
    return m_Property;
}

__out CLayeredValue*
CAttachedPropertyHolder::GetLayeredValue( 
    )
{
    return &m_Value;
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