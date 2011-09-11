#include "LibXMLReader.h"

#include <iconv.h>

template< typename T, unsigned int Size >
class ParseBuffer
{
    public:
        ParseBuffer(
            ) 
            : m_UseInternalBuffer(TRUE)
            , m_pHeapBuffer(NULL)
            , m_HeapBufferSize(0)
        {
        }
    
        ~ParseBuffer(
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
        			delete m_pHeapBuffer;
        			m_pHeapBuffer = NULL;

        			m_HeapBufferSize = 0;
        		}

        		if (m_pHeapBuffer == NULL)
        		{
        			m_pHeapBuffer = new T(BufferSize);
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
        BOOL m_UseInternalBuffer;
        T m_InternalBuffer[Size];
        T* m_pHeapBuffer;
        UINT32 m_HeapBufferSize;
};

CLibXMLReader::CLibXMLReader(
	) 
{
}

CLibXMLReader::~CLibXMLReader(
	)
{
}

__checkReturn HRESULT 
CLibXMLReader::Initialize(
	)
{
    HRESULT hr = S_OK;

    return hr;
}

__checkReturn HRESULT 
CLibXMLReader::LoadFromFile(
	__in_z const WCHAR* pPath, 
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;
    xmlTextReaderPtr pReader = NULL;
    iconv_t Converter = NULL;
    size_t ConversionResult = 0;  
    ParseBuffer< CHAR, 2048 > ConvertedTextBuffer;
    
    IFCPTR(pPath);
    IFCPTR(pCallback);
    
    {
        size_t PathTextLength = wcslen(pPath) * sizeof(WCHAR);
        size_t ConvertedTextLength = 0;

        IFC(ConvertedTextBuffer.EnsureBufferSize(PathTextLength + 1));

        {
            CHAR* pInputBuffer = (CHAR*)pPath;
		    CHAR* pOutputBuffer = ConvertedTextBuffer.GetBuffer();        
		    size_t ConvertedTextBufferSize = ConvertedTextBuffer.GetBufferByteSize();  
		    
		    Converter = iconv_open("utf-8", "WCHAR_T");
		       
		    ConversionResult = iconv(Converter, &pInputBuffer, &PathTextLength, &pOutputBuffer, &ConvertedTextBufferSize);

		    IFCEXPECT(ConversionResult != (size_t)-1);
		    
		    ConvertedTextLength = (ConvertedTextBuffer.GetBufferByteSize() - ConvertedTextBufferSize);
		    
		    IFCEXPECT(ConvertedTextLength < ConvertedTextBuffer.GetBufferSize() - 1);
		    
		    ConvertedTextBuffer.GetBuffer()[ConvertedTextLength] = L'\0';
		    
		    pReader = xmlNewTextReaderFilename(ConvertedTextBuffer.GetBuffer());
		    
		    IFC(ProcessReader(pReader, pCallback));
		}
    }
    
    IFC(ProcessReader(pReader, pCallback));
    
Cleanup:
    if (pReader != NULL)
    {
        xmlFreeTextReader(pReader);
    }
    
    if (Converter != NULL)
    {
        iconv_close(Converter);
    }
    
    return hr;
}

__checkReturn HRESULT 
CLibXMLReader::LoadFromString(
	__in_z const WCHAR* pText, 
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;
    xmlTextReaderPtr pReader = NULL;
    iconv_t Converter = NULL;
    size_t ConversionResult = 0;  
    ParseBuffer< CHAR, 2048 > ConvertedTextBuffer; 
    xmlParserInputBuffer* pBuffer = NULL;
    
    IFCPTR(pText);
    IFCPTR(pCallback);
    
    {
        size_t TextLength = wcslen(pText) * sizeof(WCHAR);
        size_t ConvertedTextLength = 0;

        IFC(ConvertedTextBuffer.EnsureBufferSize(TextLength + 1));

        {
            CHAR* pInputBuffer = (CHAR*)pText;
		    CHAR* pOutputBuffer = ConvertedTextBuffer.GetBuffer();        
		    size_t ConvertedTextBufferSize = ConvertedTextBuffer.GetBufferByteSize();  
		    
		    Converter = iconv_open("utf-8", "WCHAR_T");
		       
		    ConversionResult = iconv(Converter, &pInputBuffer, &TextLength, &pOutputBuffer, &ConvertedTextBufferSize);

		    IFCEXPECT(ConversionResult != (size_t)-1);
		    
		    ConvertedTextLength = (ConvertedTextBuffer.GetBufferByteSize() - ConvertedTextBufferSize);
		    
		    IFCEXPECT(ConvertedTextLength < ConvertedTextBuffer.GetBufferSize() - 1);
		    
		    ConvertedTextBuffer.GetBuffer()[ConvertedTextLength] = L'\0';
        
        	pBuffer = xmlParserInputBufferCreateMem(ConvertedTextBuffer.GetBuffer(), ConvertedTextLength, XML_CHAR_ENCODING_UTF8);
        
        	pReader = xmlNewTextReader(pBuffer, NULL);
        
        	IFC(ProcessReader(pReader, pCallback));
        }
    }
    
    IFC(ProcessReader(pReader, pCallback));
    
Cleanup:
    if (pReader != NULL)
    {
        xmlFreeTextReader(pReader);
    }
    
    if (pBuffer != NULL)
    {
        delete pBuffer;
    }
    
    if (Converter != NULL)
    {
        iconv_close(Converter);
    }
    
    return hr;
}

__checkReturn HRESULT 
CLibXMLReader::ProcessReader(
	__in xmlTextReaderPtr pReader,
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;
    iconv_t Converter = NULL;
    ParseBuffer< WCHAR, 2048 > ConversionBuffer;
    ParseBuffer< WCHAR, 2048 > AttributeNameBuffer;
    ParseBuffer< WCHAR, 2048 > AttributeValueBuffer;
    ParseBuffer< WCHAR, 2048 > AttributePrefixBuffer;
    ParseBuffer< WCHAR, 2048 > AttributeNamespaceUriBuffer;

    IFCPTR(pReader);
    IFCPTR(pCallback);
    
    Converter = iconv_open("WCHAR_T", "utf-8");

    do
    {
        UINT32 NodeType = xmlTextReaderNodeType(pReader);
        
        switch(NodeType)
        {
            case XML_READER_TYPE_ELEMENT:
                {
                    const xmlChar* pName = NULL;
                    size_t NameLength = 0;
                    BOOL EmptyElement = FALSE;
                    size_t ConversionResult = 0;
                    size_t ConvertedTextLength = 0;

                    EmptyElement = xmlTextReaderIsEmptyElement(pReader);

                    pName = xmlTextReaderConstLocalName(pReader);
                    
                    NameLength = xmlStrlen(pName);
                    
                    IFC(ConversionBuffer.EnsureBufferSize(NameLength + 1));
                    
                    {
                        CHAR* pInputBuffer = (CHAR*)pName;
                        CHAR* pOutputBuffer = (CHAR*)ConversionBuffer.GetBuffer();
                        size_t ConvertedTextBufferSize = ConversionBuffer.GetBufferByteSize();
                                       
                        ConversionResult = iconv(Converter, &pInputBuffer, &NameLength, &pOutputBuffer, &ConvertedTextBufferSize);
                        
                        IFCEXPECT(ConversionResult != (size_t)-1);
                        
                        ConvertedTextLength = (ConversionBuffer.GetBufferByteSize() - ConvertedTextBufferSize) / sizeof(WCHAR);
                        
                        ConversionBuffer.GetBuffer()[ConvertedTextLength] = L'\0';                        
                        
                        IFC(RaiseElementStart(ConversionBuffer.GetBuffer(), ConvertedTextLength, pCallback));
                        
                        for (INT32 ContinueAttributes = xmlTextReaderMoveToFirstAttribute(pReader); ContinueAttributes == 1; ContinueAttributes = xmlTextReaderMoveToNextAttribute(pReader))
                        {
                            const xmlChar* pAttributeName = xmlTextReaderConstLocalName(pReader);
                            const xmlChar* pAttributeValue = xmlTextReaderConstValue(pReader);
                            const xmlChar* pAttributePrefix = xmlTextReaderConstPrefix(pReader);
                            const xmlChar* pAttributeNamespaceUri = xmlTextReaderConstNamespaceUri(pReader);
                            
                            IFCPTR(pAttributeName);
                            IFCPTR(pAttributeValue);
                            
                            {   
                                size_t AttributeNameLength = xmlStrlen(pAttributeName);
                                size_t AttributeValueLength = xmlStrlen(pAttributeValue);
                                size_t AttributePrefixLength = (pAttributePrefix != NULL) ? xmlStrlen(pAttributePrefix) : 0;
                                size_t AttributeNamespaceUriLength = (pAttributeNamespaceUri != NULL) ? xmlStrlen(pAttributeNamespaceUri) : 0;
                                
                                size_t AttributeNameConvertedLength = 0;
                                size_t AttributeValueConvertedLength = 0;
                                size_t AttributePrefixConvertedLength = 0;
                                size_t AttributeNamespaceUriConvertedLength = 0;
                                
                                IFC(AttributeNameBuffer.EnsureBufferSize(AttributeNameLength + 1));
                                IFC(AttributeValueBuffer.EnsureBufferSize(AttributeValueLength + 1));
                                IFC(AttributePrefixBuffer.EnsureBufferSize(AttributePrefixLength + 1));
                                IFC(AttributeNamespaceUriBuffer.EnsureBufferSize(AttributeNamespaceUriLength + 1));
                                
                                {
                                    CHAR* pAttributeTextInputBuffer = (CHAR*)pAttributeName;                                
                                    CHAR* pAttributeTextOutBuffer = (CHAR*)AttributeNameBuffer.GetBuffer();
                                    size_t InputBufferSize = AttributeNameLength;
                                    size_t ConversionBufferSize = AttributeNameBuffer.GetBufferByteSize();
                                    
                                    ConversionResult = iconv(Converter, &pAttributeTextInputBuffer, &InputBufferSize, &pAttributeTextOutBuffer, &ConversionBufferSize);
                                    
                                    IFCEXPECT(ConversionResult != (size_t)-1);
                                    
                                    AttributeNameConvertedLength = (AttributeNameBuffer.GetBufferByteSize() - ConversionBufferSize) / sizeof(WCHAR);
                                }
                                
                                AttributeNameBuffer.GetBuffer()[AttributeNameConvertedLength] = L'\0';
                                
                                {
                                    CHAR* pAttributeTextInputBuffer = (CHAR*)pAttributeValue;                                
                                    CHAR* pAttributeTextOutBuffer = (CHAR*)AttributeValueBuffer.GetBuffer();
                                    size_t InputBufferSize = AttributeValueLength;
                                    size_t ConversionBufferSize = AttributeValueBuffer.GetBufferByteSize();
                                    
                                    ConversionResult = iconv(Converter, &pAttributeTextInputBuffer, &InputBufferSize, &pAttributeTextOutBuffer, &ConversionBufferSize);
                                    
                                    IFCEXPECT(ConversionResult != (size_t)-1);
                                    
                                    AttributeValueConvertedLength = (AttributeValueBuffer.GetBufferByteSize() - ConversionBufferSize) / sizeof(WCHAR);
                                }
                                
                                AttributeValueBuffer.GetBuffer()[AttributeValueConvertedLength] = L'\0';  
                                
                                if (pAttributePrefix != NULL)
                                {
                                    CHAR* pAttributeTextInputBuffer = (CHAR*)pAttributePrefix;                                
                                    CHAR* pAttributeTextOutBuffer = (CHAR*)AttributePrefixBuffer.GetBuffer();
                                    size_t InputBufferSize = AttributePrefixLength;
                                    size_t ConversionBufferSize = AttributePrefixBuffer.GetBufferByteSize();
                                    
                                    ConversionResult = iconv(Converter, &pAttributeTextInputBuffer, &InputBufferSize, &pAttributeTextOutBuffer, &ConversionBufferSize);
                                    
                                    IFCEXPECT(ConversionResult != (size_t)-1);
                                    
                                    AttributePrefixConvertedLength = (AttributePrefixBuffer.GetBufferByteSize() - ConversionBufferSize) / sizeof(WCHAR);
                                }
                                
                                AttributePrefixBuffer.GetBuffer()[AttributePrefixConvertedLength] = L'\0';  
                                
                                if (pAttributeNamespaceUri != NULL)
                                {
                                    CHAR* pAttributeTextInputBuffer = (CHAR*)pAttributeNamespaceUri;                                
                                    CHAR* pAttributeTextOutBuffer = (CHAR*)AttributeNamespaceUriBuffer.GetBuffer();
                                    size_t InputBufferSize = AttributeNamespaceUriLength;
                                    size_t ConversionBufferSize = AttributeNamespaceUriBuffer.GetBufferByteSize();
                                    
                                    ConversionResult = iconv(Converter, &pAttributeTextInputBuffer, &InputBufferSize, &pAttributeTextOutBuffer, &ConversionBufferSize);
                                    
                                    IFCEXPECT(ConversionResult != (size_t)-1);
                                    
                                    AttributeNamespaceUriConvertedLength = (AttributeNamespaceUriBuffer.GetBufferByteSize() - ConversionBufferSize) / sizeof(WCHAR);
                                }
                                
                                AttributeNamespaceUriBuffer.GetBuffer()[AttributeNamespaceUriConvertedLength] = L'\0';      
                                
                                IFC(RaiseAttribute(AttributeNameBuffer.GetBuffer(), AttributeNameConvertedLength, AttributeValueBuffer.GetBuffer(), AttributeValueConvertedLength, AttributePrefixBuffer.GetBuffer(), AttributePrefixConvertedLength, AttributeNamespaceUriBuffer.GetBuffer(), AttributeNamespaceUriConvertedLength, pCallback));
                            }
                        }
                        
                        if(EmptyElement)
                        {
                            IFC(RaiseElementEnd(ConversionBuffer.GetBuffer(), ConvertedTextLength, pCallback));
                        }                        
                    }

                    break;
                }

            case XML_READER_TYPE_END_ELEMENT:
                {
                    const xmlChar* pName = NULL;
                    size_t NameLength = 0;
                    BOOL EmptyElement = FALSE;
                    size_t ConversionResult = 0;
                    size_t ConvertedTextLength = 0;
                    
                    EmptyElement = xmlTextReaderIsEmptyElement(pReader);
                    
                    pName = xmlTextReaderConstLocalName(pReader);
                    
                    NameLength = xmlStrlen(pName);
                    
                    IFC(ConversionBuffer.EnsureBufferSize(NameLength + 1));
                    
                    {
                        CHAR* pInputBuffer = (CHAR*)pName;
                        CHAR* pOutputBuffer = (CHAR*)ConversionBuffer.GetBuffer();
                        size_t ConvertedTextBufferSize = ConversionBuffer.GetBufferByteSize();
                        
                        ConversionResult = iconv(Converter, &pInputBuffer, &NameLength, &pOutputBuffer, &ConvertedTextBufferSize);
                        
                        IFCEXPECT(ConversionResult != (size_t)-1);
                        
                        ConvertedTextLength = (ConversionBuffer.GetBufferByteSize() - ConvertedTextBufferSize) / sizeof(WCHAR);
                        
                        ConversionBuffer.GetBuffer()[ConvertedTextLength] = L'\0';    
                        
                        IFC(RaiseElementEnd(ConversionBuffer.GetBuffer(), ConvertedTextLength, pCallback));                        
                    }
                    
                    break;
                }

            case XML_READER_TYPE_TEXT:
                {
                    const xmlChar* pValue = NULL;
                    size_t ValueLength = 0;
                    size_t ConversionResult = 0;
                    size_t ConvertedTextLength = 0;
                    
                    if (xmlTextReaderHasValue(pReader))
                    {
                        pValue = xmlTextReaderConstValue(pReader);
                        
                        ValueLength = xmlStrlen(pValue);
                        
                        IFC(ConversionBuffer.EnsureBufferSize(ValueLength + 1));
                        
                        {
                            CHAR* pInputBuffer = (CHAR*)pValue;
                            CHAR* pOutputBuffer = (CHAR*)ConversionBuffer.GetBuffer();
                            size_t ConvertedTextBufferSize = ConversionBuffer.GetBufferByteSize();
                            
                            ConversionResult = iconv(Converter, &pInputBuffer, &ValueLength, &pOutputBuffer, &ConvertedTextBufferSize);
                            
                            IFCEXPECT(ConversionResult != (size_t)-1);
                            
                            ConvertedTextLength = (ConversionBuffer.GetBufferByteSize() - ConvertedTextBufferSize) / sizeof(WCHAR);
                            
                            ConversionBuffer.GetBuffer()[ConvertedTextLength] = L'\0';    
                            
                            IFC(RaiseElementEnd(ConversionBuffer.GetBuffer(), ConvertedTextLength, pCallback));                        
                        }
                    }
                    
                    break;
                }
        }
        
        if(xmlTextReaderRead(pReader) != 1)
        {
            break;
        }
    }
    while(SUCCEEDED(hr));

Cleanup:
    if (Converter != NULL)
    {
        iconv_close(Converter);
    }

    return hr;
}

__checkReturn HRESULT 
CLibXMLReader::RaiseElementStart(
	__in_ecount(NameLength) const WCHAR* pName, 
	UINT32 NameLength, 
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pName);
    IFCPTR(pCallback);
    
