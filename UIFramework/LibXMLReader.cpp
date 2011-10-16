#include "LibXMLReader.h"
#include "StackHeapBuffer.h"
#include "StringConversion.h"

typedef StackHeapBuffer< CHAR, 2048 > StackCharBuffer;
typedef StackHeapBuffer< WCHAR, 2048 > StackWCharBuffer;

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
    StackCharBuffer ConvertedTextBuffer;
    
    IFCPTR(pPath);
    IFCPTR(pCallback);
    
    hr = ConvertWCHARToUTF8< StackCharBuffer::Type, StackCharBuffer::StackSize >(pPath, &ConvertedTextBuffer, (UINT32*)NULL);
    IFC(hr);
    
    pReader = xmlNewTextReaderFilename(ConvertedTextBuffer.GetBuffer());
    IFCPTR(pReader);
    
    IFC(ProcessReader(pReader, pCallback));
    
Cleanup:
    if (pReader != NULL)
    {
        xmlFreeTextReader(pReader);
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
    StackCharBuffer ConvertedTextBuffer; 
    xmlParserInputBuffer* pBuffer = NULL;
    UINT32 ConvertedTextLength = 0;
    
    IFCPTR(pText);
    IFCPTR(pCallback);
    
    hr = ConvertWCHARToUTF8< StackCharBuffer::Type, StackCharBuffer::StackSize >(pText, &ConvertedTextBuffer, &ConvertedTextLength);
    IFC(hr);
    
    //
    // NOTE: Remove NULL terminator from string length.
    //
    pBuffer = xmlParserInputBufferCreateStatic(ConvertedTextBuffer.GetBuffer(), ConvertedTextLength - 1, XML_CHAR_ENCODING_UTF8);
    IFCOOM(pBuffer);

    pReader = xmlNewTextReader(pBuffer, NULL);
    IFCPTR(pReader);
    
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
    
    return hr;
}

__checkReturn HRESULT 
CLibXMLReader::ProcessReader(
	__in xmlTextReaderPtr pReader,
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;
    StackWCharBuffer ConversionBuffer;
    StackWCharBuffer AttributeNameBuffer;
    StackWCharBuffer AttributeValueBuffer;
    StackWCharBuffer AttributePrefixBuffer;
    StackWCharBuffer AttributeNamespaceUriBuffer;

    IFCPTR(pReader);
    IFCPTR(pCallback);

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
                    UINT32 ConvertedTextLength = 0;

                    EmptyElement = xmlTextReaderIsEmptyElement(pReader);

                    pName = xmlTextReaderConstLocalName(pReader);
                    
                    NameLength = xmlStrlen(pName);
                    
                    hr = ConvertUTF8ToWCHAR< StackWCharBuffer::Type, StackWCharBuffer::StackSize >((const CHAR*)pName, NameLength, &ConversionBuffer, &ConvertedTextLength);
                    IFC(hr);                    
                    
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
                            
                            UINT32 AttributeNameConvertedLength = 0;
                            UINT32 AttributeValueConvertedLength = 0;
                            UINT32 AttributePrefixConvertedLength = 0;
                            UINT32 AttributeNamespaceUriConvertedLength = 0;
                            
                            hr = ConvertUTF8ToWCHAR< StackWCharBuffer::Type, StackWCharBuffer::StackSize >((const CHAR*)pAttributeName, AttributeNameLength, &AttributeNameBuffer, &AttributeNameConvertedLength);
                            IFC(hr); 
                            
                            hr = ConvertUTF8ToWCHAR< StackWCharBuffer::Type, StackWCharBuffer::StackSize >((const CHAR*)pAttributeValue, AttributeValueLength, &AttributeValueBuffer, &AttributeValueConvertedLength);
                            IFC(hr);                             
                            
                            if (pAttributePrefix != NULL)
                            {
                                hr = ConvertUTF8ToWCHAR< StackWCharBuffer::Type, StackWCharBuffer::StackSize >((const CHAR*)pAttributePrefix, AttributePrefixLength, &AttributePrefixBuffer, &AttributePrefixConvertedLength);
                                IFC(hr);                                   
                            }
                            else
                            {
                                AttributePrefixBuffer.GetBuffer()[0] = L'\0';
                            }
                            
                            if (pAttributeNamespaceUri != NULL)
                            {
                                hr = ConvertUTF8ToWCHAR< StackWCharBuffer::Type, StackWCharBuffer::StackSize >((const CHAR*)pAttributeNamespaceUri, AttributeNamespaceUriLength, &AttributeNamespaceUriBuffer, &AttributeNamespaceUriConvertedLength);
                                IFC(hr);                               
                            }
                            else
                            {
                                AttributeNamespaceUriBuffer.GetBuffer()[0] = L'\0';
                            }
                            
                            IFC(RaiseAttribute(AttributeNameBuffer.GetBuffer(), AttributeNameConvertedLength, AttributeValueBuffer.GetBuffer(), AttributeValueConvertedLength, AttributePrefixBuffer.GetBuffer(), AttributePrefixConvertedLength, AttributeNamespaceUriBuffer.GetBuffer(), AttributeNamespaceUriConvertedLength, pCallback));
                        }
                    }
                    
                    if(EmptyElement)
                    {
                        IFC(RaiseElementEnd(ConversionBuffer.GetBuffer(), ConvertedTextLength, pCallback));
                    }                        

                    break;
                }

            case XML_READER_TYPE_END_ELEMENT:
                {
                    const xmlChar* pName = NULL;
                    size_t NameLength = 0;
                    UINT32 ConvertedTextLength = 0;
                    
                    pName = xmlTextReaderConstLocalName(pReader);
                    
                    NameLength = xmlStrlen(pName);
                    
                    hr = ConvertUTF8ToWCHAR< StackWCharBuffer::Type, StackWCharBuffer::StackSize >((const CHAR*)pName, NameLength, &ConversionBuffer, &ConvertedTextLength);
                    IFC(hr); 
                    
                    IFC(RaiseElementEnd(ConversionBuffer.GetBuffer(), ConvertedTextLength, pCallback));
                    
                    break;
                }

            case XML_READER_TYPE_TEXT:
                {
                    const xmlChar* pValue = NULL;
                    size_t ValueLength = 0;
                    UINT32 ConvertedTextLength = 0;
                    
                    if (xmlTextReaderHasValue(pReader))
                    {
                        pValue = xmlTextReaderConstValue(pReader);
                        
                        ValueLength = xmlStrlen(pValue);
                        
                        hr = ConvertUTF8ToWCHAR< StackWCharBuffer::Type, StackWCharBuffer::StackSize >((const CHAR*)pValue, ValueLength, &ConversionBuffer, &ConvertedTextLength);
                        IFC(hr);                         
                            
                        IFC(RaiseElementEnd(ConversionBuffer.GetBuffer(), ConvertedTextLength, pCallback));                        
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