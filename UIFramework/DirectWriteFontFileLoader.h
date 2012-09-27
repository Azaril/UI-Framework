#pragma once

#if defined(FRAMEWORK_DWRITE)

#include "Factory.h"
#include "COMRefCounted.h"
#include "DirectWriteRegisteredFontCollection.h"

#include <dwrite.h>

class CDirectWriteFontFileLoader : public CCOMRefCountedObjectBase< IDWriteFontFileLoader >
{
    public:
        DECLARE_FACTORY1( CDirectWriteFontFileLoader, CDirectWriteRegisteredFontCollection* );

        //
        // IUnknown
        //
        STDMETHOD(QueryInterface)( 
            REFIID riid,
            __deref_out void** ppvObject
            );

        //
        // IDWriteFontFileLoader
        //
        STDMETHOD(CreateStreamFromKey)(
            __in_bcount(fontFileReferenceKeySize) void const* pFontFileReferenceKey,
            UINT32 fontFileReferenceKeySize,
            __deref_out IDWriteFontFileStream** ppFontFileStream
            );

    protected:
        CDirectWriteFontFileLoader(
            );

        virtual ~CDirectWriteFontFileLoader(
            );

        __checkReturn HRESULT Initialize(
            __in CDirectWriteRegisteredFontCollection* pFontCollection
            );

        CDirectWriteRegisteredFontCollection* m_pFontCollection;
};

#endif