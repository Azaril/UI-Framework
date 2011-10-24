#include "FreetypeTextProvider.h"
#include "ReadStream.h"
#include "FreetypeFontFace.h"
#include "FreetypeTextFormat.h"
#include "FreetypeTextLayout.h"

CFreetypeTextProvider::CFreetypeTextProvider(
    )
    : m_pLibrary(NULL)
    , m_pTextureAllocator(NULL)
{
}

CFreetypeTextProvider::~CFreetypeTextProvider(
    )
{
    for (vector< CRegisteredFont* >::iterator it = m_RegisteredFonts.begin(); it != m_RegisteredFonts.end(); ++it)
    {
        (*it)->Release();
    }

    if (m_pLibrary != NULL)
    {
        FT_Done_FreeType(m_pLibrary);
    }
}

__checkReturn HRESULT
CFreetypeTextProvider::Initialize( 
    __in ITextureAllocator* pTextureAllocator
    )
{
    HRESULT hr = S_OK;
    
    m_pTextureAllocator = pTextureAllocator;

    IFCEXPECT(FT_Init_FreeType(&m_pLibrary) == 0);

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CFreetypeTextProvider::CreateFormat(
    __in const CFontDescription* pFontDescription,
    __deref_out CTextFormat** ppTextFormat 
    )
{
    HRESULT hr = S_OK;
    FT_Face pFreetypeFontFace = NULL;
    IReadStream* pReadStream = NULL;
    UINT64 streamSize = 0;       
    CFreetypeFontFace* pNewFontFace = NULL;
    CFreetypeTextFormat* pFreetypeTextFormat = NULL;
    FT_Stream pFTStream = NULL;

    for (vector< CRegisteredFont* >::iterator it = m_RegisteredFonts.begin(); it != m_RegisteredFonts.end(); ++it)
    {
        CFreetypeFontFace* pFontFace = (*it)->GetFontFace();

        //
        // Load the font face if it has not yet been loaded.
        //
        if (pFontFace == NULL)
        {
            const WCHAR* pIdentifier = NULL;
            UINT32 IdentifierLength = 0;

            IResourceProvider* pResourceProvider = (*it)->GetResourceProvider();

            (*it)->GetIdentifier(&pIdentifier, &IdentifierLength);

            IFC(pResourceProvider->ReadResource(pIdentifier, IdentifierLength, &pReadStream));

            IFC(pReadStream->GetSize(&streamSize));

            pFTStream = new FT_StreamRec_();
            IFCOOM(pFTStream);

            {
                FT_Open_Args openArgs = { };

                pFTStream->descriptor.pointer = pReadStream;
                pFTStream->base = 0;
                pFTStream->pos = 0;
                pFTStream->size = (unsigned long)streamSize;
                pFTStream->read = FreeTypeStreamRead;
                pFTStream->close = FreeTypeStreamClose;;

                openArgs.flags = FT_OPEN_STREAM;
                openArgs.stream = pFTStream;

                IFCEXPECT(FT_Open_Face(m_pLibrary, &openArgs, 0, &pFreetypeFontFace) == 0);

                //
                // Transfer ownership of stream.
                //
                pFTStream = NULL;
                pReadStream = NULL;
            }

            IFC(CFreetypeFontFace::Create(pFreetypeFontFace, &pNewFontFace));

            //
            // Cache the font face.
            //
            (*it)->SetFontFace(pNewFontFace);

            //
            // Keep a weak reference to the font face.
            //
            pFontFace = pNewFontFace;

            //
            // Clean up font loading resources.
            // 
            ReleaseObject(pReadStream);
            ReleaseObject(pNewFontFace);

            FT_Done_Face(pFreetypeFontFace);
            pFreetypeFontFace = NULL;

            delete pFTStream;
        }


        {
            BOOL matchesDescription = FALSE;

            //
            // Search family names in font face for a match.
            //
            const WCHAR** ppFamilyNames = NULL;
            UINT32 familyNameCount = 0;

            IFC(pFontFace->GetFamilyNames(&ppFamilyNames, &familyNameCount));

            for (UINT32 i = 0; i < familyNameCount && !matchesDescription; ++i)
            {
                if (wcscmp(ppFamilyNames[i], pFontDescription->GetFontName()) == 0)
                {
                    matchesDescription = TRUE;

                    break;
                }
            }

            //
            // If this font matches the description then get or create a new format.
            //
            if (matchesDescription)
            {
                IFC(pFontFace->GetOrCreateFormat(pFontDescription, m_pTextureAllocator, &pFreetypeTextFormat));

                break;
            }
        }
    }

    *ppTextFormat = pFreetypeTextFormat;
    pFreetypeTextFormat = NULL;

Cleanup:
    delete pFTStream;

    ReleaseObject(pReadStream);
    ReleaseObject(pNewFontFace);
    ReleaseObject(pFreetypeTextFormat);

    FT_Done_Face(pFreetypeFontFace);

    return hr;
}

__override __checkReturn HRESULT 
CFreetypeTextProvider::CreateTextLayout(
    __in_ecount(CharacterCount) const WCHAR* pText, 
    UINT32 CharacterCount, 
    __in CTextFormat* pTextFormat, 
    const SizeF& Size,
    __deref_out CTextLayout** ppTextLayout 
    )
{
    HRESULT hr = S_OK;
    CFreetypeTextFormat* pFreetypeTextFormat = NULL;
    CFreetypeTextLayout* pFreetypeTextLayout = NULL;

    pFreetypeTextFormat = (CFreetypeTextFormat*)pTextFormat;

    IFC(CFreetypeTextLayout::Create(pText, CharacterCount, pFreetypeTextFormat, Size, &pFreetypeTextLayout));

    *ppTextLayout = pFreetypeTextLayout;
    pFreetypeTextLayout = NULL;

Cleanup:
    ReleaseObject(pFreetypeTextLayout);

    return hr;
}

__override __checkReturn HRESULT 
CFreetypeTextProvider::CreateEditableTextLayout(
    __in CTextFormat* pTextFormat, 
    const SizeF& Size, 
    __deref_out CEditableTextLayout** ppEditableTextLayout 
    )
{
    HRESULT hr = S_OK;
    CFreetypeTextFormat* pFreetypeTextFormat = NULL;
    CFreetypeTextLayout* pFreetypeTextLayout = NULL;

    pFreetypeTextFormat = (CFreetypeTextFormat*)pTextFormat;

    IFC(CFreetypeTextLayout::Create(NULL, 0, pFreetypeTextFormat, Size, &pFreetypeTextLayout));

    *ppEditableTextLayout = pFreetypeTextLayout;
    pFreetypeTextLayout = NULL;

Cleanup:
    ReleaseObject(pFreetypeTextLayout);

    return hr;
}

unsigned long
CFreetypeTextProvider::FreeTypeStreamRead(
    __in FT_Stream pStream,
    unsigned long streamOffset,
    __out_bcount(bufferSize) unsigned char* pBuffer,
    unsigned long bufferSize
    )
{
    HRESULT hr = S_OK;
    IReadStream* pReadStream = NULL;
    UINT64 bytesRead = 0;

    IFCEXPECT(streamOffset <= pStream->size);

    pReadStream = (IReadStream*)pStream->descriptor.pointer;

    if (pStream->pos != streamOffset)
    {
        IFC(pReadStream->Seek(SeekType::Begin, streamOffset, NULL));
    }

    if (bufferSize > 0)
    {
        IFC(pReadStream->Read(pBuffer, bufferSize, &bytesRead));
    }

Cleanup:
    if (FAILED(hr))
    {
        bytesRead = 0;
    }

    return (unsigned long)bytesRead;
}

void
CFreetypeTextProvider::FreeTypeStreamClose(
    FT_Stream pStream 
    )
{
    IReadStream* pReadStream = NULL;

    pReadStream = (IReadStream*)pStream->descriptor.pointer;

    ReleaseObject(pReadStream);

    delete pStream;
}

__override __checkReturn HRESULT 
CFreetypeTextProvider::RegisterFont(
    __in IResourceProvider* pResourceProvider,
    __in_ecount(IdentifierLength) const WCHAR* pIdentifier,
    UINT32 IdentifierLength
    )
{
    HRESULT hr = S_OK;
    CRegisteredFont* pRegisteredFont = NULL;

    IFC(CRegisteredFont::Create(pResourceProvider, pIdentifier, IdentifierLength, &pRegisteredFont));

    m_RegisteredFonts.push_back(pRegisteredFont);

    pRegisteredFont = NULL;

Cleanup:
    ReleaseObject(pRegisteredFont);

    return hr;
}

CRegisteredFont::CRegisteredFont(
    )
    : m_pResourceProvider(NULL)
    , m_pFontFace(NULL)
{
}

CRegisteredFont::~CRegisteredFont( 
    )
{
    ReleaseObject(m_pResourceProvider);
    ReleaseObject(m_pFontFace);
}

__checkReturn HRESULT
CRegisteredFont::Initialize(
    __in IResourceProvider* pResourceProvider,
    __in_ecount(IdentifierLength) const WCHAR* pIdentifier,
    UINT32 IdentifierLength
    )
{
    HRESULT hr = S_OK;

    SetObject(m_pResourceProvider, pResourceProvider);
    
    m_Identifier.assign(pIdentifier, IdentifierLength);

    return hr;
}

void 
CRegisteredFont::GetIdentifier(
    __deref_out_ecount(*pIdentifierLength) const WCHAR** ppIdentifier,
    UINT32* pIdentifierLength
    )
{
    *ppIdentifier = m_Identifier.c_str();
    *pIdentifierLength = m_Identifier.length();
}

__out IResourceProvider* 
CRegisteredFont::GetResourceProvider(
    )
{
    return m_pResourceProvider;
}

__out_opt CFreetypeFontFace* 
CRegisteredFont::GetFontFace(
    )
{
    return m_pFontFace;
}

void 
CRegisteredFont::SetFontFace(
    __in_opt CFreetypeFontFace* pFontFace
    )
{
    ReplaceObject(m_pFontFace, pFontFace);
}