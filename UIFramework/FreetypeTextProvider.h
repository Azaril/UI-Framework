#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Factory.h"
#include "TextProvider.h"
#include "TextureAllocator.h"

class CFreetypeFontFace;

class CRegisteredFont : public CRefCountedObject
{
    public:
        DECLARE_FACTORY3( CRegisteredFont, IResourceProvider*, const WCHAR*, UINT32 );

        void GetIdentifier(
            __deref_out_ecount(*pIdentifierLength) const WCHAR** ppIdentifier,
            UINT32* pIdentifierLength
            );

        __out IResourceProvider* GetResourceProvider(
            );

        __out_opt CFreetypeFontFace* GetFontFace(
            );

        void SetFontFace(
            __in_opt CFreetypeFontFace* pFontFace
            );

    protected:
        CRegisteredFont(
            );

        virtual ~CRegisteredFont(
            );

        __checkReturn HRESULT Initialize(
            __in IResourceProvider* pResourceProvider,
            __in_ecount(IdentifierLength) const WCHAR* pIdentifier,
            UINT32 IdentifierLength
            );

        IResourceProvider* m_pResourceProvider;
        std::wstring m_Identifier;
        CFreetypeFontFace* m_pFontFace;
};

class CFreetypeTextProvider : public CTextProvider
{
    public:
        DECLARE_FACTORY1( CFreetypeTextProvider, ITextureAllocator* );

        CFreetypeTextProvider(
            );

        virtual ~CFreetypeTextProvider(
            );

        __override virtual __checkReturn HRESULT CreateFormat(
            __in const CFontDescription* pFontDescription,
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

        __override virtual __checkReturn HRESULT RegisterFont(
            __in IResourceProvider* pResourceProvider,
            __in_ecount(IdentifierLength) const WCHAR* pIdentifier,
            UINT32 IdentifierLength
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
        ITextureAllocator* m_pTextureAllocator;
        vector< CRegisteredFont* > m_RegisteredFonts;
};

