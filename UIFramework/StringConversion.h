#pragma once

#include "Types.h"
#include "StackHeapBuffer.h"

#include <iconv.h>

template< typename T, unsigned int V >
__checkReturn HRESULT
ConvertWCHARToUTF8(
    __in_z const WCHAR* pSourceString,
    __out StackHeapBuffer< T, V >* pBuffer,
    __out_opt UINT32* pStringLength
    )
{
    HRESULT hr = S_OK;
    UINT32 stringLength = 0;

    stringLength = wcslen(pSourceString);

    IFC(ConvertWCHARToUTF8(pSourceString, stringLength, pBuffer, pStringLength));

Cleanup:
    return hr;
}

template< typename T, unsigned int V >
__checkReturn HRESULT
ConvertWCHARToUTF8(
    __in_ecount(stringLength) const WCHAR* pSourceString,
    UINT32 stringLength,
    __out StackHeapBuffer< T, V >* pBuffer,
    __out_opt UINT32* pStringLength
    )
{
    HRESULT hr = S_OK;
    iconv_t Converter = NULL;
    size_t ConversionResult = 0; 
    
    size_t InputStringLength = stringLength;
    size_t ConvertedTextLength = 0;
    
    IFC(pBuffer->EnsureBufferSize((InputStringLength * 2) + 1));
    
    {
        CHAR* pInputBuffer = (CHAR*)pSourceString;
        CHAR* pOutputBuffer = (CHAR*)pBuffer->GetBuffer();  
        
        size_t SourceTextBufferSize = InputStringLength * sizeof(WCHAR);
        size_t ConvertedTextBufferSize = pBuffer->GetBufferByteSize();  
        
        Converter = iconv_open("utf-8", "WCHAR_T");
        
        ConversionResult = iconv(Converter, &pInputBuffer, &SourceTextBufferSize, &pOutputBuffer, &ConvertedTextBufferSize);
        
        IFCEXPECT(ConversionResult != (size_t)-1);
        
        ConvertedTextLength = (pBuffer->GetBufferByteSize() - ConvertedTextBufferSize);
        
        IFCEXPECT(ConvertedTextLength < pBuffer->GetBufferSize() - 1);
        
        pBuffer->GetBuffer()[ConvertedTextLength] = '\0';
    }
    
    if (pStringLength != NULL)
    {
        *pStringLength = ConvertedTextLength + 1;
    }

Cleanup:
    if (Converter != NULL)
    {
        iconv_close(Converter);
    }
    
    return hr;
}

template< typename T, unsigned int V >
__checkReturn HRESULT
ConvertUTF8ToWCHAR(
    __in_z const CHAR* pSourceString,
    __out StackHeapBuffer< T, V >* pBuffer,
    __out_opt UINT32* pStringLength
    )
{
    HRESULT hr = S_OK;
    UINT32 stringLength = 0;

    stringLength = strlen(pSourceString);

    IFC(ConvertUTF8ToWCHAR(pSourceString, stringLength, pBuffer, pStringLength));

Cleanup:
    return hr;
}

template< typename T, unsigned int V >
__checkReturn HRESULT
ConvertUTF8ToWCHAR(
    __in_ecount(stringLength) const CHAR* pSourceString,
    UINT32 stringLength,
    __out StackHeapBuffer< T, V >* pBuffer,
    __out_opt UINT32* pStringLength
    )
{
    HRESULT hr = S_OK;
    iconv_t Converter = NULL;
    size_t ConversionResult = 0; 
    
    size_t InputStringLength = stringLength;
    size_t ConvertedTextLength = 0;
    
    IFC(pBuffer->EnsureBufferSize((InputStringLength + 1) * sizeof(WCHAR)));
    
    {
        CHAR* pInputBuffer = (CHAR*)pSourceString;
        CHAR* pOutputBuffer = (CHAR*)pBuffer->GetBuffer();  
        
        size_t SourceTextBufferSize = InputStringLength * sizeof(CHAR);
        size_t ConvertedTextBufferSize = pBuffer->GetBufferByteSize();  
        
        Converter = iconv_open("WCHAR_T", "utf-8");
        
        ConversionResult = iconv(Converter, &pInputBuffer, &SourceTextBufferSize, &pOutputBuffer, &ConvertedTextBufferSize);
        
        IFCEXPECT(ConversionResult != (size_t)-1);
        
        ConvertedTextLength = (pBuffer->GetBufferByteSize() - ConvertedTextBufferSize) / sizeof(WCHAR);
        
        IFCEXPECT(ConvertedTextLength < pBuffer->GetBufferSize() - 1);
        
        pBuffer->GetBuffer()[ConvertedTextLength] = L'\0';
    }
    
    if (pStringLength != NULL)
    {
        *pStringLength = ConvertedTextLength + 1;
    }

Cleanup:
    if (Converter != NULL)
    {
        iconv_close(Converter);
    }
    
    return hr;
}