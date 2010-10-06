#include "Image.h"
#include "StaticPropertyInformation.h"
#include "DelegatingPropertyInformation.h"
#include "BasicTypes.h"

//
// Property Defaults
//
DEFINE_GET_DEFAULT_NULL( Source );
DEFINE_GET_DEFAULT( Stretch, CStretchValue, Stretch::Uniform );
DEFINE_GET_DEFAULT( StretchDirection, CStretchDirectionValue, StretchDirection::Both );

//
// Properties
//
CStaticProperty CImage::SourceProperty( L"Source", TypeIndex::Object, StaticPropertyFlags::None, &GET_DEFAULT( Source ), &INSTANCE_CHANGE_CALLBACK( CImage, OnSourceChanged ) );
CStaticProperty CImage::StretchProperty( L"Stretch", TypeIndex::Stretch, StaticPropertyFlags::None, &GET_DEFAULT( Stretch ), &INSTANCE_CHANGE_CALLBACK( CImage, OnStretchChanged ) );
CStaticProperty CImage::StretchDirectionProperty( L"StretchDirection", TypeIndex::StretchDirection, StaticPropertyFlags::None, &GET_DEFAULT( StretchDirection ), &INSTANCE_CHANGE_CALLBACK( CImage, OnStretchDirectionChanged ) );

//
// Property Change Handlers
//
DEFINE_INSTANCE_CHANGE_CALLBACK( CImage, OnSourceChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CImage, OnStretchChanged );
DEFINE_INSTANCE_CHANGE_CALLBACK( CImage, OnStretchDirectionChanged );

CImage::CImage() : m_Source(this, &CImage::SourceProperty),
                   m_Stretch(this, &CImage::StretchProperty),
                   m_StretchDirection(this, &CImage::StretchDirectionProperty),
                   m_ImageVisual(NULL),
                   m_ImageBrush(NULL),
                   m_GeometryDirty(TRUE)
{
}

CImage::~CImage()
{
    Finalize();
}

HRESULT CImage::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::Initialize(pProviders));

    IFC(CGeometryVisual::Create(&m_ImageVisual));

    IFC(AddChildVisual(m_ImageVisual));

    IFC(CImageBrush::Create(pProviders, &m_ImageBrush));

    IFC(m_ImageVisual->SetFillBrush(m_ImageBrush));

Cleanup:
    return hr;
}

HRESULT CImage::Finalize()
{
    HRESULT hr = S_OK;

    if(m_ImageVisual)
    {
        IFC(RemoveChildVisual(m_ImageVisual));

        ReleaseObject(m_ImageVisual);
    }

    ReleaseObject(m_ImageBrush);

Cleanup:
    return hr;
}

HRESULT CImage::SetSource(const WCHAR* pSource)
{
    HRESULT hr = S_OK;
    CStringValue* pStringValue = NULL;

    IFCPTR(pSource);

    IFC(CStringValue::Create(pSource, &pStringValue));

    IFC(SetValue(&CImage::SourceProperty, pStringValue));

Cleanup:
    ReleaseObject(pStringValue);

    return hr;
}

HRESULT CImage::SetSource(CObjectWithType* pSource)
{
    HRESULT hr = S_OK;

    IFC(SetValue(&CImage::SourceProperty, pSource));

Cleanup:
    return hr;
}

HRESULT CImage::OnSourceChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    IFC(m_ImageBrush->SetSource(pNewValue));

    m_GeometryDirty = TRUE;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CImage::OnStretchChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    m_GeometryDirty = TRUE;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CImage::OnStretchDirectionChanged(CObjectWithType* pOldValue, CObjectWithType* pNewValue)
{
    HRESULT hr = S_OK;

    m_GeometryDirty = TRUE;

    IFC(InvalidateMeasure());

Cleanup:
    return hr;
}

HRESULT CImage::GetEffectiveSource(CObjectWithType** ppSource)
{
    HRESULT hr = S_OK;

    IFCPTR(ppSource);

    IFC(m_Source.GetEffectiveValue(ppSource));

Cleanup:
    return hr;
}

