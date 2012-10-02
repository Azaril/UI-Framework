#pragma once

#include <OpenGLES/ES2/gl.h>

#include "RefCounted.h"
#include "Factory.h"
#include "Texture.h"

class COpenGLES20Texture : public CRefCountedObjectBase< IBatchUpdateTexture >
{
    public:
        DECLARE_FACTORY4( COpenGLES20Texture, GLuint, UINT32, UINT32, PixelFormat::Value );
    
        GLuint GetTextureID(
            );
    
        __override virtual UINT32 GetWidth(
            );
        
        __override virtual UINT32 GetHeight(
            );
        
        __override virtual INT32 GetStride(
            );
        
        __override virtual PixelFormat::Value GetPixelFormat(
            );    

        __override virtual __checkReturn HRESULT SetData(
            __in_ecount(DataSize) const BYTE* pData,
            UINT32 DataSize,
            INT32 Stride
            );

        __override virtual __checkReturn HRESULT SetSubData(
            const RectU& Region,
            __in_ecount(DataSize) const BYTE* pData,
            UINT32 DataSize,
            INT32 Stride
            );    

        __override virtual __checkReturn HRESULT SetMultipleSubData(
            __in_ecount(RegionCount) const RectU* pRegions,
            __in_ecount(RegionCount) const BYTE** ppData,
            __in_ecount(RegionCount) const UINT32* pDataSizes,
            __in_ecount(RegionCount) const INT32* pStrides,
            UINT32 RegionCount
            );    

    protected:
    	COpenGLES20Texture(
	    	);

	    virtual ~COpenGLES20Texture(
		    );

		__checkReturn HRESULT Initialize(
			GLuint TextureID,
			UINT32 Width,
			UINT32 Height,
            PixelFormat::Value Format
			);

		GLuint m_TextureID;
        UINT32 m_Width;
        UINT32 m_Height;
        PixelFormat::Value m_PixelFormat;
};