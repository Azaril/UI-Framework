#include "FontDescription.h"

CFontDescription::CFontDescription(
    )
    : m_FontSize(0)
{
}

CFontDescription::CFontDescription(
    __in_z const WCHAR* pFontName, 
    FLOAT FontSize,
    __in_z const WCHAR* pLocale
    )
    : m_FontName(pFontName)
    , m_FontSize(FontSize)
    , m_Locale(pLocale)
{
}

CFontDescription::~CFontDescription( 
    )
{
}

__out const WCHAR*
CFontDescription::GetFontName(
    ) const
{
    return m_FontName.c_str();
}

FLOAT
CFontDescription::GetFontSize(
    ) const
{
    return m_FontSize;
}

__out const WCHAR*
CFontDescription::GetLocale(
    ) const
{
    return m_Locale.c_str();
}