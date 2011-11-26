#pragma once

#include "Factory.h"
#include "ResourceProvider.h"
#include "RefCounted.h"

class CDirectWriteRegisteredFontCollection : public CRefCountedObject
{
    public:
        DECLARE_FACTORY( CDirectWriteRegisteredFontCollection );

        __checkReturn HRESULT RegisterFont(
            __in IResourceProvider* pResourceProvider,
            __in_ecount(IdentifierLength) const WCHAR* pIdentifier,
            UINT32 IdentifierLength,
            __out UINT32* pFontToken
            );

        __checkReturn HRESULT GetReadStream(
            UINT32 fontToken,
            __deref_out IReadStream** ppStream
            );

    protected:
        CDirectWriteRegisteredFontCollection(
            );

        virtual ~CDirectWriteRegisteredFontCollection(
            );

        __checkReturn HRESULT Initialize(
            );

        class CFontInfo
        {
            public:
                CFontInfo(
                    __in IResourceProvider* pResourceProvider,
                    __in_ecount(IdentifierLength) const WCHAR* pIdentifier,
                    UINT32 IdentifierLength
                    )
                    : m_pResourceProvider(pResourceProvider)
                    , m_Identifier(pIdentifier, IdentifierLength)
                {
                    AddRefObject(m_pResourceProvider);
                }

                CFontInfo(
                    const CFontInfo& other
                    )
                    : m_pResourceProvider(other.m_pResourceProvider)
                    , m_Identifier(other.m_Identifier)
                {
                    AddRefObject(m_pResourceProvider);
                }

                ~CFontInfo(
                    )
                {
                    ReleaseObject(m_pResourceProvider);
                }

                CFontInfo& operator=(
                    const CFontInfo& other
                    )
                {
                    ReplaceObject(m_pResourceProvider, other.m_pResourceProvider);
                    m_Identifier = other.m_Identifier;
                }

                __out IResourceProvider* GetResourceProvider(
                    )
                {
                    return m_pResourceProvider;
                }

                const std::wstring GetIdentifier(
                    )
                {
                    return m_Identifier;
                }

            protected:
                IResourceProvider* m_pResourceProvider;
                std::wstring m_Identifier;
        };

        vector< CFontInfo > m_RegisteredFonts;
};

