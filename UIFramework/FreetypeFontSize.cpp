#include "FreetypeFontSize.h"

#include FT_SIZES_H

CFreetypeFontSize::CFreetypeFontSize(
    )
    : m_pFontFace(NULL)
    , m_pSize(NULL)
    , m_InitializedSize(FALSE)
    , m_FontSize(0)
{
}

CFreetypeFontSize::~CFreetypeFontSize(
    )
{
    if (m_pSize != NULL)
    {
        FT_Done_Size(m_pSize);
    }

    ReleaseObject(m_pFontFace);
}

__checkReturn HRESULT 
CFreetypeFontSize::Initialize(
    __in CFreetypeFontFace* pFontFace,
    FT_Size pFontSizeStorage,
    FLOAT fontSize
    )
{
    HRESULT hr = S_OK;

    SetObject(m_pFontFace, pFontFace);

    m_pSize = pFontSizeStorage;
    m_FontSize = fontSize;

    return hr;
}

__checkReturn HRESULT
CFreetypeFontSize::GetFontMetrics(
    __out FontMetrics* pFontMetrics
    )
{
    HRESULT hr = S_OK;

    if (!m_InitializedSize)
    {
        IFC(Apply());
    }

    pFontMetrics->Height = m_pSize->metrics.height;
    pFontMetrics->Asecender = m_pSize->metrics.ascender;
    pFontMetrics->Descender = m_pSize->metrics.descender;

Cleanup:
    return hr;
}

__checkReturn HRESULT
CFreetypeFontSize::Apply(
    )
{
    HRESULT hr = S_OK;

    IFCEXPECT(FT_Activate_Size(m_pSize) == FT_Err_Ok);

    if (!m_InitializedSize)
    {
        m_InitializedSize = true;

        FLOAT realFontSize = fabsf(m_FontSize);

        FT_F26Dot6 fixedFontSize = (FT_F26Dot6)(realFontSize * 64.0f);

        //TODO: Query DPI?
        IFCEXPECT(FT_Set_Char_Size(m_pFontFace->GetFontFace(), 0, fixedFontSize, 96, 96) == 0);
    }

Cleanup:
    return hr;
}