#pragma once

#include "ErrorChecking.h"

template< typename T, size_t Size >
class StackHeapBuffer
{
    public:
        static const size_t StackSize = Size;
        typedef T Type;

        StackHeapBuffer(
            ) 
            : m_UseInternalBuffer(TRUE)
            , m_pHeapBuffer(NULL)
            , m_HeapBufferSize(0)
        {
        }
    
        ~StackHeapBuffer(
             )
        {
            delete [] m_pHeapBuffer;
        }
    
        __checkReturn HRESULT EnsureBufferSize(
        	size_t BufferSize
        	)
        {
        	HRESULT hr = S_OK;

        	if (BufferSize <= ARRAYSIZE(m_InternalBuffer))
        	{
        		m_UseInternalBuffer = TRUE;
        	}
        	else
        	{
        		m_UseInternalBuffer = FALSE;

        		if (BufferSize > m_HeapBufferSize)
        		{
        			delete [] m_pHeapBuffer;
        			m_pHeapBuffer = NULL;

        			m_HeapBufferSize = 0;
        		}

        		if (m_pHeapBuffer == NULL)
        		{
        			m_pHeapBuffer = new T[BufferSize];
                    IFCOOM(m_pHeapBuffer);

        			m_HeapBufferSize = BufferSize;
        		}
        	}

        Cleanup:
        	return hr;
        }

        size_t GetBufferSize(
        	)
        {
        	if(m_UseInternalBuffer)
        	{
        		return ARRAYSIZE(m_InternalBuffer);
        	}
        	else
        	{
        		return m_HeapBufferSize;
        	}
        }

        size_t GetBufferByteSize(
        	)
        {
        	return GetBufferSize() * sizeof(T);
        }

        __out T* GetBuffer(
        	)
    	{
    		if (m_UseInternalBuffer)	
    		{
    			return m_InternalBuffer;
    		}
    		else
    		{
    			return m_pHeapBuffer;
    		}
    	}
    
    protected:
        bool m_UseInternalBuffer;
        T m_InternalBuffer[Size];
        T* m_pHeapBuffer;
        UINT32 m_HeapBufferSize;
};
