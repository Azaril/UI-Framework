#include "ButtonBase.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "RoutedEventInformation.h"
#include "DelegatingRoutedEventInformation.h"

//
// Properties
//
namespace ButtonBaseProperties
{
    enum Value
    {
        Command
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Command );

//
// Properties
//
CStaticProperty CButtonBase::CommandProperty(ButtonBaseProperties::Command, L"Command", TypeIndex::Command, StaticPropertyFlags::None, &GET_DEFAULT( Command ), &INSTANCE_CHANGE_CALLBACK( CButtonBase, OnCommandChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CButtonBase, OnCommandChanged );

//
// Events
//
CStaticRoutedEvent CButtonBase::ClickEvent(L"Click", RoutingStrategy::Bubbling);

CButtonBase::CButtonBase() : m_Command(this, &CButtonBase::CommandProperty)
{
}

CButtonBase::~CButtonBase()
{
}

void CButtonBase::OnMouseLeftButtonDown(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    CContentControl::OnMouseLeftButtonDown(pSender, pRoutedEventArgs);

    IFC(CaptureMouse());

Cleanup:
    ;
}

void CButtonBase::OnMouseLeftButtonUp(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    if(!pRoutedEventArgs->IsHandled())
    {
        IFC(RaiseClick());

        pRoutedEventArgs->SetHandled();
    }

    IFC(ReleaseMouse());

Cleanup:
    ;
}

HRESULT CButtonBase::GetEffectiveCommand(CCommand** ppCommand)
{
    HRESULT hr = S_OK;

    IFCPTR(ppCommand);

    IFC(m_Command.GetTypedEffectiveValue(ppCommand));

Cleanup:
    return hr;
}

HRESULT CButtonBase::RaiseClick()
{
    HRESULT hr = S_OK;
    CRoutedEventArgs* pClickEventArgs = NULL;
    CCommand* pCommand = NULL;

    IFC(CRoutedEventArgs::Create(&ClickEvent, &pClickEventArgs));

    IFC(RaiseEvent(pClickEventArgs));

    if(!pClickEventArgs->IsHandled())
    {
        IFC(GetEffectiveCommand(&pCommand));

        if(pCommand)
        {
            IFC(pCommand->Execute(this));
        }
    }

Cleanup:
    ReleaseObject(pClickEventArgs);
    ReleaseObject(pCommand);

    return hr;
}

HRESULT CButtonBase::OnCommandChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    //TODO: Update if command can execute and disable UI or not.

    return hr;
}

HRESULT CButtonBase::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &CommandProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CContentControl::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

    return hr;
}

HRESULT CButtonBase::CreateEventInformation(CEventInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CRoutedEventInformation* pEventInformation = NULL;
    CEventInformation* pBaseInformation = NULL;
    CDelegatingRoutedEventInformation* pDelegatingEventInformation = NULL;

    CRoutedEvent* Events[] = 
    {
        &ClickEvent
    };
    
    IFCPTR(ppInformation);

    IFC(CRoutedEventInformation::Create(Events, ARRAYSIZE(Events), &pEventInformation));
    IFC(CContentControl::CreateEventInformation(&pBaseInformation));
    IFC(CDelegatingRoutedEventInformation::Create(pEventInformation, pBaseInformation, &pDelegatingEventInformation));

    *ppInformation = pDelegatingEventInformation;
    pDelegatingEventInformation = NULL;

Cleanup:
    ReleaseObject(pEventInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingEventInformation);

    return hr;
}

__override __checkReturn HRESULT 
CButtonBase::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetType() == TypeIndex::ButtonBase)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case ButtonBaseProperties::Command:
                {
                    *ppLayeredValue = &m_Command;
                    break;
                }

            default:
                {
                    IFC(E_UNEXPECTED);
                }
        }
    }
    else
    {
        IFC_NOTRACE(CContentControl::GetLayeredValue(pProperty, ppLayeredValue));
    }

Cleanup:
    return hr;
}

//
// CButtonBase
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value CButtonBase_TypeIndex()
{
    return TypeIndex::ButtonBase;
}

extern "C" UIFRAMEWORK_API
CContentControl* CButtonBase_CastTo_CContentControl(CButtonBase* pButtonBase)
{
    return pButtonBase;
}

extern "C" UIFRAMEWORK_API
CButtonBase* CObjectWithType_CastTo_CButtonBase(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::ButtonBase)) ? (CButtonBase*)pObject : NULL;
}

extern "C" UIFRAMEWORK_API
CRoutedEvent* CButtonBase_GetClickEvent()
{
    return &CButtonBase::ClickEvent;
}