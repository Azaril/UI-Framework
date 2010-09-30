#include "FocusManager.h"

CFocusManager::CFocusManager() : m_FocusedElement(NULL)
{
}

CFocusManager::~CFocusManager()
{
    ReleaseObject(m_FocusedElement);
}

HRESULT CFocusManager::Initialize()
{
    HRESULT hr = S_OK;

    return hr;
}

HRESULT CFocusManager::GetFocusedElement(CUIElement** ppElement)
{
    HRESULT hr = S_OK;

    IFCPTR(ppElement);

    *ppElement = m_FocusedElement;
    AddRefObject(m_FocusedElement);

Cleanup:
    return hr;
}

HRESULT CFocusManager::SetFocus(CUIElement* pElement, BOOL* pSetFocus)
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
// CUIHost
//
extern "C" __declspec(dllexport)
void CFocusManager_AddRef(CFocusManager* pManager)
{
    pManager->AddRef();
}

extern "C" __declspec(dllexport)
void CFocusManager_Release(CFocusManager* pManager)
{
    pManager->Release();
}

extern "C" __declspec(dllexport)
HRESULT CFocusManager_SetFocus(CFocusManager* pManager, CUIElement* pElement, BOOL* pSetFocus)
{
    return pManager->SetFocus(pElement, pSetFocus);
}

extern "C" __declspec(dllexport)
HRESULT CFocusManager_GetFocusedElement(CFocusManager* pManager, CUIElement** ppElement)
{
    return pManager->GetFocusedElement(ppElement);
}