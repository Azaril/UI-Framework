#include "Image.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"

StaticClassProperty ImageProperties[] =
{
    { L"Source", FALSE, TypeIndex::Object }
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

//HRESULT CImage::SetSource(CBitmapSource* pBitmapSource)
//{
//    HRESULT hr = S_OK;
//
//    ReleaseObject(m_Source);
//
//    m_Source = pBitmapSource;
//    AddRefObject(m_Source);
//
//    IFC(m_ImageBrush->SetSource(m_Source));
//
//Cleanup:
//    return hr;
//}

HRESULT CImage::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    SizeU ImageSize = { 0 };
    SizeF BaseSize = { 0 };

    IFC(CFrameworkElement::MeasureInternal(AvailableSize, BaseSize));

    if(m_ImageBrush != NULL)
    {
        //TODO: Fix this!
        
        __debugbreak();

        //IFC(m_ImageBrush->GetSize(&ImageSize));
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