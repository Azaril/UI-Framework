#pragma once

#include "Types.h"

class UIFRAMEWORK_API CFontDescription
{
    public:
        CFontDescription(
            );

        CFontDescription(
            __in_z const WCHAR* pFontName,
            FLOAT FontSize,
            __in_z const WCHAR* pLocale
            );

        ~CFontDescription(
            );

        __out const WCHAR* GetFontName(
            ) const;

        __out const WCHAR* GetLocale(
            ) const;

        FLOAT GetFontSize(
            ) const;

    protected:
        std::wstring m_FontName;
        std::wstring m_Locale;
        FLOAT m_FontSize;
};