    {
        CLibXMLElementStart ElementStart(pName, NameLength);

        IFC(pCallback->OnElementStart(&ElementStart));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT
CLibXMLReader::RaiseElementEnd(
	__in_ecount(NameLength) const WCHAR* pName,
	UINT32 NameLength,
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pName);
    IFCPTR(pCallback);
    
    {
        CLibXMLElementEnd ElementEnd(pName, NameLength);

        IFC(pCallback->OnElementEnd(&ElementEnd));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CLibXMLReader::RaiseText(
	__in_ecount(TextLength) const WCHAR* pText,
	UINT32 TextLength,
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pText);
    IFCPTR(pCallback);
    
    {
        CLibXMLText Text(pText, TextLength);

        IFC(pCallback->OnText(&Text));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CLibXMLReader::RaiseAttribute(
	__in_ecount(NameLength) const WCHAR* pName,
	UINT32 NameLength,
	__in_ecount(ValueLength) const WCHAR* pValue,
	UINT32 ValueLength,
	__in_ecount(PrefixLength) const WCHAR* pPrefix,
	UINT32 PrefixLength,
	__in_ecount(NamespaceUriLength) const WCHAR* pNamespaceUri,
	UINT32 NamespaceUriLength,
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pCallback);
    
    {
        CLibXMLAttribute Attribute(pName, NameLength, pValue, ValueLength, pPrefix, PrefixLength, pNamespaceUri, NamespaceUriLength);

        IFC(pCallback->OnAttribute(&Attribute));
    }

Cleanup:
    return hr;
}

CLibXMLElementStart::CLibXMLElementStart(
	__in_ecount(NameLength) const WCHAR* pElementName, 
	UINT32 NameLength
	) 
	: m_Name(pElementName)
	, m_NameLength(NameLength)
{
}

__override __checkReturn HRESULT 
CLibXMLElementStart::GetName(
	__deref_out_ecount(*pNameBufferLength) const WCHAR** ppNameBuffer, 
	__out UINT32* pNameBufferLength
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppNameBuffer);
    IFCPTR(pNameBufferLength);

    *ppNameBuffer = m_Name;
    *pNameBufferLength = m_NameLength;

Cleanup:
    return hr;
}

CLibXMLElementEnd::CLibXMLElementEnd(
	__in_ecount(NameLength) const WCHAR* pElementName, 
	UINT32 NameLength
	) 
	: m_Name(pElementName)
	, m_NameLength(NameLength)
{
}

__override __checkReturn HRESULT 
CLibXMLElementEnd::GetName(
	__deref_out_ecount(*pNameBufferLength) const WCHAR** ppNameBuffer,
	__out UINT32* pNameBufferLength
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppNameBuffer);
    IFCPTR(pNameBufferLength);

    *ppNameBuffer = m_Name;
    *pNameBufferLength = m_NameLength;

Cleanup:
    return hr;
}

CLibXMLText::CLibXMLText(
	__in_ecount(TextLength) const WCHAR* pText, 
	UINT32 TextLength 
	) 
	: m_Text(pText)
	, m_TextLength(TextLength)
{
}

__override __checkReturn HRESULT 
CLibXMLText::GetText(
	__deref_out_ecount(*pTextLength) const WCHAR** ppText, 
	__out UINT32* pTextLength
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppText);
    IFCPTR(pTextLength);