HRESULT CImage::GetEffectiveStretch(Stretch::Value* pStretch)
{
    HRESULT hr = S_OK;
    CStretchValue* pEffectiveValue = NULL;

    IFCPTR(pStretch);

    IFC(m_Stretch.GetTypedEffectiveValue(&pEffectiveValue));

    *pStretch = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CImage::GetEffectiveStretchDirection(StretchDirection::Value* pStretchDirection)
{
    HRESULT hr = S_OK;
    CStretchDirectionValue* pEffectiveValue = NULL;

    IFCPTR(pStretchDirection);

    IFC(m_StretchDirection.GetTypedEffectiveValue(&pEffectiveValue));

    *pStretchDirection = pEffectiveValue->GetValue();

Cleanup:
    ReleaseObject(pEffectiveValue);

    return hr;
}

HRESULT CImage::OnAttach(CUIAttachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CFrameworkElement::OnAttach(Context));

    m_GeometryDirty = TRUE;

Cleanup:
    return hr;
}

HRESULT CImage::OnDetach(CUIDetachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(ReleaseGeometry());

    IFC(CFrameworkElement::OnDetach(Context));

Cleanup:
    return hr;
}

HRESULT CImage::RebuildGeometry()
{
    HRESULT hr = S_OK;
    CRectangleGeometry* pRectangleGeometry = NULL;
    SizeF FinalSize = GetFinalSize();
    SizeU ImageSize;
    RectF Rectangle(0, 0, FinalSize.width, FinalSize.height);

    if(m_ImageBrush != NULL)
    {
        IFC(m_VisualContext.GetGraphicsDevice()->CreateRectangleGeometry(Rectangle, &pRectangleGeometry));

        IFC(m_ImageVisual->SetGeometry(pRectangleGeometry));

        IFC(m_ImageBrush->GetSize(&ImageSize));

        if(ImageSize.width > 0 && ImageSize.height > 0)
        {
            Matrix3X2F Transform = Matrix3X2F::Scale(1.0f / ImageSize.width, 1.0f / ImageSize.height);

            IFC(m_ImageVisual->SetFillBrushTransform(Transform));
        }
    }
    else
    {
        IFC(m_ImageVisual->SetGeometry(NULL));
    }

Cleanup:
    ReleaseObject(pRectangleGeometry);

    return hr;
}

HRESULT CImage::ReleaseGeometry()
{
    HRESULT hr = S_OK;

    IFC(m_ImageVisual->SetGeometry(NULL));

Cleanup:
    return hr;
}

HRESULT CImage::PreRenderInternal(CPreRenderContext& Context)
{
    HRESULT hr = S_OK;

    if(m_GeometryDirty)
    {
        m_GeometryDirty = FALSE;

        IFC(RebuildGeometry());
    }

    IFC(CFrameworkElement::PreRenderInternal(Context));

Cleanup:
    return hr;
}

//internal static Size ComputeScaleFactor(Size availableSize, 
//                                                Size contentSize, 
//                                                Stretch stretch, 
//                                                StretchDirection stretchDirection)
//        {
//            // Compute scaling factors to use for axes
//            double scaleX = 1.0;
//            double scaleY = 1.0;
//
//            bool isConstrainedWidth = !Double.IsPositiveInfinity(availableSize.Width);
//            bool isConstrainedHeight = !Double.IsPositiveInfinity(availableSize.Height);
//
//           if (     (stretch == Stretch.Uniform || stretch == Stretch.UniformToFill || stretch == Stretch.Fill)
//                &&  (isConstrainedWidth || isConstrainedHeight) )
//            {
//                // Compute scaling factors for both axes
//                scaleX = (DoubleUtil.IsZero(contentSize.Width)) ? 0.0 : availableSize.Width / contentSize.Width;
//                scaleY = (DoubleUtil.IsZero(contentSize.Height)) ? 0.0 : availableSize.Height / contentSize.Height;
//
//                if (!isConstrainedWidth)        scaleX = scaleY;
//                else if (!isConstrainedHeight)  scaleY = scaleX;
//                else 
//                {
//                    // If not preserving aspect ratio, then just apply transform to fit
//                    switch (stretch) 
//                    {
//                        case Stretch.Uniform:       //Find minimum scale that we use for both axes
//                            double minscale = scaleX < scaleY ? scaleX : scaleY;
//                            scaleX = scaleY = minscale;
//                            break;
//
//                        case Stretch.UniformToFill: //Find maximum scale that we use for both axes
//                            double maxscale = scaleX > scaleY ? scaleX : scaleY;
//                            scaleX = scaleY = maxscale;
//                            break;
//
//                        case Stretch.Fill:          //We already computed the fill scale factors above, so just use them
//                            break;
//                    }
//                }
//
//                //Apply stretch direction by bounding scales.
//                //In the uniform case, scaleX=scaleY, so this sort of clamping will maintain aspect ratio
//                //In the uniform fill case, we have the same result too.
//                //In the fill case, note that we change aspect ratio, but that is okay
//                switch(stretchDirection)
//                {
//                    case StretchDirection.UpOnly:
//                        if (scaleX < 1.0) scaleX = 1.0;
//                        if (scaleY < 1.0) scaleY = 1.0;
//                        break;
//
//                    case StretchDirection.DownOnly:
//                        if (scaleX > 1.0) scaleX = 1.0;
//                        if (scaleY > 1.0) scaleY = 1.0;
//                        break;
//
//                    case StretchDirection.Both:
//                        break;
//
//                    default:
//                        break;
//                }
//            }
//            //Return this as a size now
//            return new Size(scaleX, scaleY);
//        }

