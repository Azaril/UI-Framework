#include "RoutedCommand.h"
#include "UIElement.h"
#include "RoutedCommandEventArgs.h"
#include "RoutedEventInformation.h"

//
// Events
//
CStaticRoutedEvent CRoutedCommand::ExecutedEvent(L"Executed", RoutingStrategy::Bubbling);

CRoutedCommand::CRoutedCommand(
    )
{
}

CRoutedCommand::~CRoutedCommand(
    )
{
}

__override __checkReturn HRESULT 
CRoutedCommand::Execute(
    __in CUIElement* pSender
    )
{
    HRESULT hr = S_OK;
    CRoutedCommandEventArgs* pRoutedEventArgs = NULL;

    IFCPTR(pSender);

    IFC(CRoutedCommandEventArgs::Create(&ExecutedEvent, this, &pRoutedEventArgs));

    IFC(pSender->RaiseEvent(pRoutedEventArgs));

Cleanup:
    ReleaseObject(pRoutedEventArgs);

    return hr;
}

__checkReturn HRESULT
CRoutedCommand::CreateEventInformation(CEventInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CRoutedEventInformation* pEventInformation = NULL;

    CRoutedEvent* Events[] = 
    {
        &ExecutedEvent
    };
    
    IFCPTR(ppInformation);

    IFC(CRoutedEventInformation::Create(Events, ARRAYSIZE(Events), &pEventInformation));

    *ppInformation = pEventInformation;
    pEventInformation = NULL;

Cleanup:
    ReleaseObject(pEventInformation);

    return hr;
}