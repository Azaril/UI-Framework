#include "XMLLiteReader.h"
#include <Shlwapi.h>
#include <strsafe.h>

CXMLLiteReader::CXMLLiteReader(
	) 
	: m_XMLLiteModule(NULL)
	, m_CreateXmlReader(NULL)
	, m_ShlwapiModule(NULL)
	, m_SHCreateStreamOnFileW(NULL)
	, m_SHCreateMemStream(NULL)
{
}

CXMLLiteReader::~CXMLLiteReader(
	)
{
    if(m_XMLLiteModule)
    {
        FreeLibrary(m_XMLLiteModule);
    }
}

__checkReturn HRESULT 
CXMLLiteReader::Initialize(
	)
{
    HRESULT hr = S_OK;

    m_XMLLiteModule = LoadLibrary(L"XMLLite.dll");
    IFCEXPECT(m_XMLLiteModule != NULL);

    m_CreateXmlReader = (CreateXmlReaderFunc)GetProcAddress(m_XMLLiteModule, "CreateXmlReader");
    IFCPTR(m_CreateXmlReader);

    m_ShlwapiModule = LoadLibrary(L"Shlwapi.dll");
    IFCEXPECT(m_ShlwapiModule != NULL);

    m_SHCreateStreamOnFileW = (SHCreateStreamOnFileWFunc)GetProcAddress(m_ShlwapiModule, "SHCreateStreamOnFileW");
    IFCPTR(m_SHCreateStreamOnFileW);

    m_SHCreateMemStream = (SHCreateMemStreamFunc)GetProcAddress(m_ShlwapiModule, "SHCreateMemStream");
    IFCPTR(m_SHCreateMemStream);

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CXMLLiteReader::LoadFromFile(
	__in_z const WCHAR* pPath, 
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;
    IStream* pStream = NULL;

    IFCPTR(pPath);
    IFCPTR(pCallback);

    IFC(m_SHCreateStreamOnFileW(pPath, STGM_READ, &pStream));

    IFC(LoadFromStream(pStream, pCallback));

Cleanup:
    ReleaseObject(pStream);

    return hr;
}

__checkReturn HRESULT 
CXMLLiteReader::LoadFromString(
	__in_z const WCHAR* pText, 
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;
    IStream* pStream = NULL;

    IFCPTR(pText);
    IFCPTR(pCallback);

    pStream = m_SHCreateMemStream((const BYTE*)pText, wcslen(pText) * sizeof(WCHAR));
    IFCPTR(pStream);

    IFC(LoadFromStream(pStream, pCallback));

Cleanup:
    ReleaseObject(pStream);

    return hr;
}

__checkReturn HRESULT 
CXMLLiteReader::LoadFromStream(
	__in IStream* pStream, 
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;
    IXmlReader* pReader = NULL;

    IFCPTR(pStream);
    IFCPTR(pCallback);

    IFC(m_CreateXmlReader(__uuidof(IXmlReader), (void**)&pReader, NULL));

    IFC(pReader->SetInput(pStream));

    IFC(ProcessReader(pReader, pCallback));

Cleanup:
    ReleaseObject(pReader);

    return hr;
}

__checkReturn HRESULT 
CXMLLiteReader::ProcessReader(
	__in IXmlReader* pReader,
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;
    WCHAR* pNameCopy = NULL;

    IFCPTR(pReader);
    IFCPTR(pCallback);

    XmlNodeType NodeType = XmlNodeType_None;

    do
    {
        IFC(pReader->Read(&NodeType));

        if(hr == S_FALSE)
        {
            break;
        }

        switch(NodeType)
        {
            case XmlNodeType_Element:
                {
                    const WCHAR* pName = NULL;
                    UINT32 NameLength = 0;
                    BOOL EmptyElement = FALSE;

                    EmptyElement = pReader->IsEmptyElement();

                    IFC(pReader->GetLocalName(&pName, &NameLength));

                    pNameCopy = new WCHAR[NameLength + 1];
                    IFCOOM(pNameCopy);

                    IFC(StringCchCopyN(pNameCopy, NameLength + 1, pName, NameLength));

                    IFC(RaiseElementStart(pNameCopy, NameLength, pCallback));

                    while(S_OK == (hr = pReader->MoveToNextAttribute()))
                    {
                        IFC(RaiseAttribute(pReader, pCallback));
                    }

                    if(EmptyElement)
                    {
                        IFC(RaiseElementEnd(pNameCopy, NameLength, pCallback));
                    }

                    delete [] pNameCopy;
                    pNameCopy = NULL;

                    break;
                }

            case XmlNodeType_EndElement:
                {
                    const WCHAR* pName = NULL;
                    UINT32 NameLength = 0;

                    IFC(pReader->GetLocalName(&pName, &NameLength));

                    pNameCopy = new WCHAR[NameLength + 1];
                    IFCOOM(pNameCopy);

                    IFC(StringCchCopyN(pNameCopy, NameLength + 1, pName, NameLength));

                    IFC(RaiseElementEnd(pNameCopy, NameLength, pCallback));

                    delete [] pNameCopy;
                    pNameCopy = NULL;

                    break;
                }

            case XmlNodeType_Text:
                {
                    IFC(RaiseText(pReader, pCallback));
                    
                    break;
                }
        }
    }
    while(SUCCEEDED(hr));

Cleanup:
    delete [] pNameCopy;

    return hr;
}

__checkReturn HRESULT 
CXMLLiteReader::RaiseElementStart(
	__in_ecount(NameLength) const WCHAR* pName, 
	UINT32 NameLength, 
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pName);
    IFCPTR(pCallback);
    
    {
        CXMLLiteXMLElementStart ElementStart(pName, NameLength);

        IFC(pCallback->OnElementStart(&ElementStart));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT
CXMLLiteReader::RaiseElementEnd(
	__in_ecount(NameLength) const WCHAR* pName,
	UINT32 NameLength,
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pName);
    IFCPTR(pCallback);
    
    {
        CXMLLiteXMLElementEnd ElementEnd(pName, NameLength);

        IFC(pCallback->OnElementEnd(&ElementEnd));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CXMLLiteReader::RaiseText(
	__in IXmlReader* pReader, 
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pReader);
    IFCPTR(pCallback);
    
    {
        CXMLLiteXMLText Text(pReader);

        IFC(pCallback->OnText(&Text));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CXMLLiteReader::RaiseAttribute(
	__in IXmlReader* pReader, 
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pReader);
    IFCPTR(pCallback);
    
    {
        CXMLLiteXMLAttribute Attribute(pReader);

        IFC(pCallback->OnAttribute(&Attribute));
    }

Cleanup:
    return hr;
}

CXMLLiteXMLElementStart::CXMLLiteXMLElementStart(
	__in_ecount(NameLength) const WCHAR* pElementName, 
	UINT32 NameLength
	) 
	: m_Name(pElementName)
	, m_NameLength(NameLength)
{
}

__override __checkReturn HRESULT 
CXMLLiteXMLElementStart::GetName(
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

CXMLLiteXMLElementEnd::CXMLLiteXMLElementEnd(
	__in_ecount(NameLength) const WCHAR* pElementName, 
	UINT32 NameLength
	) 
	: m_Name(pElementName)
	, m_NameLength(NameLength)
{
}

__override __checkReturn HRESULT 
CXMLLiteXMLElementEnd::GetName(
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

CXMLLiteXMLText::CXMLLiteXMLText(
	__in IXmlReader* pReader
	) 
	: m_Reader(pReader)
{
}

__override __checkReturn HRESULT 
CXMLLiteXMLText::GetText(
	__deref_out_ecount(*pTextLength) const WCHAR** ppText, 
	__out UINT32* pTextLength
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppText);
    IFCPTR(pTextLength);

    IFC(m_Reader->GetValue(ppText, pTextLength));

Cleanup:
    return hr;
}

CXMLLiteXMLAttribute::CXMLLiteXMLAttribute(
	__in IXmlReader* pReader
	) 
	: m_Reader(pReader)
{
}

__override __checkReturn HRESULT
CXMLLiteXMLAttribute::GetName(
	__deref_out_ecount(*pNameBufferLength) const WCHAR** ppNameBuffer, 
	__out UINT32* pNameBufferLength
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppNameBuffer);
    IFCPTR(pNameBufferLength);

    IFC(m_Reader->GetLocalName(ppNameBuffer, pNameBufferLength));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CXMLLiteXMLAttribute::GetValue(
	__deref_out_ecount(*pValueBufferLength) const WCHAR** ppValueBuffer, 
	__out UINT32* pValueBufferLength
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppValueBuffer);
    IFCPTR(pValueBufferLength);

    IFC(m_Reader->GetValue(ppValueBuffer, pValueBufferLength));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CXMLLiteXMLAttribute::GetPrefix(
	__deref_out_ecount(*pPrefixBufferLength) const WCHAR** ppPrefix, 
	__out UINT32* pPrefixBufferLength
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppPrefix);
    IFCPTR(pPrefixBufferLength);

    IFC(m_Reader->GetPrefix(ppPrefix, pPrefixBufferLength));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CXMLLiteXMLAttribute::GetNamespaceUri(
	__deref_out_ecount(*pNamespaceUriLength) const WCHAR** ppNamespaceUri, 
	__out UINT32* pNamespaceUriLength
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppNamespaceUri);
    IFCPTR(pNamespaceUriLength);

    IFC(m_Reader->GetNamespaceUri(ppNamespaceUri, pNamespaceUriLength));

Cleanup:
    return hr;
}