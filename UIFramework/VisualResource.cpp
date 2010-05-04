#include "VisualResource.h"
#include "ErrorChecking.h"
#include "Visual.h"

HRESULT CVisualResource::OnVisualAttach(CVisualAttachContext& Context)
{
    HRESULT hr = S_OK;

    for(std::vector< VisualParentInfo >::iterator It = m_Parents.begin(); It != m_Parents.end(); ++It)
    {
        if(It->Parent == Context.GetParent())
        {
            It->References++;

            goto Cleanup;
        }
    }

    m_Parents.push_back(Context.GetParent());

Cleanup:
    return hr;
}

HRESULT CVisualResource::OnVisualDetach(CVisualDetachContext& Context)
{
    HRESULT hr = S_OK;

    for(std::vector< VisualParentInfo >::iterator It = m_Parents.begin(); It != m_Parents.end(); ++It)
    {
        if(It->Parent == Context.GetParent())
        {
            It->References--;

            if(It->References == 0)
            {
                m_Parents.erase(It);
            }

            goto Cleanup;
        }
    }

    IFC(E_UNEXPECTED);

Cleanup:
    return hr;
}

UINT32 CVisualResource::GetVisualParentCount()
{
    return m_Parents.size();
}

CVisual* CVisualResource::GetVisualParent(UINT32 Index)
{
    return m_Parents[Index].Parent;
}

HRESULT CVisualResource::NotifyParents(CVisualNotification* pVisualNotification)
{
    HRESULT hr = S_OK;

    for(std::vector< VisualParentInfo >::iterator It = m_Parents.begin(); It != m_Parents.end(); ++It)
    {
        IFC(It->Parent->OnVisualNotification(pVisualNotification));
    }

Cleanup:
    return hr;
}