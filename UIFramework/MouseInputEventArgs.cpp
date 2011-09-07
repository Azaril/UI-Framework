#include "MouseInputEventArgs.h"
#include "Visual.h"

CMouseEventArgs::CMouseEventArgs(
    )
{
}

CMouseEventArgs::~CMouseEventArgs(
    )
{
}

__checkReturn HRESULT
CMouseEventArgs::Initialize(
    __in CRoutedEvent* pRoutedEvent, 
    Point2F MouseLocation
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEvent);

    IFC(CInputEventArgs::Initialize(pRoutedEvent));

    m_Location = MouseLocation;

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CMouseEventArgs::Initialize(
    __in CRoutedEvent* pRoutedEvent,
    __in CMouseEventArgs* pSourceArgs
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEvent);
    IFCPTR(pSourceArgs);

    IFC(CInputEventArgs::Initialize(pRoutedEvent));

    IFC(pSourceArgs->GetLocation(&m_Location));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CMouseEventArgs::GetLocation(
    __out Point2F* pLocation
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pLocation);

    *pLocation = m_Location;

Cleanup:
    return hr;
}

__checkReturn HRESULT
CMouseEventArgs::GetLocation(
    __in CVisual* pVisual, 
    __out Point2F* pLocation
    )
{
    HRESULT hr = S_OK;
    CTransform* pTransform = NULL;

    IFCPTR(pLocation);
    IFCPTR(pVisual);

    IFC(pVisual->TransformToAncestor(NULL, &pTransform));

    IFC(pTransform->TransformPoint(m_Location, pLocation));

Cleanup:
    ReleaseObject(pTransform);

    return hr;
}

CMouseButtonEventArgs::CMouseButtonEventArgs(
    )
{
}

CMouseButtonEventArgs::~CMouseButtonEventArgs(
    )
{
}

__checkReturn HRESULT 
CMouseButtonEventArgs::Initialize(
    __in CRoutedEvent* pRoutedEvent, 
    Point2F MouseLocation, 
    MouseButton::Value Button, 
    MouseButtonState::Value ButtonState
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEvent);

    IFC(CMouseEventArgs::Initialize(pRoutedEvent, MouseLocation));

    m_Button = Button;
    m_ButtonState = ButtonState;

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CMouseButtonEventArgs::Initialize(
    __in CRoutedEvent* pRoutedEvent, 
    __in CMouseButtonEventArgs* pSourceArgs
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pRoutedEvent);
    IFCPTR(pSourceArgs);

    IFC(CMouseEventArgs::Initialize(pRoutedEvent, pSourceArgs));

    m_Button = pSourceArgs->GetChangedButton();
    m_ButtonState = pSourceArgs->GetButtonState();

Cleanup:
    return hr;
}

MouseButton::Value 
CMouseButtonEventArgs::GetChangedButton(
    )
{
    return m_Button;
}

MouseButtonState::Value 
CMouseButtonEventArgs::GetButtonState(
    )
{
    return m_ButtonState;
}

//
// CMouseEventArgs
//
extern "C" __declspec(dllexport)
TypeIndex::Value 
CMouseEventArgs_TypeIndex()
{
    return TypeIndex::MouseEventArgs;
}

extern "C" __declspec(dllexport)
__out CInputEventArgs* 
CMouseEventArgs_CastTo_CInputEventArgs(
    __in CMouseEventArgs* pArgs
    )
{
    return pArgs;
}

extern "C" __declspec(dllexport)
__out_opt CMouseEventArgs* 
CObjectWithType_CastTo_CMouseEventArgs(
    __in CObjectWithType* pObject
    )
{
    return (pObject->IsTypeOf(TypeIndex::MouseEventArgs)) ? (CMouseEventArgs*)pObject : NULL;
}

//
// CMouseButtonEventArgs
//
extern "C" __declspec(dllexport)
TypeIndex::Value 
CMouseButtonEventArgs_TypeIndex(
    )
{
    return TypeIndex::MouseButtonEventArgs;
}

extern "C" __declspec(dllexport)
__out CMouseEventArgs* 
CMouseButtonEventArgs_CastTo_CMouseEventArgs(
    __in CMouseButtonEventArgs* pArgs
    )
{
    return pArgs;
}

extern "C" __declspec(dllexport)
__out_opt CMouseButtonEventArgs* 
CObjectWithType_CastTo_CMouseButtonEventArgs(
    __in CObjectWithType* pObject
    )
{
    return (pObject->IsTypeOf(TypeIndex::MouseButtonEventArgs)) ? (CMouseButtonEventArgs*)pObject : NULL;
}