#include "Image.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

StaticClassProperty ImageProperties[] =
{
    { L"Source", TypeIndex::Object, StaticPropertyFlags::None }
};

StaticClassProperties ImagePropertyInformation =
{
    ImageProperties,
    ARRAYSIZE(ImageProperties)
};

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

HRESULT CImage::Arrange(SizeF Size)
{
    HRESULT hr = S_OK;

    IFC(m_ImageRect->SetSize(Size));

    IFC(CFrameworkElement::Arrange(Size));

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

    IFC(CStaticPropertyInformation::Create(&ImagePropertyInformation, &pStaticInformation));
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

    //TODO: Ensure this property actually belongs to this object.

    //TODO: Looking up other than by name would be much better.
    if(wcscmp(pProperty->GetName(), L"Source") == 0)
    {
        IFC(InternalSetSource(pValue));
    }
    else if(wcscmp(pProperty->GetName(), L"Padding") == 0)
    {
        IFCEXPECT(pValue->IsTypeOf(TypeIndex::RectF));
        //TODO: Implement.

        __debugbreak();

        IFC(E_FAIL);
    }
    else
    {
        IFC(CFrameworkElement::SetValue(pProperty, pValue));
    }

Cleanup:
    return hr;
}