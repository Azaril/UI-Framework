#include "FreetypeFontFace.h"
#include "StackHeapBuffer.h"

CFreetypeFontFace::CFreetypeFontFace(
    )
    : m_pFontFace(NULL)
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
CFreetypeFontFace::SetFontSize(
    const FLOAT& fontSize
    )
{
    HRESULT hr = S_OK;

    //TODO: Query DPI?
    IFCEXPECT(FT_Set_Char_Size(m_pFontFace, 0, FT_F26Dot6(64.0f * fontSize), 96, 96) == 0);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CFreetypeFontFace::LoadGlyph(
    const WCHAR glyph
    )
{
    HRESULT hr = S_OK;
    UINT32 glyphIndex = 0;

    glyphIndex = FT_Get_Char_Index(m_pFontFace, glyph);

    IFCEXPECT(FT_Load_Glyph(m_pFontFace, glyphIndex, FT_LOAD_DEFAULT) == 0)

Cleanup:
    return hr;
}

__checkReturn HRESULT
CFreetypeFontFace::LoadIntoTexture(
    __in ITextureAllocator* pAllocator,
    __deref_out ITexture** ppTexture
    )
{
    HRESULT hr = S_OK;
    FT_GlyphSlot pGlyphSlot = NULL;
    ITexture* pOutputTexture = NULL;
    StackHeapBuffer< BYTE, 1024 * 4 > bitmapBuffer;

    pGlyphSlot = m_pFontFace->glyph;

    IFCEXPECT(FT_Render_Glyph(pGlyphSlot, FT_RENDER_MODE_NORMAL) == 0);

    {
        const FT_Bitmap& bitmap = pGlyphSlot->bitmap;

        UINT32 width = bitmap.width;
        UINT32 height = bitmap.rows;

        IFC(pAllocator->AllocateTexture(width, height, &pOutputTexture));

        switch(pOutputTexture->GetPixelFormat())
        {
            case PixelFormat::B8G8R8A8:
                {
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

                            pDestination[0] = val;
                            pDestination[1] = val;
                            pDestination[2] = val;
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

    *ppTexture = pOutputTexture;
    pOutputTexture = NULL;

Cleanup:
    ReleaseObject(pOutputTexture);

    return hr;
}