#include "ContentControl.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

//
// Properties
//
namespace ContentControlProperties
{
    enum Value
    {
        Content
    };
}

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Content );

//
// Properties
//
CStaticProperty CContentControl::ContentProperty(ContentControlProperties::Content, L"Content", TypeIndex::Object, StaticPropertyFlags::Content, GET_DEFAULT( Content ), &INSTANCE_CHANGE_CALLBACK( CContentControl, OnContentChanged ) );

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

    IFC(m_Content.GetEffectiveValue(ppContent));

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

__override __checkReturn HRESULT 
CContentControl::GetLayeredValue(
    __in CProperty* pProperty,
    __deref_out CLayeredValue** ppLayeredValue
    )
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    if (pProperty->GetType() == TypeIndex::ContentControl)
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        switch(pStaticProperty->GetLocalIndex())
        {
            case ContentControlProperties::Content:
                {
                    *ppLayeredValue = &m_Content;
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
        IFC_NOTRACE(CControl::GetLayeredValue(pProperty, ppLayeredValue));
    }

Cleanup:
    return hr;
}

//
// CContentControl
//
extern "C" UIFRAMEWORK_API
TypeIndex::Value CContentControl_TypeIndex()
{
    return TypeIndex::ContentControl;
}

extern "C" UIFRAMEWORK_API
CControl* CContentControl_CastTo_CControl(CContentControl* pContentControl)
{
    return pContentControl;
}

extern "C" UIFRAMEWORK_API
CContentControl* CObjectWithType_CastTo_CContentControl(CObjectWithType* pObject)
{
    return (pObject->IsTypeOf(TypeIndex::ContentControl)) ? (CContentControl*)pObject : NULL;
}