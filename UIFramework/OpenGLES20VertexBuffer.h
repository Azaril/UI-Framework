#pragma once

#include <OpenGLES/ES2/gl.h>

#include "Factory.h"
#include "RenderVertex.h"
#include "RefCounted.h"
#include "VertexBuffer.h"

class COpenGLES20VertexBuffer : public CRefCountedObjectBase< IVertexBuffer >
{
    public:
        DECLARE_FACTORY1( COpenGLES20VertexBuffer, GLuint );
    
        __checkReturn HRESULT Bind(
           GLuint Buffer
           );
    
        __override virtual __checkReturn HRESULT SetVertices(
            __in_ecount(VertexCount) RenderVertex* pVertices,
            UINT32 VertexCount            
            );
    
        __override virtual UINT32 GetVertexCount(
            );
    
        __override virtual UINT32 GetMaximumVertices(
            );
        
    protected:
        COpenGLES20VertexBuffer(
            );
    
        virtual ~COpenGLES20VertexBuffer( 
            );
    
        __checkReturn HRESULT Initialize(
            GLuint BufferID
            );
    
        GLuint m_BufferID;
        UINT32 m_VertexCount;
};