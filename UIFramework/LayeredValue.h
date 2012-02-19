#pragma once

#include "PropertyObject.h"
#include "Providers.h"
//#include "Binding.h"
#include "Signals.h"

class UIFRAMEWORK_API CLayeredValue
{
    class CLayerBlock
    {
        public:
            CLayerBlock(
                )
                : m_pNext(NULL)
                , m_ValueType(EffectiveValue::None)
                , m_pValue(NULL)
            {
            }

            CLayerBlock(
                EffectiveValue::Value valueType,
                __in_opt CLayerBlock* pNext
                )
                : m_pNext(pNext)
                , m_ValueType(valueType)
                , m_pValue(NULL)
            {
            }

            CLayerBlock(
                EffectiveValue::Value valueType
                )
                : m_pNext(NULL)
                , m_ValueType(valueType)
                , m_pValue(NULL)
            {
            }

            CLayerBlock(
                EffectiveValue::Value valueType,
                __in_opt CObjectWithType* pValue
                )
                : m_pNext(NULL)
                , m_ValueType(valueType)
                , m_pValue(pValue)
            {
                AddRefObject(pValue);
            }

            ~CLayerBlock(
                )
            {
                Reset();
            }

            void Reset(
                )
            {
                m_ValueType = EffectiveValue::None;

                ReleaseObject(m_pValue);
            }

            __out_opt CLayerBlock* GetNext(
                )
            {
                return m_pNext;
            }

            void SetNext(
                __in_opt CLayerBlock* pNext
                )
            {
                m_pNext = pNext;
            }

            EffectiveValue::Value GetValueType(
                )
            {
                return m_ValueType;
            }

            void SetValueType(
                EffectiveValue::Value valueType
                )
            {
                m_ValueType = valueType;
            }

            void SetValue(
                __in CObjectWithType* pValue
                )
            {
                ReplaceObject(m_pValue, pValue);
            }

            __out_opt CObjectWithType* GetValue(
                )
            {
                return m_pValue;
            }

        private:
            CLayerBlock(
                const CLayerBlock&
                );

            CLayerBlock& operator=(
                const CLayerBlock&
                );

        protected:
            EffectiveValue::Value m_ValueType;
            CLayerBlock* m_pNext;
            CObjectWithType* m_pValue;
    };

    public:
        CLayeredValue(
            )
            : m_pHeadBlock(NULL)
        {
        }

        ~CLayeredValue(
            )
        {
            CLayerBlock* pBlock = m_pHeadBlock;
            CLayerBlock* pNextBlock = NULL;

            while (pBlock != NULL)
            {
                pNextBlock = pBlock->GetNext();

                FreeBlock(pBlock);

                pBlock = pNextBlock;
            }
        }

        __checkReturn HRESULT GetEffectiveValue(
            __in CProperty* pProperty,
            __deref_out_opt CObjectWithType** ppValue 
            )
        {
            HRESULT hr = S_OK;

#ifdef FRAMEWORK_DEBUG
            CObjectWithType* pDebugDefaultValue = NULL;
#endif
            
            if (m_pHeadBlock != NULL)
            {
                SetObject(*ppValue, m_pHeadBlock->GetValue());
            }
            else
            {
                IFC(pProperty->GetDefaultValue(ppValue));

#ifdef FRAMEWORK_DEBUG
                //
                // Verify that this value is a single instance that can be shared.
                // This saves having to allocate a default block to store the value.
                //
                IFC(pProperty->GetDefaultValue(&pDebugDefaultValue));

                ASSERT(pDebugDefaultValue == *ppValue);
#endif
            }

        Cleanup:
#ifdef FRAMEWORK_DEBUG
            ReleaseObject(pDebugDefaultValue);
#endif
            return hr;
        }