//Helper
            //ImageSource imageSource = Source;
            //Size naturalSize = new Size();

            //if (imageSource == null)
            //{
            //    return naturalSize;
            //}

            //try
            //{
            //    UpdateBaseUri(this, imageSource);

            //    naturalSize = imageSource.Size;
            //}
            //catch(Exception e)
            //{
            //    Source = null;
            //    RaiseEvent(new ExceptionRoutedEventArgs(ImageFailedEvent, this, e));
            //}

            ////get computed scale factor
            //Size scaleFactor = Viewbox.ComputeScaleFactor(inputSize,
            //                                              naturalSize,
            //                                              this.Stretch,
            //                                              this.StretchDirection);

            //// Returns our minimum size & sets DesiredSize.
            //return new Size(naturalSize.Width * scaleFactor.Width, naturalSize.Height * scaleFactor.Height);

SizeF ComputeScaleFactor(SizeF AvailableSize, SizeF ContentSize, Stretch::Value ContentStretch, StretchDirection::Value ContentStretchDirection)
{
    FLOAT ScaleX = 1.0f;
    FLOAT ScaleY = 1.0f;

    BOOL ConstrainedWidth = (AvailableSize.width != FLT_MAX);
    BOOL ConstrainedHeight = (AvailableSize.height != FLT_MAX);

    if((ContentStretch == Stretch::Uniform || ContentStretch == Stretch::UniformToFill || ContentStretch == Stretch::Fill) && (ConstrainedWidth || ConstrainedHeight))
    {
        ScaleX = (ContentSize.width == 0.0f) ? 0.0 : AvailableSize.width / ContentSize.width;
        ScaleY = (ContentSize.height == 0.0f) ? 0.0 : AvailableSize.height / ContentSize.height;

        if(!ConstrainedWidth)
        {
            ScaleX = ScaleY;
        }
        else if(ConstrainedHeight)
        {
            ScaleY = ScaleX;
        }
        else 
        {
            switch (ContentStretch) 
            {
                case Stretch::Uniform:
                    {
                        FLOAT MinScale = std::min(ScaleX, ScaleY);

                        ScaleX = MinScale;
                        ScaleY = MinScale;

                        break;
                    }

                case Stretch::UniformToFill:
                    {
                        FLOAT MinScale = std::max(ScaleX, ScaleY);

                        ScaleX = MinScale;
                        ScaleY = MinScale;

                        break;
                    }

                case Stretch::Fill:
                    {
                        break;
                    }
            }
        }

        switch(ContentStretchDirection)
        {
            case StretchDirection::UpOnly:
                {
                    ScaleX = std::max(ScaleX, 1.0f);
                    ScaleY = std::max(ScaleY, 1.0f);

                    break;
                }

            case StretchDirection::DownOnly:
                {
                    ScaleX = std::min(ScaleX, 1.0f);
                    ScaleY = std::min(ScaleY, 1.0f);

                    break;
                }

            case StretchDirection::Both:
                break;

            default:
                break;
        }
    }

    return SizeF(ScaleX, ScaleY);
}

