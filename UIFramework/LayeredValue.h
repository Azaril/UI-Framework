#pragma once

#include "PropertyObject.h"
#include "Providers.h"

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
        Default = 0x00,
        Local   = 0x01,
        Style   = 0x02,

        Last    = Style
    };
}

template< typename T >
class CTypedLocalLayeredValue : public CLayeredValue
{
    public:
        CTypedLocalLayeredValue( CPropertyObject* pOwner, CProperty* pProperty ) : m_Owner(pOwner),
                                                                                   m_Property(pProperty),
                                                                                   m_LocalValue(NULL),
                                                                                   m_EffectiveValue(EffectiveValue::Default)
        {
            AddRefObject(m_Property);

            //TODO: Query default when created?
        }

        virtual ~CTypedLocalLayeredValue()
        {
            ReleaseObject(m_LocalValue);
            ReleaseObject(m_Property);
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

            IFC(GetValueFromLayer(m_LocalValue, pProviders, ppValue));

        Cleanup:
            return hr;
        }

        virtual HRESULT SetLocalValue( CObjectWithType* pObject, CProviders* pProviders )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pOldValue = NULL;
            CObjectWithType* pNewValue = NULL;
            BOOL RaiseChanged = (m_EffectiveValue <= EffectiveValue::Local);

            if(RaiseChanged)
            {
                IFC(GetEffectiveValue(pProviders, &pOldValue));
            }

            IFC(SetValueToLayer(pObject, &m_LocalValue, pProviders));

            m_EffectiveValue = (EffectiveValue::Value)(m_EffectiveValue | EffectiveValue::Local);

            if(RaiseChanged)
            {
                IFC(GetEffectiveValue(pProviders, &pNewValue));

                IFC(RaiseValueChanged(pOldValue, pNewValue));
            }  

        Cleanup:
            ReleaseObject(pOldValue);
            ReleaseObject(pNewValue);

            return hr;
        }

        HRESULT RaiseValueChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue )
        {
            HRESULT hr = S_OK;

            IFCPTR(m_Property);

            IFC(m_Property->OnValueChanged(m_Owner, pOldValue, pNewValue));

        Cleanup:
            return hr;
        }

        virtual HRESULT GetEffectiveValue( CProviders* pProviders, CObjectWithType** ppValue )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pDefaultValue = NULL;
            
            IFCPTR(ppValue);

            EffectiveValue::Value EffectiveValueType = EffectiveValue::Last;

            while(EffectiveValueType && !(EffectiveValueType & m_EffectiveValue))
            {
                EffectiveValueType = (EffectiveValue::Value)(EffectiveValueType >> 1);
            }

            switch(EffectiveValueType)
            {
                case EffectiveValue::Local:
                    {
                        IFC(GetValueFromLayer(m_LocalValue, pProviders, ppValue));

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
        HRESULT SetValueToLayer( CObjectWithType* pValue, CObjectWithType** ppLayer, CProviders* pProviders )
        {
            HRESULT hr = S_OK;
            CTypeConverter* pTypeConverter = NULL;
            CObjectWithType* pConvertedType = NULL;

            IFCPTR(pValue);
            IFCPTR(ppLayer);

            ReleaseObject(*ppLayer);

            //TODO: Support binding and expressions.

            if(pValue)
            {
                if(pValue->IsTypeOf(ObjectTypeTraits< T >::Type))
                {
                    *ppLayer = (T*)pValue;
                    AddRefObject(pValue);
                }
                else 
                {
                    IFCPTR(pProviders);

                    pTypeConverter = pProviders->GetTypeConverter();
                    IFCPTR(pTypeConverter);

                    hr = pTypeConverter->Convert(pValue, ObjectTypeTraits< T >::Type, &pConvertedType);

                    if(SUCCEEDED(hr))
                    {
                        *ppLayer = pConvertedType;
                        pConvertedType = NULL;
                    }
                    else
                    {
                        IFC(hr);
                    }
                }
            }

        Cleanup:
            ReleaseObject(pConvertedType);

            return hr;
        }

        HRESULT GetValueFromLayer( CObjectWithType* pValue, CProviders* pProviders, CObjectWithType** ppEffectiveValue )
        {
            HRESULT hr = S_OK;

            IFCPTR(pValue);
            IFCPTR(ppEffectiveValue);

            if(pValue)
            {
                if(pValue->IsTypeOf(ObjectTypeTraits< T >::Type))
                {
                    *ppEffectiveValue = pValue;
                    AddRefObject(pValue);
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
            return hr;
        }

        EffectiveValue::Value m_EffectiveValue;
        CPropertyObject* m_Owner;
        CProperty* m_Property;
        CObjectWithType* m_LocalValue;
};

template< typename T >
class CTypedLayeredValue : public CTypedLocalLayeredValue< T >
{
    public:
        CTypedLayeredValue( CPropertyObject* pOwner, CProperty* pProperty ) : CTypedLocalLayeredValue< T >(pOwner, pProperty),
                                                                              m_StyleValue(NULL)
        {
        }

        virtual ~CTypedLayeredValue()
        {
            ReleaseObject(m_StyleValue);
        }

        virtual HRESULT SetStyleValue( CObjectWithType* pObject, CProviders* pProviders )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pOldValue = NULL;
            CObjectWithType* pNewValue = NULL;
            BOOL RaiseChanged = (m_EffectiveValue <= EffectiveValue::Style);

            if(RaiseChanged)
            {
                IFC(GetEffectiveValue(pProviders, &pOldValue));
            }

            IFC(SetValueToLayer(pObject, &m_StyleValue, pProviders));

            m_EffectiveValue = (EffectiveValue::Value)(m_EffectiveValue | EffectiveValue::Style);

            if(RaiseChanged)
            {
                IFC(GetEffectiveValue(pProviders, &pNewValue));

                IFC(RaiseValueChanged(pOldValue, pNewValue));
            }           

        Cleanup:
            ReleaseObject(pOldValue);
            ReleaseObject(pNewValue);

            return hr;
        }

        virtual HRESULT GetEffectiveValue( CProviders* pProviders, CObjectWithType** ppValue )
        {
            HRESULT hr = S_OK;
            
            IFCPTR(ppValue);

            if(m_StyleValue != NULL)
            {
                if(SUCCEEDED(GetValueFromLayer(m_StyleValue, pProviders, ppValue)))
                {
                    goto Cleanup;
                }
            }

            IFC(CTypedLocalLayeredValue< T >::GetEffectiveValue(pProviders, ppValue));

        Cleanup:
            return hr;
        }

    protected:
        CObjectWithType* m_StyleValue;
};