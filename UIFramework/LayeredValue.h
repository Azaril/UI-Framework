#pragma once

#include "PropertyObject.h"
#include "Providers.h"
#include "Binding.h"
#include "Signals.h"

class CLayeredValue
{
    public:
        virtual HRESULT SetStyleValue( CObjectWithType* pObject, CProviders* pProviders ) = 0;
        virtual HRESULT SetLocalValue( CObjectWithType* pObject, CProviders* pProviders ) = 0;
        virtual HRESULT GetLocalValue( CProviders* pProviders, CObjectWithType** ppValue ) = 0;
        virtual HRESULT GetEffectiveValue( CProviders* pProviders, CObjectWithType** ppValue ) = 0;
};

namespace EffectiveValue
{
    enum Value
    {
        None    = 0x00,
        Default = 0x01,
        Style   = 0x02,
        Local   = 0x04,

        Last    = Local
    };
}

template< typename T >
class CTypedLocalLayeredValue : public CLayeredValue
{
    protected:
        struct ValueLayer
        {
            ValueLayer() : Value(NULL)
            {
            }

            ~ValueLayer()
            {
                ReleaseObject(Value);
            }

            CObjectWithType* Value;
            connection BindingInvalidationConnection;
        };

    public:
        CTypedLocalLayeredValue( CPropertyObject* pOwner, CProperty* pProperty ) : m_Owner(pOwner),
                                                                                   m_Property(pProperty),
                                                                                   m_EffectiveValue(EffectiveValue::Default),
                                                                                   m_EffectiveValueObject(NULL),
                                                                                   m_IsInvalidated(TRUE)
        {
            AddRefObject(m_Property);

            //TODO: Query default when created?
        }

        virtual ~CTypedLocalLayeredValue()
        {
            ReleaseObject(m_Property);
            ReleaseObject(m_EffectiveValueObject);
        }

        virtual HRESULT SetStyleValue( CObjectWithType* pObject, CProviders* pProviders )
        {
            HRESULT hr = S_OK;

            IFC(E_FAIL);

        Cleanup:
            return hr;
        }

        virtual HRESULT GetLocalValue( CProviders* pProviders, CObjectWithType** ppValue )
        {
            HRESULT hr = S_OK;

            IFC(GetValueFromLayer(m_LocalLayer, pProviders, ppValue));

        Cleanup:
            return hr;
        }

        virtual HRESULT SetLocalValue( CObjectWithType* pObject, CProviders* pProviders )
        {
            HRESULT hr = S_OK;
 
            IFC(SetValueToLayer(m_LocalLayer, EffectiveValue::Local, pObject, pProviders));

        Cleanup:
            return hr;
        }

