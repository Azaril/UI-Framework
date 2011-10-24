#include "FreetypeFontFace.h"
#include "StackHeapBuffer.h"
#include "StringConversion.h"
#include "FontDescription.h"
#include "FreetypeTextFormat.h"

#include <freetype/ftsnames.h>
#include <freetype/ttnameid.h>

CFreetypeFontFace::CFreetypeFontFace(
    )
    : m_pFontFace(NULL)
    , m_CurrentGlyph((UINT32)-1)
    , m_CurrentFontSize(-1.0f)
    , m_LoadedFamilyNames(FALSE)
    , m_ppFamilyNames(NULL)
    , m_FamilyNameCount(0)
{
}

CFreetypeFontFace::~CFreetypeFontFace(
    )
{
    for (UINT32 i = 0; i < m_FamilyNameCount; ++i)
    {
        delete m_ppFamilyNames[i];
    }

    delete [] m_ppFamilyNames;

    if (m_pFontFace != NULL)
    {
        FT_Done_Face(m_pFontFace);
    }
}

__checkReturn HRESULT
CFreetypeFontFace::Initialize( 
    __in FT_Face pFontFace
    )
{
    HRESULT hr = S_OK;

    m_pFontFace = pFontFace;

    IFCEXPECT(FT_Reference_Face(m_pFontFace) == 0);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CFreetypeFontFace::LoadGlyph(
    const UINT32 glyph
    )
{
    HRESULT hr = S_OK;

    if (m_CurrentGlyph != glyph)
    {
        UINT32 glyphIndex = FT_Get_Char_Index(m_pFontFace, glyph);

        IFCEXPECT(FT_Load_Glyph(m_pFontFace, glyphIndex, FT_LOAD_DEFAULT) == 0);

        m_CurrentGlyph = glyph;
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT
CFreetypeFontFace::SetFontSize(
    const FLOAT& fontSize
    )
{
    HRESULT hr = S_OK;
    FLOAT realFontSize = fabsf(fontSize);

    if (m_CurrentFontSize != realFontSize)
    {
        //TODO: Query DPI?
        IFCEXPECT(FT_Set_Char_Size(m_pFontFace, 0, FT_F26Dot6(64.0f * realFontSize), 96, 96) == 0);

        m_CurrentFontSize = realFontSize;
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT
CFreetypeFontFace::GetFontMetrics(
    const FLOAT& fontSize,
    __out FontMetrics* pFontMetrics
    )
{
    HRESULT hr = S_OK;

    IFC(SetFontSize(fontSize));

    pFontMetrics->Height = m_pFontFace->size->metrics.height;
    pFontMetrics->Asecender = m_pFontFace->size->metrics.ascender;
    pFontMetrics->Descender = m_pFontFace->size->metrics.descender;

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CFreetypeFontFace::GetGlyphMetrics(
    const UINT32 glyph,
    const FLOAT& fontSize,
    __out GlyphMetrics* pGlyphMetrics
    )
{
    HRESULT hr = S_OK;
    FT_GlyphSlot pGlyphSlot = NULL;

    IFC(SetFontSize(fontSize));
    IFC(LoadGlyph(glyph));

    pGlyphSlot = m_pFontFace->glyph;

    pGlyphMetrics->Advance.x = pGlyphSlot->metrics.horiAdvance;
    pGlyphMetrics->Advance.y = pGlyphSlot->metrics.vertAdvance;
    
    pGlyphMetrics->HorizontalBearing.x = pGlyphSlot->metrics.horiBearingX;
    pGlyphMetrics->HorizontalBearing.y = pGlyphSlot->metrics.horiBearingY;

    pGlyphMetrics->VerticalBearing.x = pGlyphSlot->metrics.vertBearingX;
    pGlyphMetrics->VerticalBearing.y = pGlyphSlot->metrics.vertBearingY;

    pGlyphMetrics->Size.x = pGlyphSlot->metrics.width;
    pGlyphMetrics->Size.y = pGlyphSlot->metrics.height;

Cleanup:
    return hr;
}

__checkReturn HRESULT
CFreetypeFontFace::LoadIntoTexture(
    const UINT32 glyph,
    const FLOAT& fontSize,
    __in ITextureAllocator* pAllocator,
    __deref_out_opt ITexture** ppTexture
    )
{
    HRESULT hr = S_OK;
    FT_GlyphSlot pGlyphSlot = NULL;
    ITexture* pOutputTexture = NULL;
    StackHeapBuffer< BYTE, 1024 * 4 > bitmapBuffer;

    IFC(SetFontSize(fontSize));
    IFC(LoadGlyph(glyph));

    pGlyphSlot = m_pFontFace->glyph;

    IFCEXPECT(FT_Render_Glyph(pGlyphSlot, FT_RENDER_MODE_NORMAL) == 0);

    {
        const FT_Bitmap& bitmap = pGlyphSlot->bitmap;

        UINT32 width = bitmap.width;
        UINT32 height = bitmap.rows;

        if (width > 0 && height > 0)
        {
            IFC(pAllocator->AllocateTexture(width, height, &pOutputTexture));

            switch(pOutputTexture->GetPixelFormat())
            {
                case PixelFormat::B8G8R8A8:
                    {
                        UINT32 textureStride = PixelFormat::GetLineSize(PixelFormat::B8G8R8A8, width);
                        UINT32 outputDataSize = textureStride * height;

                        IFC(bitmapBuffer.EnsureBufferSize(outputDataSize));

                        const BYTE* pSourceRow = bitmap.buffer;
                        BYTE* pDestinationRow = bitmapBuffer.GetBuffer();

                        for (UINT32 i = 0; i < height; ++i)
                        {
                            const BYTE* pSource = pSourceRow;
                            BYTE* pDestination = pDestinationRow;

                            for (UINT32 j = 0; j < width; ++j)
                            {
                                BYTE val = *pSource;

                                pDestination[0] = 255;
                                pDestination[1] = 255;
                                pDestination[2] = 255;
                                pDestination[3] = val;

                                pSource += 1;
                                pDestination += 4;
                            }

                            pSourceRow += bitmap.pitch;
                            pDestinationRow += textureStride;
                        }

                        IFC(pOutputTexture->SetData(bitmapBuffer.GetBuffer(), outputDataSize, textureStride));

                        break;
                    }

                case PixelFormat::R8G8B8A8:
                    {
                        UINT32 textureStride = PixelFormat::GetLineSize(PixelFormat::R8G8B8A8, width);
                        UINT32 outputDataSize = textureStride * height;
                        
                        IFC(bitmapBuffer.EnsureBufferSize(outputDataSize));

                        const BYTE* pSourceRow = bitmap.buffer;
                        BYTE* pDestinationRow = bitmapBuffer.GetBuffer();

                        for (UINT32 i = 0; i < height; ++i)
                        {
                            const BYTE* pSource = pSourceRow;
                            BYTE* pDestination = pDestinationRow;

                            for (UINT32 j = 0; j < width; ++j)
                            {
                                BYTE val = *pSource;

                                pDestination[0] = 255;
                                pDestination[1] = 255;
                                pDestination[2] = 255;
                                pDestination[3] = val;

                                pSource += 1;
                                pDestination += 4;
                            }

                            pSourceRow += bitmap.pitch;
                            pDestinationRow += textureStride;
                        }

                        IFC(pOutputTexture->SetData(bitmapBuffer.GetBuffer(), outputDataSize, textureStride));

                        break;
                    }

                default:
                    {
                        IFC(E_UNEXPECTED);
                    }
            }
        }
    }

    *ppTexture = pOutputTexture;
    pOutputTexture = NULL;

Cleanup:
    ReleaseObject(pOutputTexture);

    return hr;
}

__checkReturn HRESULT
CFreetypeFontFace::GetFamilyNames(
    __deref_out_ecount(*pFamilyNameCount) const WCHAR*** pppFamilyNames,
    __out UINT32* pFamilyNameCount
    )
{
    HRESULT hr = S_OK;

    if (!m_LoadedFamilyNames)
    {
        StackHeapBuffer< WCHAR, 128 > conversionBuffer;
        list< std::wstring > familyNames;

        //
        // Get the font name count.
        //
        UINT32 sftNames = FT_Get_Sfnt_Name_Count(m_pFontFace);

        for (UINT32 i = 0; i < sftNames; ++i)
        {
            UINT32 convertedTextLength = 0;
            const WCHAR* pText = NULL;
            UINT32 textLength = 0;

            FT_SfntName name = { };

            //
            // Load the current font name string.
            //
            IFCEXPECT(FT_Get_Sfnt_Name(m_pFontFace, i, &name) == FT_Err_Ok);

            //
            // Check if this is a font family name.
            //
            if (name.name_id == TT_NAME_ID_FONT_FAMILY)
            {
                //
                // Check the platform type of this name.
                //
                switch (name.platform_id)
                {
                    //
                    // Microsoft platform string.
                    //
                    case TT_PLATFORM_MICROSOFT:
                        {
                            switch (name.encoding_id)
                            {
                                case TT_MS_ID_UNICODE_CS:
                                case TT_MS_ID_SYMBOL_CS:
                                    {
                                        IFC(ConvertUTF16BEToWCHAR((const UINT16*)name.string, name.string_len / sizeof(UINT16), &conversionBuffer, &convertedTextLength));

                                        pText = conversionBuffer.GetBuffer();

                                        break;
                                    }   
                            }

                            break;
                        }

                    //
                    // Apple unicode string.
                    //
                    case TT_PLATFORM_APPLE_UNICODE:
                        {
                            /*switch (name.encoding_id)
                            {
                            }*/

                            break;
                        }

                    case TT_PLATFORM_MACINTOSH:
                    case TT_PLATFORM_ISO:
                    case TT_PLATFORM_CUSTOM:
                    case TT_PLATFORM_ADOBE:
                        {
                            break;
                        }
                }
            }

            //
            // If text was converted, used it as the source buffer.
            //
            if (convertedTextLength > 0)
            {
                pText = conversionBuffer.GetBuffer();
                textLength = convertedTextLength;
            }

            //
            // Temporarily copy the name string.
            //
            if (pText != NULL)
            {
                familyNames.push_back(std::wstring(pText, textLength));
            }
        }

        //
        // Consolidate the family names.
        //
        m_ppFamilyNames = new WCHAR*[familyNames.size()];
        IFCOOM(m_ppFamilyNames);

        {
            UINT32 familyNameIndex = 0;

            //
            // Copy each family name in to a buffer.
            //
            for (list< std::wstring >::iterator it = familyNames.begin(); it != familyNames.end(); ++it)
            {
                UINT32 nameLength = it->length();

                m_ppFamilyNames[familyNameIndex] = new WCHAR[nameLength + 1];
                IFCOOM(m_ppFamilyNames[familyNameIndex]);

                wcsncpy(m_ppFamilyNames[familyNameIndex], it->c_str(), nameLength);

                m_ppFamilyNames[familyNameIndex][nameLength] = L'\0';

                ++familyNameIndex;
            }

            m_FamilyNameCount = familyNameIndex;
        }

        m_LoadedFamilyNames = TRUE;
    }

    *pppFamilyNames = (const WCHAR**)m_ppFamilyNames;
    *pFamilyNameCount = m_FamilyNameCount;

Cleanup:
    return hr;
}

__checkReturn HRESULT
CFreetypeFontFace::GetOrCreateFormat(
    __in const CFontDescription* pDescription,
    __in ITextureAllocator* pAllocator,
    __deref_out CFreetypeTextFormat** ppFormat
    )
{
    HRESULT hr = S_OK;
    CFreetypeTextFormat* pFormat = NULL;

    for (list< CFreetypeTextFormat* >::iterator it = m_CachedFormats.begin(); it != m_CachedFormats.end(); ++it)
    {
        //
        // Allow for almost similar fonts to use the same font format.
        //
        if (fabs(pDescription->GetFontSize() - (*it)->GetFontSize()) < 0.1f)
        {
            SetObject(pFormat, *it);

            break;
        }
    }

    //
    // Create a new format as no suitable cached version was found.
    //
    if (pFormat == NULL)
    {
        IFC(CFreetypeTextFormat::Create(this, pDescription->GetFontSize(), pAllocator, &pFormat));

        //
        // Keep a weak reference to the format for caching.
        //
        m_CachedFormats.push_back(pFormat);
    }

    *ppFormat = pFormat;
    pFormat = NULL;

Cleanup:
    ReleaseObject(pFormat);

    return hr;
}

void
CFreetypeFontFace::RemoveCachedFormat(
    __in CFreetypeTextFormat* pFormat
    )
{
    for (list< CFreetypeTextFormat* >::iterator it = m_CachedFormats.begin(); it != m_CachedFormats.end(); ++it)
    {
        if (*it == pFormat)
        {
            m_CachedFormats.erase(it);

            break;
        }
    }
}