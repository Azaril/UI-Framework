#pragma once

#include "PropertyObject.h"
#include "Providers.h"
#include "Noncopyable.h"
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
            DISABLE_COPY_AND_ASSIGN( CLayerBlock );

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
        DISABLE_COPY_AND_ASSIGN( CLayeredValue );

        CLayerBlock* m_pHeadBlock;
        CLayerBlock m_Blocks[1];
};