#include "FreetypeTextProvider.h"
#include "ReadStream.h"
#include "FreetypeFontFace.h"
#include "FreetypeTextFormat.h"
#include "FreetypeTextLayout.h"

//TODO: Remove file stream reliance in text provider (add load from stream).
#include "FileResourceStream.h"

CFreetypeTextProvider::CFreetypeTextProvider(
    )
    : m_pLibrary(NULL)
    , m_pDefaultFormat(NULL)
    , m_pTextureAllocator(NULL)
{
}

CFreetypeTextProvider::~CFreetypeTextProvider(
    )
{
    ReleaseObject(m_pDefaultFormat);

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

    IFC(CreateFormat(L"Arial", 12, L"en-us", &m_pDefaultFormat));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CFreetypeTextProvider::CreateFormat(
    __in_z const WCHAR* pFontName,
    FLOAT FontSize, 
    __in_z const WCHAR* pLocaleName,
    __deref_out CTextFormat** ppTextFormat 
    )
{
    HRESULT hr = S_OK;
    FT_Face pFontFace = NULL;
    IReadStream* pReadStream = NULL;
    UINT64 streamSize = 0;       
    CFreetypeFontFace* pFreetypeFontFace = NULL;
    CFreetypeTextFormat* pFreetypeTextFormat = NULL;
    FT_Stream pFTStream = NULL;

    //HACK: Remove this and load from stream provided.
    {
        FILE* pFile = fopen("c:\\temp\\segoeui.ttf", "rb");
        CFileResourceStream* pFileStream = NULL;

        IFC(CFileResourceStream::Create(pFile, &pFileStream));

        pReadStream = pFileStream;
        pFileStream = NULL;
    }

    AddRefObject(pReadStream);

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

        IFCEXPECT(FT_Open_Face(m_pLibrary, &openArgs, 0, &pFontFace) == 0);

        //
        // Transfer ownership of stream.
        //
        pFTStream = NULL;
        pReadStream = NULL;
    }

    //TODO: Cache font face so it can be shared by different text formats.
    IFC(CFreetypeFontFace::Create(pFontFace, &pFreetypeFontFace));

    //TODO: Cache text format as it will contain textures of glyphs.
    IFC(CFreetypeTextFormat::Create(pFreetypeFontFace, FontSize, m_pTextureAllocator, &pFreetypeTextFormat));

    *ppTextFormat = pFreetypeTextFormat;
    pFreetypeTextFormat = NULL;

Cleanup:
    delete pFTStream;

    ReleaseObject(pReadStream);
    ReleaseObject(pFreetypeFontFace);
    ReleaseObject(pFreetypeTextFormat);

    FT_Done_Face(pFontFace);

    return hr;
}

__override __checkReturn HRESULT 
CFreetypeTextProvider::GetDefaultFormat( 
    __deref_out CTextFormat** ppTextFormat 
    )
{
    HRESULT hr = S_OK;

    SetObject(*ppTextFormat, m_pDefaultFormat);

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
    const SizeF& Size, 
    __deref_out CEditableTextLayout** ppEditableTextLayout 
    )
{
    HRESULT hr = S_OK;

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