        HRESULT RaiseValueChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue )
        {
            HRESULT hr = S_OK;

            IFCPTR(m_Property);

            if(((pOldValue != NULL && pNewValue != NULL) && !pNewValue->Equals(pOldValue)) || pNewValue != pOldValue)
            {
                IFC(m_Property->OnValueChanged(m_Owner, pOldValue, pNewValue));
            }

        Cleanup:
            return hr;
        }

        HRESULT GetEffectiveValue( CProviders* pProviders, CObjectWithType** ppValue )
        {
            HRESULT hr = S_OK;
            
            IFCPTR(ppValue);

            if(m_IsInvalidated)
            {
                EffectiveValue::Value EffectiveValueType = EffectiveValue::Last;

                IFCEXPECT(m_EffectiveValueObject == NULL);

                for(EffectiveValue::Value EffectiveValueType = EffectiveValue::Last; EffectiveValueType != EffectiveValue::None; EffectiveValueType = (EffectiveValue::Value)(EffectiveValueType >> 1))
                {
                    if(EffectiveValueType & m_EffectiveValue)
                    {
                        if(SUCCEEDED(GetEffectiveValueInternal(EffectiveValueType, pProviders, &m_EffectiveValueObject)))
                        {
                            m_ActualEffectiveValue = EffectiveValueType;

                            m_IsInvalidated = FALSE;
                            break;
                        }
                    }
                }
            }

            IFCEXPECT(!m_IsInvalidated);

            *ppValue = m_EffectiveValueObject;
            AddRefObject(m_EffectiveValueObject);

        Cleanup:
            return hr;
        }

        HRESULT GetTypedEffectiveValue( CProviders* pProviders, T** ppEffectiveValue )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pValue = NULL;

            IFCPTR(ppEffectiveValue);

            IFC(GetEffectiveValue(pProviders, &pValue));

            if(pValue)
            {
                IFCEXPECT(pValue->IsTypeOf(ObjectTypeTraits< T >::Type));
            }

            *ppEffectiveValue = (T*)pValue;
            pValue = NULL;

        Cleanup:
            ReleaseObject(pValue);

            return hr;
        }

    protected:
        virtual HRESULT GetEffectiveValueInternal( EffectiveValue::Value ValueToGet, CProviders* pProviders, CObjectWithType** ppValue )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pDefaultValue = NULL;

            switch(ValueToGet)
            {
                case EffectiveValue::Local:
                    {
                        IFC(GetValueFromLayer(m_LocalLayer, pProviders, ppValue));

                        break;
                    }

                case EffectiveValue::Default:
                    {
                        IFCPTR(m_Property);

                        IFC(m_Property->GetDefaultValue(&pDefaultValue));

                        if(pDefaultValue)
                        {
                            IFCEXPECT(pDefaultValue->IsTypeOf(ObjectTypeTraits< T >::Type));
                        }

                        *ppValue = pDefaultValue;
                        pDefaultValue = NULL;

                        break;
                    }

                default:
                    {
                        IFC(E_FAIL);
                    }
            }

        Cleanup:
            ReleaseObject(pDefaultValue);

            return hr;
        }

        HRESULT SetValueToLayer( ValueLayer& Layer, EffectiveValue::Value ValueType, CObjectWithType* pValue, CProviders* pProviders )
        {
            HRESULT hr = S_OK;
            CTypeConverter* pTypeConverter = NULL;
            CObjectWithType* pConvertedType = NULL;
            BOOL IsEffectiveValue = (m_ActualEffectiveValue <= ValueType);
            CObjectWithType* pOldValue = NULL;
            CObjectWithType* pNewValue = NULL;
            CObjectWithType* pOldBinding = NULL;

            IFCPTR(pValue);

            if(IsEffectiveValue)
            {
                pOldValue = m_EffectiveValueObject;
                AddRefObject(pOldValue);
            }

            if(Layer.Value && Layer.Value->IsTypeOf(TypeIndex::Binding))
            {
                CBinding* pOldBinding = (CBinding*)Layer.Value;

                IFC(pOldBinding->ClearTarget());
            }

            ReleaseObject(Layer.Value);
            Layer.BindingInvalidationConnection.disconnect();

            if(pValue)
            {
                if(pValue->IsTypeOf(ObjectTypeTraits< T >::Type))
                {
                    Layer.Value = (T*)pValue;
                    AddRefObject(pValue);
                }
                else if(pValue->IsTypeOf(TypeIndex::Binding))
                {
                    CBinding* pBinding = (CBinding*)pValue;

                    Layer.Value = (CBinding*)pBinding;
                    AddRefObject(pValue);

                    IFC(pBinding->SetTarget(m_Owner, m_Property));

                    IFC(pBinding->AddChangeListener(bind(&CTypedLocalLayeredValue< T >::OnBindingInvalidated, this, ValueType, pProviders, _1), &Layer.BindingInvalidationConnection));
                }
                else 
                {
                    IFCPTR(pProviders);

                    pTypeConverter = pProviders->GetTypeConverter();
                    IFCPTR(pTypeConverter);

                    CConversionContext Context(m_Owner, m_Property, pProviders);

                    hr = pTypeConverter->Convert(&Context, pValue, &pConvertedType);

                    if(SUCCEEDED(hr))
                    {
                        Layer.Value = pConvertedType;
                        pConvertedType = NULL;
                    }
                    else
                    {
                        IFC(hr);
                    }
                }
            }

            m_EffectiveValue = (EffectiveValue::Value)(m_EffectiveValue | ValueType);

            if(IsEffectiveValue)
            {
                m_IsInvalidated = TRUE;

                IFC(GetEffectiveValue(pProviders, &pNewValue));

                IFC(RaiseValueChanged(pOldValue, pNewValue));
            }  

        Cleanup:
            ReleaseObject(pConvertedType);
            ReleaseObject(pOldValue);
            ReleaseObject(pNewValue);

            return hr;
        }

        HRESULT GetValueFromLayer( ValueLayer& Layer, CProviders* pProviders, CObjectWithType** ppEffectiveValue )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pBoundValue = NULL;

            IFCPTR(ppEffectiveValue);

            if(Layer.Value)
            {
                if(Layer.Value->IsTypeOf(ObjectTypeTraits< T >::Type))
                {
                    *ppEffectiveValue = Layer.Value;
                    AddRefObject(Layer.Value);
                }
                else if(Layer.Value->IsTypeOf(TypeIndex::Binding))
                {
                    CBinding* pBinding = (CBinding*)Layer.Value;

                    IFC(pBinding->GetBoundValue(&pBoundValue));

                    if(pBoundValue)
                    {
                        if(pBoundValue->IsTypeOf(ObjectTypeTraits< T >::Type))
                        {
                            *ppEffectiveValue = pBoundValue;
                            pBoundValue = NULL;
                        }
                        else
                        {
                            CTypeConverter* pTypeConverter = NULL;

                            IFCPTR(pProviders);

                            pTypeConverter = pProviders->GetTypeConverter();

                            CConversionContext Context(m_Owner, m_Property, pProviders);

                            hr = pTypeConverter->Convert(&Context, pBoundValue, ppEffectiveValue);
                        }
                    }
                }
                else
                {
                    IFC(E_FAIL);
                }
            }
            else
            {
                *ppEffectiveValue = NULL;
            }

        Cleanup:
            ReleaseObject(pBoundValue);

            return hr;
        }

        void OnBindingInvalidated( EffectiveValue::Value BindingValueChanged, CProviders* pProviders, CBinding* pBinding )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pOldValue = NULL;
            CObjectWithType* pNewValue = NULL;

            if(BindingValueChanged >= m_ActualEffectiveValue)
            {
                IFCPTR(pBinding);

                pOldValue = m_EffectiveValueObject;
                m_EffectiveValueObject = NULL;

                m_IsInvalidated = TRUE;

                IFC(GetEffectiveValue(pProviders, &pNewValue));

                IFC(RaiseValueChanged(pOldValue, pNewValue));
            }

        Cleanup:
            ReleaseObject(pOldValue);
            ReleaseObject(pNewValue);
        }

        EffectiveValue::Value m_EffectiveValue;
        EffectiveValue::Value m_ActualEffectiveValue;
        CPropertyObject* m_Owner;
        CProperty* m_Property;
        CObjectWithType* m_EffectiveValueObject;
        BOOL m_IsInvalidated;
        ValueLayer m_LocalLayer;
};

