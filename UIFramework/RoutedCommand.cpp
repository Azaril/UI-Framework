#include "RoutedCommand.h"
#include "UIElement.h"
#include "RoutedCommandEventArgs.h"
#include "RoutedEventInformation.h"

//
// Events
//
CStaticRoutedEvent< RoutingStrategy::Bubbling > CRoutedCommand::ExecutedEvent(L"Executed");

CRoutedCommand::CRoutedCommand()
{
}

CRoutedCommand::~CRoutedCommand()
{
}

HRESULT CRoutedCommand::Execute(CUIElement* pSender)
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

HRESULT CRoutedCommand::CreateEventInformation(CEventInformation** ppInformation)
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