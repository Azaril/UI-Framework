#pragma once

#include "Factory.h"
#include "COMRefCounted.h"
#include "DirectWriteFontFileLoader.h"

#include <dwrite.h>

class CDirectWriteFontFileEnumerator : public CCOMRefCountedObjectBase< IDWriteFontFileEnumerator >
{
    public:
        DECLARE_FACTORY4( CDirectWriteFontFileEnumerator, IDWriteFactory*, CDirectWriteFontFileLoader*, UINT32*, UINT32 );

        //
        // IUnknown
        //
        STDMETHOD(QueryInterface)( 
            REFIID riid,
            __deref_out void** ppvObject
            );

        //
        // IDWriteFontFileEnumerator
        //
        STDMETHOD(MoveNext)(
            __out BOOL* pHasCurrentFile
            );

        STDMETHOD(GetCurrentFontFile)(
            __deref_out IDWriteFontFile** ppFontFile
            );

    protected:
        CDirectWriteFontFileEnumerator(
            );

        virtual ~CDirectWriteFontFileEnumerator(
            );

        __checkReturn HRESULT Initialize(
            __in IDWriteFactory* pFactory,
            __in CDirectWriteFontFileLoader* pFileLoader,
            __in_ecount(tokenCount) UINT32* pFontTokens,
            UINT32 tokenCount
            );

        IDWriteFactory* m_pFactory;
        CDirectWriteFontFileLoader* m_pFontFileLoader;
        INT32 m_Current;
        UINT32* m_pFontTokens;
        INT32 m_TokenCount;
        IDWriteFontFile* m_pCurrentFontFile;
};

