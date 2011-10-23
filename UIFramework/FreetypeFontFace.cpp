#include "FreetypeFontFace.h"
#include "StackHeapBuffer.h"

CFreetypeFontFace::CFreetypeFontFace(
    )
    : m_pFontFace(NULL)
    , m_CurrentGlyph((UINT32)-1)
    , m_CurrentFontSize(-1.0f)
{
}

CFreetypeFontFace::~CFreetypeFontFace(
    )
{
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
    FLOAT realFontSize = std::fabsf(fontSize);

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