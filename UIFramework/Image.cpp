#include "Image.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

CStaticProperty ImageProperties[] = 
{
    CStaticProperty( L"Source", TypeIndex::Object, StaticPropertyFlags::None )
};

namespace ImagePropertyIndex
{
    enum Value
    {
        Source
    };
}

CImage::CImage() : m_Source(NULL),
                   m_ImageRect(NULL),
                   m_ImageBrush(NULL)
{
}

CImage::~CImage()
{
    Finalize();
}

HRESULT CImage::Initialize()
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::Initialize());

    IFC(CRectangleVisual::Create(&m_ImageRect));

    IFC(AddChildVisual(m_ImageRect));

    IFC(CImageBrush::Create(&m_ImageBrush));

    IFC(m_ImageRect->SetBrush(m_ImageBrush));

Cleanup:
    return hr;
}

HRESULT CImage::Finalize()
{
    HRESULT hr = S_OK;

    if(m_ImageRect)
    {
        IFC(RemoveChildVisual(m_ImageRect));

        ReleaseObject(m_ImageRect);
    }

    ReleaseObject(m_Source);

    ReleaseObject(m_ImageBrush);

Cleanup:
    return hr;
}

HRESULT CImage::SetSource(CObjectWithType* pSource)
{
    HRESULT hr = S_OK;

    IFC(InternalSetSource(pSource));

Cleanup:
    return hr;
}

HRESULT CImage::InternalSetSource(CObjectWithType* pSource)
{
    HRESULT hr = S_OK;

    IFC(m_ImageBrush->SetSource(pSource));

Cleanup:
    return hr;
}

HRESULT CImage::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    SizeU ImageSize = { 0 };
    SizeF BaseSize = { 0 };

    IFC(CFrameworkElement::MeasureInternal(AvailableSize, BaseSize));

    if(m_ImageBrush != NULL)
    {
        IFC(m_ImageBrush->GetSize(&ImageSize));
    }

    DesiredSize.width = max((FLOAT)ImageSize.width, BaseSize.width);
    DesiredSize.height = max((FLOAT)ImageSize.height, BaseSize.height);

Cleanup:
    return hr;
}

HRESULT CImage::ArrangeInternal(SizeF Size)
{
    HRESULT hr = S_OK;

    IFC(m_ImageRect->SetSize(Size));

    IFC(CFrameworkElement::ArrangeInternal(Size));

Cleanup:
    return hr;
}

HRESULT CImage::CreatePropertyInformation(CPropertyInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingProperyInformation = NULL;

    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(ImageProperties, ARRAYSIZE(ImageProperties), &pStaticInformation));
    IFC(CFrameworkElement::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingProperyInformation));

    *ppInformation = pDelegatingProperyInformation;
    pDelegatingProperyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingProperyInformation);

    return hr;
}

HRESULT CImage::SetValue(CProperty* pProperty, CObjectWithType* pValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(pValue);

    // Check if the property is a static property.
    if(pProperty >= ImageProperties && pProperty < ImageProperties + ARRAYSIZE(ImageProperties))
    {
        CStaticProperty* pStaticProperty = (CStaticProperty*)pProperty;

        UINT32 Index = (pStaticProperty - ImageProperties);
        
        switch(Index)
        {
            case ImagePropertyIndex::Source:
                {
                    IFC(InternalSetSource(pValue));

                    break;
                }

            default:
                {
                    IFC(E_FAIL);
                }
        }
    }
    else
    {
        IFC(CFrameworkElement::SetValue(pProperty, pValue));
    }

Cleanup:
    return hr;
}