        __checkReturn HRESULT SetValueToLayer(
            EffectiveValue::Value valueType,
            __in_opt CObjectWithType* pValue,
            __out bool* pReplacedEffectiveValue,
            __deref_out_opt CObjectWithType** ppPreviousEffectiveValue
            )
        {
            HRESULT hr = S_OK;
            CLayerBlock* pBlock = NULL;

            bool isEffectiveValue = (m_pHeadBlock == NULL || valueType >= m_pHeadBlock->GetValueType());

            if (isEffectiveValue && m_pHeadBlock != NULL)
            {
                SetObject(*ppPreviousEffectiveValue, m_pHeadBlock->GetValue());
            }
            else
            {
                *ppPreviousEffectiveValue = NULL;
            }

            IFC(GetOrCreateLayer(valueType, &pBlock));

            pBlock->SetValue(pValue);

            *pReplacedEffectiveValue = isEffectiveValue;

        Cleanup:
            return hr;
        }

        __checkReturn HRESULT GetValueFromLayer(
            EffectiveValue::Value valueType,
            __deref_out_opt CObjectWithType** ppValue
            )
        {
            HRESULT hr = S_OK;

            CLayerBlock* pBlock = GetLayer(valueType);

            if (pBlock != NULL)
            {
                SetObject(*ppValue, pBlock->GetValue());
            }
            else
            {
                *ppValue = NULL;
            }

            return hr;
        }

        __checkReturn HRESULT ClearValueFromLayer(
            EffectiveValue::Value valueType,
            __out bool* pReplacedEffectiveValue,
            __deref_out_opt CObjectWithType** ppPreviousEffectiveValue
            )
        {
            HRESULT hr = S_OK;

            bool isEffectiveValue = (m_pHeadBlock == NULL || valueType >= m_pHeadBlock->GetValueType());

            if (isEffectiveValue && m_pHeadBlock != NULL)
            {
                SetObject(*ppPreviousEffectiveValue, m_pHeadBlock->GetValue());
            }
            else
            {
                *ppPreviousEffectiveValue = NULL;
            }

            RemoveLayer(valueType);

            *pReplacedEffectiveValue = isEffectiveValue;

            return hr;
        }

    protected:
        __checkReturn HRESULT NewBlock(
            __deref_out CLayerBlock** ppLayerBlock
            )
        {
            HRESULT hr = S_OK;
            CLayerBlock* pBlock = NULL;

            //
            // Linear search to find an empty block.
            //
            for (UINT32 i = 0; i < ARRAYSIZE(m_Blocks); ++i)
            {
                if (m_Blocks[i].GetValueType() == EffectiveValue::None)
                {
                    pBlock = &m_Blocks[i];
                    break;
                }
            }

            if (pBlock == NULL)
            {
                pBlock = new CLayerBlock();
                IFCOOM(pBlock);
            }

            *ppLayerBlock = pBlock;
            
        Cleanup:
            return hr;
        }

        __checkReturn HRESULT NewBlock(
            EffectiveValue::Value valueType,
            __in_opt CLayerBlock* pNextLayer,
            __deref_out CLayerBlock** ppLayerBlock
            )
        {
            HRESULT hr = S_OK;
            CLayerBlock* pBlock = NULL;

            for (UINT32 i = 0; i < ARRAYSIZE(m_Blocks); ++i)
            {
                if (m_Blocks[i].GetValueType() == EffectiveValue::None)
                {
                    pBlock = &m_Blocks[i];
                    break;
                }
            }

            if (pBlock == NULL)
            {
                pBlock = new CLayerBlock(valueType, pNextLayer);
                IFCOOM(pBlock);
            }
            else
            {
                pBlock->SetValueType(valueType);
                pBlock->SetNext(pNextLayer);
            }

            *ppLayerBlock = pBlock;
            
        Cleanup:
            return hr;
        }

        void FreeBlock(
            __in CLayerBlock* pBlock
            )
        {
            if (pBlock >= &m_Blocks[0] && pBlock <= &m_Blocks[ARRAYSIZE(m_Blocks) - 1])
            {
                pBlock->Reset();
            }
            else
            {
                delete pBlock;
            }
        }

