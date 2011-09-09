#include "DelegatingCommandInformation.h"

CDelegatingCommandInformation::CDelegatingCommandInformation(
    ) 
    : m_Root(NULL)
    , m_Base(NULL)
{
}

CDelegatingCommandInformation::~CDelegatingCommandInformation(
    )
{
    ReleaseObject(m_Root);
    ReleaseObject(m_Base);
}

__checkReturn HRESULT 
CDelegatingCommandInformation::Initialize(
    __in CCommandInformation* pRoot,
    __in CCommandInformation* pBase
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pRoot);
    IFCPTR(pBase);

    m_Root = pRoot;
    AddRefObject(m_Root);

    m_Base = pBase;
    AddRefObject(m_Base);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CDelegatingCommandInformation::GetCommand(
    __in_z const WCHAR* pCommandName, 
    __deref_out_opt CCommand** ppCommand
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pCommandName);
    IFCPTR(ppCommand);

    if(FAILED(m_Root->GetCommand(pCommandName, ppCommand)))
    {
        IFC(m_Base->GetCommand(pCommandName, ppCommand));
    }

Cleanup:
    return hr;
}