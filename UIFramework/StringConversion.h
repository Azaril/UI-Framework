#pragma once

#include "Types.h"
#include "StackHeapBuffer.h"

#if defined(_WINDOWS) || defined(_XBOX)

template< unsigned int V >
__checkReturn HRESULT
ConvertUTF8ToWCHAR(
    __in_ecount(stringLength) const CHAR* pSourceString,
    UINT32 stringLength,
    __out StackHeapBuffer< WCHAR, V >* pBuffer,
    __out_opt UINT32* pStringLength
    )
{
    HRESULT hr = S_OK;
    INT32 outputCharCount = 0;

    outputCharCount = MultiByteToWideChar(CP_UTF8, 0, pSourceString, stringLength, NULL, 0);

    IFC(pBuffer->EnsureBufferSize(outputCharCount + 1));

    outputCharCount = MultiByteToWideChar(CP_UTF8, 0, pSourceString, stringLength, pBuffer->GetBuffer(), pBuffer->GetBufferSize());
    IFCEXPECT(outputCharCount > 0);

    pBuffer->GetBuffer()[outputCharCount] = L'\0';

    if (pStringLength != NULL)
    {
        *pStringLength = outputCharCount + 1;
    }
   
Cleanup:    
    return hr;
}

template< unsigned int V >
__checkReturn HRESULT
ConvertUTF16ToWCHAR(
    __in_ecount(stringLength) const UINT16* pSourceString,
    UINT32 stringLength,
    __out StackHeapBuffer< WCHAR, V >* pBuffer,
    __out_opt UINT32* pStringLength
    )
{
    HRESULT hr = S_OK;

    STATIC_ASSERT(sizeof(UINT16) == sizeof(WCHAR));

    IFC(pBuffer->EnsureBufferSize(stringLength + 1));

    memcpy(pBuffer->GetBuffer(), pSourceString, stringLength * sizeof(WCHAR));

    pBuffer->GetBuffer()[stringLength] = L'\0';

    if (pStringLength != NULL)
    {
        *pStringLength = stringLength + 1;
    }
   
Cleanup:    
    return hr;
}

template< unsigned int V >
__checkReturn HRESULT
ConvertUTF16BEToWCHAR(
    __in_ecount(stringLength) const UINT16* pSourceString,
    UINT32 stringLength,
    __out StackHeapBuffer< WCHAR, V >* pBuffer,
    __out_opt UINT32* pStringLength
    )
{
    HRESULT hr = S_OK;

    STATIC_ASSERT(sizeof(UINT16) == sizeof(WCHAR));

    IFC(pBuffer->EnsureBufferSize(stringLength + 1));

    {
        const WCHAR* pReadBuffer = (const WCHAR*)pSourceString;
        WCHAR* pWriteBuffer = pBuffer->GetBuffer();

        for (UINT32 i = 0; i < stringLength; ++i)
        {
#if defined(PLATFORM_LITTLE_ENDIAN)
            WCHAR val = *pReadBuffer;
            *pWriteBuffer = ((val & 0xFF00) >> 8) | ((val & 0x00FF) << 8);
#elif defined(PLATFORM_BIG_ENDIAN)
            *pWriteBuffer = *pReadBuffer;
#else
#error Unknown endianness!
#endif

            ++pReadBuffer;
            ++pWriteBuffer;
        }
    }

    pBuffer->GetBuffer()[stringLength] = L'\0';

    if (pStringLength != NULL)
    {
        *pStringLength = stringLength + 1;
    }
   
Cleanup:    
    return hr;
}

