#include "VisualResource.h"
#include "ErrorChecking.h"
#include "Visual.h"

__checkReturn HRESULT 
CVisualResource::OnVisualAttach(
    CVisualAttachContext& Context
    )
{
    HRESULT hr = S_OK;
    BOOL AddedReference = FALSE;

    for(std::vector< VisualParentInfo >::iterator It = m_Parents.begin(); It != m_Parents.end(); ++It)
    {
        if(It->Parent == Context.GetParent() && It->ChangeCallback == Context.GetChangeCallback())
        {
            It->References++;

            AddedReference = TRUE;

            break;
        }
    }

    if (!AddedReference)
    {
        m_Parents.push_back(VisualParentInfo(Context.GetParent(), Context.GetChangeCallback()));
    }

    if (Context.GetParent() != NULL && Context.GetChangeCallback() != NULL)
    {
        IFC(Context.GetChangeCallback()(Context.GetParent(), NULL, this));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CVisualResource::OnVisualDetach(
    CVisualDetachContext& Context
    )
{
    HRESULT hr = S_OK;

    for(std::vector< VisualParentInfo >::iterator It = m_Parents.begin(); It != m_Parents.end(); ++It)
    {
        if(It->Parent == Context.GetParent() && It->ChangeCallback == Context.GetChangeCallback())
        {
            It->References--;

            if(It->References == 0)
            {
                m_Parents.erase(It);
            }

            if (Context.GetParent() != NULL && Context.GetChangeCallback() != NULL)
            {
                IFC(Context.GetChangeCallback()(Context.GetParent(), this, NULL));
            }

            goto Cleanup;
        }
    }

    IFC(E_UNEXPECTED);

Cleanup:
    return hr;
}

__checkReturn HRESULT
CVisualResource::InvalidateVisualResource(
    )
{
    HRESULT hr = S_OK;

    for(std::vector< VisualParentInfo >::iterator It = m_Parents.begin(); It != m_Parents.end(); ++It)
    {
        if (It->Parent != NULL && It->ChangeCallback != NULL)
        {
            IFC(It->ChangeCallback(It->Parent, this, this));
        }
    }

Cleanup:
    return hr;
}