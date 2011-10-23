#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Factory.h"
#include "TextProvider.h"
#include "TextureAllocator.h"

class CFreetypeTextProvider : public CTextProvider
{
    public:
        DECLARE_FACTORY1( CFreetypeTextProvider, ITextureAllocator* );

        CFreetypeTextProvider(
            );

        virtual ~CFreetypeTextProvider(
            );

        __override virtual __checkReturn HRESULT CreateFormat(
            __in_z const WCHAR* pFontName,
            FLOAT FontSize, 
            __in_z const WCHAR* pLocaleName,
            __deref_out CTextFormat** ppTextFormat 
            );

        __override virtual __checkReturn HRESULT GetDefaultFormat( 
            __deref_out CTextFormat** ppTextFormat 
            );

        __override virtual __checkReturn HRESULT CreateTextLayout(
            __in_ecount(CharacterCount) const WCHAR* pText, 
            UINT32 CharacterCount, 
            __in CTextFormat* pTextFormat, 
            const SizeF& Size,
            __deref_out CTextLayout** ppTextLayout 
            );

        __override virtual __checkReturn HRESULT CreateEditableTextLayout(
            __in CTextFormat* pTextFormat,
            const SizeF& Size, 
            __deref_out CEditableTextLayout** ppEditableTextLayout 
            );

    protected:
        __checkReturn HRESULT Initialize(
            __in ITextureAllocator* pTextureAllocator
            );

        static unsigned long FreeTypeStreamRead(
            __in FT_Stream pStream,
            unsigned long streamOffset,
            __out_bcount(bufferSize) unsigned char* pBuffer,
            unsigned long bufferSize
            );

        static void FreeTypeStreamClose(
            __in FT_Stream pStream
            );

        FT_Library m_pLibrary;
        CTextFormat* m_pDefaultFormat;
        ITextureAllocator* m_pTextureAllocator;
};

