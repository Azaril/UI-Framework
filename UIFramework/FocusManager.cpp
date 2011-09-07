#include "FocusManager.h"

CFocusManager::CFocusManager(
    ) 
    : m_FocusedElement(NULL)
{
}

CFocusManager::~CFocusManager(
    )
{
    ReleaseObject(m_FocusedElement);
}

__checkReturn HRESULT 
CFocusManager::Initialize(
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__checkReturn HRESULT
CFocusManager::GetFocusedElement(
    __deref_out_opt CUIElement** ppElement
    )
{
    HRESULT hr = S_OK;

    IFCPTR(ppElement);

    *ppElement = m_FocusedElement;
    AddRefObject(m_FocusedElement);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CFocusManager::SetFocus(
    __in CUIElement* pElement, 
    __out BOOL* pSetFocus
    )
{
    HRESULT hr = S_OK;
    CRoutedEventArgs* pPreviewLostFocusEventArgs = NULL;
    CRoutedEventArgs* pPreviewGotFocusEventArgs = NULL;
    CRoutedEventArgs* pLostFocusEvent = NULL;
    CRoutedEventArgs* pGotFocusEvent = NULL;
    BOOL Abort = FALSE;

    if(pElement != m_FocusedElement)
    {
        if(pElement)
        {
            Abort = !pElement->IsFocusable();
        }

        if(!Abort && m_FocusedElement)
        {
            IFC(CRoutedEventArgs::Create(&CUIElement::PreviewLostFocusEvent, &pPreviewLostFocusEventArgs));

            IFC(m_FocusedElement->RaiseEvent(pPreviewLostFocusEventArgs));

            Abort = pPreviewLostFocusEventArgs->IsHandled();
        }

        if(!Abort && pElement)
        {
            IFC(CRoutedEventArgs::Create(&CUIElement::PreviewGotFocusEvent, &pPreviewGotFocusEventArgs));

            IFC(pElement->RaiseEvent(pPreviewGotFocusEventArgs));

            Abort = pPreviewGotFocusEventArgs->IsHandled();
        }

        if(!Abort)
        {
            if(m_FocusedElement)
            {
                IFC(CRoutedEventArgs::Create(&CUIElement::LostFocusEvent, &pLostFocusEvent));

                IFC(m_FocusedElement->RaiseEvent(pLostFocusEvent));

                ReleaseObject(m_FocusedElement);
            }

            m_FocusedElement = pElement;

            if(m_FocusedElement)
            {
                AddRefObject(m_FocusedElement);

                IFC(CRoutedEventArgs::Create(&CUIElement::GotFocusEvent, &pGotFocusEvent));

                IFC(m_FocusedElement->RaiseEvent(pGotFocusEvent));
            }
        }
    }

    if(pSetFocus)
    {
        *pSetFocus = !Abort;
    }

Cleanup:
    ReleaseObject(pPreviewLostFocusEventArgs);
    ReleaseObject(pPreviewGotFocusEventArgs);
    ReleaseObject(pLostFocusEvent);
    ReleaseObject(pGotFocusEvent);

    return hr;
}

//
// CFocusManager
//
extern "C" __declspec(dllexport)
void
CFocusManager_AddRef(
    __in CFocusManager* pManager
    )
{
    pManager->AddRef();
}

extern "C" __declspec(dllexport)
void 
CFocusManager_Release(
    __in CFocusManager* pManager
    )
{
    pManager->Release();
}

extern "C" __declspec(dllexport)
__checkReturn HRESULT 
CFocusManager_SetFocus(
    __in CFocusManager* pManager, 
    __in CUIElement* pElement, 
    __out BOOL* pSetFocus
    )
{
    return pManager->SetFocus(pElement, pSetFocus);
}

extern "C" __declspec(dllexport)
__checkReturn HRESULT
CFocusManager_GetFocusedElement(
    __in CFocusManager* pManager, 
    __deref_out_opt CUIElement** ppElement
    )
{
    return pManager->GetFocusedElement(ppElement);
}