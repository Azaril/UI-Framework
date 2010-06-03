#pragma once

#include "VisualContext.h"
#include "VisualNotification.h"
#include "Collections.h"

class CVisualResource
{
    public:
        virtual INT32 AddRef() = 0;
        virtual INT32 Release() = 0;

        virtual HRESULT OnVisualAttach( CVisualAttachContext& Context ) = 0;
        virtual HRESULT OnVisualDetach( CVisualDetachContext& Context ) = 0;

        UINT32 GetVisualParentCount();
        CVisual* GetVisualParent( UINT32 Index );

        HRESULT NotifyParents( CVisualNotification* pVisualNotification );

    private:
        struct VisualParentInfo
        {
            VisualParentInfo(CVisual* pParent) : Parent(pParent),
                                                 References(1)
            {
            }

            CVisual* Parent;
            UINT32 References;
        };

        std::vector< VisualParentInfo > m_Parents;
};