        __out_opt CLayerBlock* GetLayer(
            EffectiveValue::Value valueType
            )
        {
            CLayerBlock* pBlock = m_pHeadBlock;
            CLayerBlock* pMatchingBlock = NULL;

            ASSERT(valueType != EffectiveValue::None);

            while (pBlock != NULL)
            {
                if (pBlock->GetValueType() == valueType)
                {
                    pMatchingBlock = pBlock;
                    break;
                }

                pBlock = pBlock->GetNext();
            }

            return pMatchingBlock;
        }

        __checkReturn HRESULT GetOrCreateLayer(
            EffectiveValue::Value valueType,
            __deref_out_opt CLayerBlock** ppBlock
            )
        {
            HRESULT hr = S_OK;
            CLayerBlock* pPreviousBlock = NULL;
            CLayerBlock* pNextBlock = m_pHeadBlock;
            CLayerBlock* pMatchingBlock = NULL;

            ASSERT(valueType != EffectiveValue::None);

            //
            // Find the first block that has a lower priority than the effective value requested.
            //
            while (pNextBlock != NULL && pNextBlock->GetValueType() > valueType)
            {
                pPreviousBlock = pNextBlock;

                pNextBlock = pNextBlock->GetNext();
            }

            //
            // Check if the block found is the priority that should be used.
            //
            if (pNextBlock != NULL && valueType == pNextBlock->GetValueType())
            {
                pMatchingBlock = pNextBlock;
            }

            //
            // If the next block was lower priority and there was no previous highest priority
            // then we need to add a new head block.
            //
            if (pMatchingBlock == NULL && pPreviousBlock == NULL)
            {
                IFC(NewBlock(valueType, pNextBlock, &pMatchingBlock));

                m_pHeadBlock = pMatchingBlock;
            }
            
            //
            // If the next block is lower priority and we have a higher priority previous block,
            // insert a new block in to the chain.
            //
            if (pMatchingBlock == NULL)
            {
                IFC(NewBlock(valueType, pNextBlock, &pMatchingBlock));

                pPreviousBlock->SetNext(pMatchingBlock);
            }

            ASSERT(pMatchingBlock != NULL);

            *ppBlock = pMatchingBlock;

        Cleanup:
            return hr;
        }

        void RemoveLayer(
            EffectiveValue::Value valueType
            )
        {
            CLayerBlock* pBlock = m_pHeadBlock;
            CLayerBlock* pPreviousBlock = NULL;

            ASSERT(valueType != EffectiveValue::None);

            while (pBlock != NULL)
            {
                if (pBlock->GetValueType() == valueType)
                {
                    if (pPreviousBlock != NULL)
                    {
                        pPreviousBlock->SetNext(pBlock->GetNext());
                    }
                    else
                    {
                        m_pHeadBlock = pBlock->GetNext();
                    }

                    FreeBlock(pBlock);

                    break;
                }

                pPreviousBlock = pBlock;
                pBlock = pBlock->GetNext();
            }
        }

    private:
        private:
            CLayeredValue(
                const CLayeredValue&
                );

            CLayeredValue& operator=(
                const CLayeredValue&
                );

        CLayerBlock* m_pHeadBlock;
        CLayerBlock m_Blocks[1];
};

/*
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
            bool IsEffectiveValue = (m_ActualEffectiveValue <= ValueType);
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
            bool IsEffectiveValue = (m_ActualEffectiveValue <= ValueType);
            CObjectWithType* pOldValue = NULL;
            CObjectWithType* pNewValue = NULL;

            IFCPTR(pValue);

            if(IsEffectiveValue)
            {
                SetObject(pOldValue, m_EffectiveValueObject);
            }

            if(pValue)
            {
                if(pValue->IsTypeOf(ObjectTypeTraits< T >::Type))
                {
                    ReplaceObject(Layer.Value, (T*)pValue);
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
                        ReplaceObject(Layer.Value, pConvertedType);
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
                    SetObject(*ppEffectiveValue, Layer.Value);
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
        bool m_IsInvalidated;
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
*/