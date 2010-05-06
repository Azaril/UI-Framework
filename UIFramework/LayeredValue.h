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

template< typename T >
class CTypedLayeredValue : public CLayeredValue
{
    public:
        CTypedLayeredValue( CProperty* pProperty ) : m_Property(pProperty),
                                                     m_StyleValue(NULL),
                                                     m_LocalValue(NULL)
        {
            AddRefObject(m_Property);

            //TODO: Query default when created?
        }

        virtual ~CTypedLayeredValue()
        {
            ReleaseObject(m_StyleValue);
            ReleaseObject(m_LocalValue);
            ReleaseObject(m_Property);
        }

        virtual HRESULT SetStyleValue( CObjectWithType* pObject, CProviders* pProviders )
        {
            HRESULT hr = S_OK;

            IFC(SetValueToLayer(pObject, &m_StyleValue, pProviders));

        Cleanup:
            return hr;
        }

        virtual HRESULT SetLocalValue( CObjectWithType* pObject, CProviders* pProviders )
        {
            HRESULT hr = S_OK;

            IFC(SetValueToLayer(pObject, &m_LocalValue, pProviders));

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

        virtual HRESULT GetEffectiveValue( CProviders* pProviders, CObjectWithType** ppValue )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pDefaultValue = NULL;
            
            IFCPTR(ppValue);

            if(m_LocalValue != NULL)
            {
                if(SUCCEEDED(GetValueFromLayer(m_LocalValue, pProviders, ppValue)))
                {
                    goto Cleanup;
                }
            }

            if(m_StyleValue != NULL)
            {
                if(SUCCEEDED(GetValueFromLayer(m_StyleValue, pProviders, ppValue)))
                {
                    goto Cleanup;
                }
            }

            IFCPTR(m_Property);

            IFC(m_Property->GetDefaultValue(&pDefaultValue));

            if(pDefaultValue)
            {
                IFCEXPECT(pDefaultValue->IsTypeOf(ObjectTypeTraits< T >::Type));
            }

            *ppValue = pDefaultValue;
            pDefaultValue = NULL;

        Cleanup:
            ReleaseObject(pDefaultValue);

            return hr;
        }

        HRESULT GetEffectiveValue( CProviders* pProviders, T** ppEffectiveValue )
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

        HRESULT GetValueFromLayer( CObjectWithType* pValue, CProviders* pProviders, CObjectWithType** ppEffectiveValue )
        {
            HRESULT hr = S_OK;

            IFCPTR(pValue);
            IFCPTR(ppEffectiveValue);

            if(pValue->IsTypeOf(ObjectTypeTraits< T >::Type))
            {
                *ppEffectiveValue = pValue;
                AddRefObject(pValue);
            }
            else
            {
                IFC(E_FAIL);
            }

        Cleanup:
            return hr;
        }

    protected:
        CProperty* m_Property;
        CObjectWithType* m_StyleValue;
        CObjectWithType* m_LocalValue;
};