    *ppText = m_Text;
    *pTextLength = m_TextLength;

Cleanup:
    return hr;
}

CLibXMLAttribute::CLibXMLAttribute(
	__in_ecount(NameLength) const WCHAR* pName,
	UINT32 NameLength,
	__in_ecount(ValueLength) const WCHAR* pValue,
	UINT32 ValueLength,
	__in_ecount(PrefixLength) const WCHAR* pPrefix,
	UINT32 PrefixLength,
	__in_ecount(NamespaceUriLength) const WCHAR* pNamespaceUri,
    UINT32 NamespaceUriLength
	) 
	: m_pName(pName)
	, m_NameLength(NameLength)
	, m_pValue(pValue)
	, m_ValueLength(ValueLength)
	, m_pPrefix(pPrefix)
	, m_PrefixLength(PrefixLength)
	, m_pNamespaceUri(pNamespaceUri)
	, m_NamespaceUriLength(NamespaceUriLength)
{
}

__override __checkReturn HRESULT
CLibXMLAttribute::GetName(
	__deref_out_ecount(*pNameBufferLength) const WCHAR** ppNameBuffer, 
	__out UINT32* pNameBufferLength
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppNameBuffer);
    IFCPTR(pNameBufferLength);

    *ppNameBuffer = m_pName;
    *pNameBufferLength = m_NameLength;

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CLibXMLAttribute::GetValue(
	__deref_out_ecount(*pValueBufferLength) const WCHAR** ppValueBuffer, 
	__out UINT32* pValueBufferLength
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppValueBuffer);
    IFCPTR(pValueBufferLength);

    *ppValueBuffer = m_pValue;
    *pValueBufferLength = m_ValueLength;

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CLibXMLAttribute::GetPrefix(
	__deref_out_ecount(*pPrefixBufferLength) const WCHAR** ppPrefix, 
	__out UINT32* pPrefixBufferLength
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppPrefix);
    IFCPTR(pPrefixBufferLength);

    *ppPrefix = m_pPrefix;
    *pPrefixBufferLength = m_PrefixLength;

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CLibXMLAttribute::GetNamespaceUri(
	__deref_out_ecount(*pNamespaceUriLength) const WCHAR** ppNamespaceUri, 
	__out UINT32* pNamespaceUriLength
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppNamespaceUri);
    IFCPTR(pNamespaceUriLength);

    *ppNamespaceUri = m_pNamespaceUri;
    *pNamespaceUriLength = m_NamespaceUriLength;

Cleanup:
    return hr;
}