template< unsigned int V >
__checkReturn HRESULT
ConvertUTF16LEToWCHAR(
    __in_ecount(stringLength) const UINT16* pSourceString,
    UINT32 stringLength,
    __out StackHeapBuffer< WCHAR, V >* pBuffer,
    __out_opt UINT32* pStringLength
    )
{
    HRESULT hr = S_OK;

    STATIC_ASSERT(sizeof(UINT16) == sizeof(WCHAR));

    IFC(pBuffer->EnsureBufferSize(stringLength + 1));

    {
        const WCHAR* pReadBuffer = (const WCHAR*)pSourceString;
        WCHAR* pWriteBuffer = pBuffer->GetBuffer();

        for (UINT32 i = 0; i < stringLength; ++i)
        {
#if defined(PLATFORM_LITTLE_ENDIAN)
            *pWriteBuffer = *pReadBuffer;
#elif defined(PLATFORM_BIG_ENDIAN)
			WCHAR val = *pReadBuffer;
			*pWriteBuffer = ((val & 0xFF00) >> 8) | ((val & 0x00FF) << 8);
#else
#error Unknown endianness!
#endif

            ++pReadBuffer;
            ++pWriteBuffer;
        }
    }

    pBuffer->GetBuffer()[stringLength] = L'\0';

    if (pStringLength != NULL)
    {
        *pStringLength = stringLength + 1;
    }
   
Cleanup:    
    return hr;
}

template< unsigned int V >
__checkReturn HRESULT
ConvertWCHARToUTF8(
	__in_ecount(stringLength) const WCHAR* pSourceString,
	UINT32 stringLength,
	__out StackHeapBuffer< CHAR, V >* pBuffer,
	__out_opt UINT32* pStringLength
	)
{
	HRESULT hr = S_OK;
	INT32 outputCharCount = 0;

	outputCharCount = WideCharToMultiByte(CP_UTF8, 0, pSourceString, stringLength, NULL, 0, NULL, NULL);

	IFC(pBuffer->EnsureBufferSize(outputCharCount + 1));

	outputCharCount = WideCharToMultiByte(CP_UTF8, 0, pSourceString, stringLength, pBuffer->GetBuffer(), pBuffer->GetBufferSize(), NULL, NULL);
	IFCEXPECT(outputCharCount > 0);

	pBuffer->GetBuffer()[outputCharCount] = L'\0';

	if (pStringLength != NULL)
	{
		*pStringLength = outputCharCount + 1;
	}

Cleanup:    
	return hr;
}

#else

#include <iconv.h>

template< unsigned int V >
__checkReturn HRESULT
ConvertWCHARToUTF8(
    __in_ecount(stringLength) const WCHAR* pSourceString,
    UINT32 stringLength,
    __out StackHeapBuffer< CHAR, V >* pBuffer,
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
        
        Converter = iconv_open("UTF-8", "WCHAR_T");
        
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

template< unsigned int V >
__checkReturn HRESULT
ConvertUTF8ToWCHAR(
    __in_ecount(stringLength) const CHAR* pSourceString,
    UINT32 stringLength,
    __out StackHeapBuffer< WCHAR, V >* pBuffer,
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
        
        Converter = iconv_open("WCHAR_T", "UTF-8");
        
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

template< unsigned int V >
__checkReturn HRESULT
ConvertUTF16BEToWCHAR(
    __in_ecount(stringLength) const UINT16* pSourceString,
    UINT32 stringLength,
    __out StackHeapBuffer< WCHAR, V >* pBuffer,
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
        
        size_t SourceTextBufferSize = InputStringLength * sizeof(UINT16);
        size_t ConvertedTextBufferSize = pBuffer->GetBufferByteSize();  
        
        Converter = iconv_open("WCHAR_T", "UTF-16BE");
        
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

#endif

template< unsigned int V >
__checkReturn HRESULT
ConvertUTF8ToWCHAR(
    __in_z const CHAR* pSourceString,
    __out StackHeapBuffer< WCHAR, V >* pBuffer,
    __out_opt UINT32* pStringLength
    )
{
    HRESULT hr = S_OK;
    UINT32 stringLength = 0;
    
    stringLength = strlen(pSourceString);
    
    IFC(ConvertUTF8ToWCHAR< V >(pSourceString, stringLength, pBuffer, pStringLength));
    
Cleanup:
    return hr;
}

template< unsigned int V >
__checkReturn HRESULT
ConvertWCHARToUTF8(
    __in_z const WCHAR* pSourceString,
    __out StackHeapBuffer< CHAR, V >* pBuffer,
    __out_opt UINT32* pStringLength
    )
{
    HRESULT hr = S_OK;
    UINT32 stringLength = 0;
    
    stringLength = wcslen(pSourceString);
    
    IFC(ConvertWCHARToUTF8< V >(pSourceString, stringLength, pBuffer, pStringLength));
    
Cleanup:
    return hr;
}
