#pragma once

#include "PropertyObject.h"
#include "VisualContext.h"
#include "Collections.h"

class CVisualResource : public CRefCountedObjectBase< CPropertyObject >
{
    public:
        virtual HRESULT OnVisualAttach(
            CVisualAttachContext& Context
            ) = 0;

        virtual HRESULT OnVisualDetach( 
            CVisualDetachContext& Context 
            ) = 0;

    protected:
        __checkReturn HRESULT InvalidateVisualResource(
            );

    private:
        struct VisualParentInfo
        {
            VisualParentInfo(
                __in CVisual* pParent, 
                __in_opt OnValueChangeFunc pChangeCallback
                ) 
                : Parent(pParent)
                , ChangeCallback(pChangeCallback)
                , References(1)
            {
            }

            CVisual* Parent;
            OnValueChangeFunc ChangeCallback;
            INT32 References;
        };

        std::vector< VisualParentInfo > m_Parents;
};