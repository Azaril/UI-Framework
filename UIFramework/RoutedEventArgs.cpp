#include "RoutedEventArgs.h"

CRoutedEventArgs::CRoutedEventArgs(
    ) 
    : m_RoutedEvent(NULL)
    , m_Handled(FALSE)
{
}

CRoutedEventArgs::~CRoutedEventArgs(
    )
{
    ReleaseObject(m_RoutedEvent);
}

__checkReturn HRESULT 
CRoutedEventArgs::Initialize(
    __in CRoutedEvent* pRoutedEvent
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEvent);

    m_RoutedEvent = pRoutedEvent;
    AddRefObject(m_RoutedEvent);

Cleanup:
    return hr;
}

__out CRoutedEvent* 
CRoutedEventArgs::GetRoutedEvent(
    )
{
    return m_RoutedEvent;
}

BOOL 
CRoutedEventArgs::IsHandled(
    )
{
    return m_Handled;
}

void
CRoutedEventArgs::SetHandled(
    BOOL Handled
    )
{
    m_Handled = Handled;
}

//
// CRoutedEventArgs
//
extern "C" __declspec(dllexport)
TypeIndex::Value
CRoutedEventArgs_TypeIndex(
    )
{
    return TypeIndex::RoutedEventArgs;
}

extern "C" __declspec(dllexport)
__out CObjectWithType* 
CRoutedEventArgs_CastTo_CObjectWithType(
    __in CRoutedEventArgs* pArgs
    )
{
    return pArgs;
}

extern "C" __declspec(dllexport)
__out_opt CRoutedEventArgs*
CObjectWithType_CastTo_CRoutedEventArgs(
    __in CObjectWithType* pObject
    )
{
    return (pObject->IsTypeOf(TypeIndex::RoutedEventArgs)) ? (CRoutedEventArgs*)pObject : NULL;
}

extern "C" __declspec(dllexport)
BOOL 
CRoutedEventArgs_IsHandled(
    __in CRoutedEventArgs* pArgs
    )
{
    return pArgs->IsHandled();
}

extern "C" __declspec(dllexport)
void 
CRoutedEventArgs_SetHandled(
    __in CRoutedEventArgs* pArgs, 
    BOOL Handled
    )
{
    pArgs->SetHandled(Handled);
}