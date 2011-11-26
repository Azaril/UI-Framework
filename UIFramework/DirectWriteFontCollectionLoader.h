#pragma once

#include "Factory.h"
#include "COMRefCounted.h"
#include "DirectWriteRegisteredFontCollection.h"
#include "DirectWriteFontFileLoader.h"

#include <dwrite.h>

class CDirectWriteFontCollectionLoader : public CCOMRefCountedObjectBase< IDWriteFontCollectionLoader >
{
    public:
        DECLARE_FACTORY1( CDirectWriteFontCollectionLoader, CDirectWriteFontFileLoader* );

        //
        // IUnknown
        //
        STDMETHOD(QueryInterface)( 
            REFIID riid,
            __deref_out void** ppvObject
            );

        //
        // IDWriteFontCollectionLoader
        //
        STDMETHOD(CreateEnumeratorFromKey)(
            IDWriteFactory* pFactory,
            __in_bcount(collectionKeySize) void const* pCollectionKey,
            UINT32 collectionKeySize,
            __deref_out IDWriteFontFileEnumerator** fontFileEnumerator
            );

    protected:
        CDirectWriteFontCollectionLoader(
            );

        virtual ~CDirectWriteFontCollectionLoader(
            );

        __checkReturn HRESULT Initialize(
            __in CDirectWriteFontFileLoader* pFileLoader
            );

        CDirectWriteFontFileLoader* m_pFontFileLoader;
};

