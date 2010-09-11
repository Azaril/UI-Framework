#include "ContentControl.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Content );

//
// Properties
//
CStaticProperty CContentControl::ContentProperty( L"Content", TypeIndex::Object, StaticPropertyFlags::Content, GET_DEFAULT( Content ), &INSTANCE_CHANGE_CALLBACK( CContentControl, OnContentChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CContentControl, OnContentChanged );

CContentControl::CContentControl() : m_Content(this, &CContentControl::ContentProperty)
{
}

CContentControl::~CContentControl()
{
}

HRESULT CContentControl::OnContentChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    return hr;
}

HRESULT CContentControl::GetEffectiveContent(CObjectWithType** ppContent)
{
    HRESULT hr = S_OK;

    IFCPTR(ppContent);

    IFC(m_Content.GetEffectiveValue(GetProviders(), ppContent));

Cleanup:
    return hr;
}

HRESULT CContentControl::CreatePropertyInformation(CPropertyInformation **ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &ContentProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CControl::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

    return hr;
}

HRESULT CContentControl::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CContentControl::ContentProperty)
    {
        *ppLayeredValue = &m_Content;
    }
    else
    {
        hr = CControl::GetLayeredValue(pProperty, ppLayeredValue);
    }

Cleanup:
    return hr;
}

//
// CContentControl
//
extern "C" __declspec(dllexport)
TypeIndex::Value CContentControl_TypeIndex()
{
    return TypeIndex::ContentControl;
}

extern "C" __declspec(dllexport)
CControl* CContentControl_CastTo_CControl(CContentControl* pContentControl)
{
    return pContentControl;
}

extern "C" __declspec(dllexport)
CContentControl* CObjectWithType_CastTo_CContentControl(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::ContentControl)) ? (CContentControl*)pObject : NULL;
}