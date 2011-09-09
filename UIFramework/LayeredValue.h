#pragma once

#include "PropertyObject.h"
#include "Providers.h"
#include "Binding.h"
#include "Signals.h"

class CLayeredValue
{
    public:
        virtual HRESULT SetLocalValue( CObjectWithType* pObject ) = 0;
        virtual HRESULT GetLocalValue( CObjectWithType** ppValue ) = 0;
        virtual HRESULT ClearLocalValue( ) = 0;

        virtual HRESULT SetStyleValue( CObjectWithType* pObject ) = 0;
        
        virtual HRESULT SetAnimationValue( CObjectWithType* pObject ) = 0;

        virtual HRESULT GetEffectiveValue( CObjectWithType** ppValue ) = 0;
        virtual HRESULT SetEffectiveValue( CObjectWithType* pValue ) = 0;
};

namespace EffectiveValue
{
    enum Value
    {
        None        = 0x00,
        Default     = 0x01,
        Style       = 0x02,
        Local       = 0x04,
        Animation   = 0x08,

        Last        = Animation
    };
}

struct UIFRAMEWORK_API ValueLayer
{
    ValueLayer() : Value(NULL)
    {
    }
    
    ~ValueLayer()
    {
        ReleaseObject(Value);
    }
    
    CObjectWithType* Value;
};

template< typename OwnerType, typename T >
class UIFRAMEWORK_API CTypedLocalLayeredValue : public CLayeredValue
{
    typedef CTypedLocalLayeredValue< OwnerType, T > ThisType;

    public:
        CTypedLocalLayeredValue( OwnerType* pOwner, CProperty* pProperty ) : m_Owner(pOwner),
                                                                             m_Property(pProperty),
                                                                             m_EffectiveValue(EffectiveValue::Default),
                                                                             m_ActualEffectiveValue(EffectiveValue::None),
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

        CProviders* GetProviders()
        {
            return m_Owner->GetProviders();
        }

        virtual HRESULT SetStyleValue( CObjectWithType* pObject )
        {
            HRESULT hr = S_OK;

            IFC(E_FAIL);

        Cleanup:
            return hr;
        }

        virtual HRESULT GetLocalValue( CObjectWithType** ppValue )
        {
            HRESULT hr = S_OK;

            IFC(GetValueFromLayer(m_LocalLayer, ppValue));

        Cleanup:
            return hr;
        }

        virtual HRESULT SetLocalValue( CObjectWithType* pObject )
        {
            HRESULT hr = S_OK;
 
            IFC(SetValueToLayer(m_LocalLayer, EffectiveValue::Local, pObject));

        Cleanup:
            return hr;
        }

        virtual HRESULT ClearLocalValue( )
        {
            HRESULT hr = S_OK;

            IFC(ClearLayer(m_LocalLayer, EffectiveValue::Local));

        Cleanup:
            return hr;
        }

        virtual HRESULT SetAnimationValue( CObjectWithType* pObject )
        {
            HRESULT hr = S_OK;

            IFC(SetValueToLayer(m_AnimationLayer, EffectiveValue::Animation, pObject));

        Cleanup:
            return hr;
        }