template< typename T >
class CTypedLayeredValue : public CTypedLocalLayeredValue< T >
{
    typedef CTypedLocalLayeredValue< T > Base;
    public:
        CTypedLayeredValue( CPropertyObject* pOwner, CProperty* pProperty ) : CTypedLocalLayeredValue< T >(pOwner, pProperty)
        {
        }

        virtual ~CTypedLayeredValue()
        {
        }

        virtual HRESULT SetStyleValue( CObjectWithType* pObject, CProviders* pProviders )
        {
            HRESULT hr = S_OK;

            IFC(SetValueToLayer(m_StyleLayer, EffectiveValue::Style, pObject, pProviders));        

        Cleanup:
            return hr;
        }

    protected:
        virtual HRESULT GetEffectiveValueInternal(EffectiveValue::Value ValueToGet, CProviders* pProviders, CObjectWithType** ppValue)
        {
            HRESULT hr = S_OK;

            switch(ValueToGet)
            {
                case EffectiveValue::Style:
                    {
                        IFC(GetValueFromLayer(m_StyleLayer, pProviders, ppValue));

                        break;
                    }

                default:
                    {
                        IFC(Base::GetEffectiveValueInternal(ValueToGet, pProviders, ppValue));

                        break;
                    }
            }

        Cleanup:
            return hr;
        }

        ValueLayer m_StyleLayer;
};