HRESULT CImage::ComputeSize(SizeF AvailableSize, SizeF& UsedSize)
{
    HRESULT hr = S_OK;
    SizeU ImageSize;
    Stretch::Value ImageStretch;
    StretchDirection::Value ImageStretchDirection;

    IFC(GetEffectiveStretch(&ImageStretch));
    IFC(GetEffectiveStretchDirection(&ImageStretchDirection));

    if(m_ImageBrush != NULL)
    {
        IFC(m_ImageBrush->GetSize(&ImageSize));
    }

    SizeF ScaleFactor = ComputeScaleFactor(AvailableSize, SizeF(ImageSize.width, ImageSize.height), ImageStretch, ImageStretchDirection);

    UsedSize.width = ScaleFactor.width * ((FLOAT)ImageSize.width);
    UsedSize.height = ScaleFactor.height * ((FLOAT)ImageSize.height);

Cleanup:
    return hr;
}

HRESULT CImage::MeasureInternal(SizeF AvailableSize, SizeF& DesiredSize)
{
    HRESULT hr = S_OK;
    
    IFC(ComputeSize(AvailableSize, DesiredSize));

Cleanup:
    return hr;
}

HRESULT CImage::ArrangeInternal(SizeF AvailableSize, SizeF& UsedSize)
{
    HRESULT hr = S_OK;
    
    IFC(ComputeSize(AvailableSize, UsedSize));

    m_GeometryDirty = TRUE;    

Cleanup:
    return hr;
}

HRESULT CImage::CreatePropertyInformation(CPropertyInformation** ppInformation)
{
    HRESULT hr = S_OK;
    CStaticPropertyInformation* pStaticInformation = NULL;
    CPropertyInformation* pBaseInformation = NULL;
    CDelegatingPropertyInformation* pDelegatingPropertyInformation = NULL;

    CStaticProperty* Properties[] = 
    {
        &SourceProperty,
        &StretchProperty,
        &StretchDirectionProperty
    };
    
    IFCPTR(ppInformation);

    IFC(CStaticPropertyInformation::Create(Properties, ARRAYSIZE(Properties), &pStaticInformation));
    IFC(CFrameworkElement::CreatePropertyInformation(&pBaseInformation));
    IFC(CDelegatingPropertyInformation::Create(pStaticInformation, pBaseInformation, &pDelegatingPropertyInformation));

    *ppInformation = pDelegatingPropertyInformation;
    pDelegatingPropertyInformation = NULL;

Cleanup:
    ReleaseObject(pStaticInformation);
    ReleaseObject(pBaseInformation);
    ReleaseObject(pDelegatingPropertyInformation);

    return hr;
}

HRESULT CImage::GetLayeredValue(CProperty* pProperty, CLayeredValue** ppLayeredValue)
{
    HRESULT hr = S_OK;

    IFCPTR(pProperty);
    IFCPTR(ppLayeredValue);

    //TODO: Make this a lookup table rather than requiring a comparison per property.
    if(pProperty == &CImage::SourceProperty)
    {
        *ppLayeredValue = &m_Source;
    }
    else if(pProperty == &CImage::StretchProperty)
    {
        *ppLayeredValue = &m_Stretch;
    }
    else if(pProperty == &CImage::StretchDirectionProperty)
    {
        *ppLayeredValue = &m_StretchDirection;
    }
    else
    {
        hr = CFrameworkElement::GetLayeredValue(pProperty, ppLayeredValue);
    }

Cleanup:
    return hr;
}

HRESULT CImage::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;
    CHitTestResult* pVisualHitTestResult = NULL;

    IFCPTR(ppHitTestResult);

    IFC(m_ImageVisual->HitTest(LocalPoint, &pVisualHitTestResult));

    if(pVisualHitTestResult)
    {
        IFC(CHitTestResult::Create(this, ppHitTestResult));
    }

Cleanup:
    ReleaseObject(pVisualHitTestResult);

    return hr;
}