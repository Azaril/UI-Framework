#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Factory.h"
#include "RefCounted.h"
#include "FreetypeFontFace.h"

class CFreetypeFontSize : public CRefCountedObject
{
    public:
        DECLARE_FACTORY3( CFreetypeFontSize, CFreetypeFontFace*, FT_Size, FLOAT );

        __checkReturn HRESULT GetFontMetrics(
            __out FontMetrics* pFontMetrics
            );

        __checkReturn HRESULT Apply(
            );

    protected:
        CFreetypeFontSize(
            );

        virtual ~CFreetypeFontSize(
            );

        __checkReturn HRESULT Initialize(
            __in CFreetypeFontFace* pFontFace,
            FT_Size pFontSizeStorage,
            FLOAT fontSize
            );

        CFreetypeFontFace* m_pFontFace;
        FT_Size m_pSize;
        FLOAT m_FontSize;
        bool m_InitializedSize;
};