        HRESULT RaiseValueChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue )
        {
            HRESULT hr = S_OK;

            IFCPTR(m_Property);

            if(pOldValue == NULL || pNewValue == NULL || !pNewValue->Equals(pOldValue))
            {
                IFC(m_Property->OnValueChanged(m_Owner, pOldValue, pNewValue));

                IFC(m_Owner->RaisePropertyChanged(m_Property));
            }

        Cleanup:
            return hr;
        }

        HRESULT GetEffectiveValue( CObjectWithType** ppValue )
        {
            HRESULT hr = S_OK;
            
            IFCPTR(ppValue);

            if(m_IsInvalidated)
            {
                IFCEXPECT(m_EffectiveValueObject == NULL);

                for(EffectiveValue::Value EffectiveValueType = EffectiveValue::Last; EffectiveValueType != EffectiveValue::None; EffectiveValueType = (EffectiveValue::Value)(EffectiveValueType >> 1))
                {
                    if(EffectiveValueType & m_EffectiveValue)
                    {
                        if(SUCCEEDED(GetEffectiveValueInternal(EffectiveValueType, &m_EffectiveValueObject)))
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

        virtual HRESULT SetEffectiveValue( CObjectWithType* pValue )
        {
            HRESULT hr = S_OK;

            switch(m_ActualEffectiveValue)
            {
                    //NOTE: Set the local value but let the animation override it.
                case EffectiveValue::Animation:
                    //NOTE: This case needs to be supported if the default value is a binding object.
                case EffectiveValue::Default:
                case EffectiveValue::Local:
                    {
                        IFC(SetLocalValue(pValue));

                        break;
                    }

                default:
                    {
                        //TODO: Should set failures be bubbled out? This is only used for binding.
                        break;
                    }
            }

        Cleanup:
            return hr;
        }

        HRESULT GetTypedEffectiveValue( T** ppEffectiveValue )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pValue = NULL;

            IFCPTR(ppEffectiveValue);

            IFC(GetEffectiveValue(&pValue));

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
        virtual HRESULT GetEffectiveValueInternal( EffectiveValue::Value ValueToGet, CObjectWithType** ppValue )
        {
            HRESULT hr = S_OK;
            CObjectWithType* pDefaultValue = NULL;

            switch(ValueToGet)
            {
                case EffectiveValue::Animation:
                    {
                        IFC_NOTRACE(GetValueFromLayer(m_AnimationLayer, ppValue));

                        break;
                    }

                case EffectiveValue::Local:
                    {
                        IFC_NOTRACE(GetValueFromLayer(m_LocalLayer, ppValue));

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

        HRESULT ClearLayer( ValueLayer& Layer, EffectiveValue::Value ValueType )
        {
            HRESULT hr = S_OK;
            BOOL IsEffectiveValue = (m_ActualEffectiveValue <= ValueType);
            CObjectWithType* pOldValue = NULL;
            CObjectWithType* pNewValue = NULL;

            if(IsEffectiveValue)
            {
                pOldValue = m_EffectiveValueObject;
                m_EffectiveValueObject = NULL;
            }

            if(Layer.Value && Layer.Value->IsTypeOf(TypeIndex::BindingBase))
            {
                CBinding* pOldBinding = (CBinding*)Layer.Value;

                IFC(pOldBinding->ClearTarget());
            }

            ReleaseObject(Layer.Value);

            m_EffectiveValue = (EffectiveValue::Value)(m_EffectiveValue & (~ValueType));

            if(IsEffectiveValue)
            {
                m_IsInvalidated = TRUE;

                IFC(GetEffectiveValue(&pNewValue));

                IFC(RaiseValueChanged(pOldValue, pNewValue));
            }

        Cleanup:
            ReleaseObject(pOldValue);
            ReleaseObject(pNewValue);

            return hr;
        }

        HRESULT SetValueToLayer( ValueLayer& Layer, EffectiveValue::Value ValueType, CObjectWithType* pValue )
        {
            HRESULT hr = S_OK;
            CTypeConverter* pTypeConverter = NULL;
            CObjectWithType* pConvertedType = NULL;
            BOOL IsEffectiveValue = (m_ActualEffectiveValue <= ValueType);
            CObjectWithType* pOldValue = NULL;
            CObjectWithType* pNewValue = NULL;

            IFCPTR(pValue);

            if(IsEffectiveValue)
            {
                pOldValue = m_EffectiveValueObject;
                AddRefObject(pOldValue);
            }

            if(pValue)
            {
                if(pValue->IsTypeOf(ObjectTypeTraits< T >::Type))
                {
                    Layer.Value = (T*)pValue;
                    AddRefObject(pValue);
                }
                else 
                {
                    CProviders* pProviders = GetProviders();
                    
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
                ReleaseObject(m_EffectiveValueObject);

                m_IsInvalidated = TRUE;

                IFC(GetEffectiveValue(&pNewValue));

                IFC(RaiseValueChanged(pOldValue, pNewValue));
            }  

        Cleanup:
            ReleaseObject(pConvertedType);
            ReleaseObject(pOldValue);
            ReleaseObject(pNewValue);

            return hr;
        }

        HRESULT GetValueFromLayer( ValueLayer& Layer, CObjectWithType** ppEffectiveValue )
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

        EffectiveValue::Value m_EffectiveValue;
        EffectiveValue::Value m_ActualEffectiveValue;
        OwnerType* m_Owner;
        CProperty* m_Property;
        CObjectWithType* m_EffectiveValueObject;
        BOOL m_IsInvalidated;
        ValueLayer m_LocalLayer;
        ValueLayer m_AnimationLayer;
};

template< typename OwnerType, typename T >
class UIFRAMEWORK_API CTypedLayeredValue : public CTypedLocalLayeredValue< OwnerType, T >
{
    typedef CTypedLocalLayeredValue< OwnerType, T > Base;

    public:
        CTypedLayeredValue( OwnerType* pOwner, CProperty* pProperty ) : Base(pOwner, pProperty)
        {
        }

        virtual ~CTypedLayeredValue()
        {
        }

        virtual HRESULT SetStyleValue( CObjectWithType* pObject )
        {
            HRESULT hr = S_OK;

            IFC(Base::SetValueToLayer(m_StyleLayer, EffectiveValue::Style, pObject));

        Cleanup:
            return hr;
        }

        virtual HRESULT SetEffectiveValue( CObjectWithType* pValue )
        {
            HRESULT hr = S_OK;

            switch(this->m_ActualEffectiveValue)
            {
                case EffectiveValue::Style:
                    {
                        IFC(SetStyleValue(pValue));

                        break;
                    }

                default:
                    {
                        IFC(Base::SetEffectiveValue(pValue));
                        
                        break;
                    }
            }

        Cleanup:
            return hr;
        }

    protected:
        virtual HRESULT GetEffectiveValueInternal(EffectiveValue::Value ValueToGet, CObjectWithType** ppValue)
        {
            HRESULT hr = S_OK;

            switch(ValueToGet)
            {
                case EffectiveValue::Style:
                    {
                        IFC(Base::GetValueFromLayer(m_StyleLayer, ppValue));

                        break;
                    }

                default:
                    {
                        IFC_NOTRACE(Base::GetEffectiveValueInternal(ValueToGet, ppValue));

                        break;
                    }
            }

        Cleanup:
            return hr;
        }

        ValueLayer m_StyleLayer;
};