#include "XMLLiteReader.h"
#include <Shlwapi.h>
#include <strsafe.h>

CXMLLiteReader::CXMLLiteReader() : m_XMLLiteModule(NULL),
                                   m_CreateXmlReader(NULL),
                                   m_ShlwapiModule(NULL),
                                   m_SHCreateStreamOnFileW(NULL)
{
}

CXMLLiteReader::~CXMLLiteReader()
{
    if(m_XMLLiteModule)
    {
        FreeLibrary(m_XMLLiteModule);
    }
}

HRESULT CXMLLiteReader::Initialize()
{
    HRESULT hr = S_OK;
    CreateXmlReaderFunc CreateXmlReader = NULL;

    m_XMLLiteModule = LoadLibrary(L"XMLLite.dll");
    IFCEXPECT(m_XMLLiteModule != NULL);

    m_CreateXmlReader = (CreateXmlReaderFunc)GetProcAddress(m_XMLLiteModule, "CreateXmlReader");
    IFCPTR(m_CreateXmlReader);

    m_ShlwapiModule = LoadLibrary(L"Shlwapi.dll");
    IFCEXPECT(m_ShlwapiModule != NULL);

    m_SHCreateStreamOnFileW = (SHCreateStreamOnFileWFunc)GetProcAddress(m_ShlwapiModule, "SHCreateStreamOnFileW");
    IFCPTR(m_SHCreateStreamOnFileW);

Cleanup:
    return hr;
}

HRESULT CXMLLiteReader::LoadFromFile(const WCHAR* pPath, CXMLReaderCallback* pCallback)
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

HRESULT CXMLLiteReader::LoadFromStream(IStream* pStream, CXMLReaderCallback* pCallback)
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

HRESULT CXMLLiteReader::ProcessReader(IXmlReader* pReader, CXMLReaderCallback* pCallback)
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

                    delete pNameCopy;
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

                    delete pNameCopy;
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
    delete pNameCopy;

    return hr;
}

HRESULT CXMLLiteReader::RaiseElementStart(const WCHAR* pName, UINT32 NameLength, CXMLReaderCallback* pCallback)
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

HRESULT CXMLLiteReader::RaiseElementEnd(const WCHAR* pName, UINT32 NameLength, CXMLReaderCallback* pCallback)
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

HRESULT CXMLLiteReader::RaiseText(IXmlReader* pReader, CXMLReaderCallback* pCallback)
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

HRESULT CXMLLiteReader::RaiseAttribute(IXmlReader* pReader, CXMLReaderCallback* pCallback)
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

CXMLLiteXMLElementStart::CXMLLiteXMLElementStart(const WCHAR* pElementName, UINT32 NameLength) : m_Name(pElementName),
                                                                                                 m_NameLength(NameLength)
{
}

HRESULT CXMLLiteXMLElementStart::GetName(const WCHAR** ppNameBuffer, UINT32* pNameBufferLength)
{
    HRESULT hr = S_OK;
    const WCHAR* pInternalBuffer = NULL;
    UINT32 InternalBufferLength = 0;

    IFCPTR(ppNameBuffer);
    IFCPTR(pNameBufferLength);

    *ppNameBuffer = m_Name;
    *pNameBufferLength = m_NameLength;

Cleanup:
    return hr;
}

CXMLLiteXMLElementEnd::CXMLLiteXMLElementEnd(const WCHAR* pElementName, UINT32 NameLength) : m_Name(pElementName),
                                                                                             m_NameLength(NameLength)
{
}

HRESULT CXMLLiteXMLElementEnd::GetName(const WCHAR** ppNameBuffer, UINT32* pNameBufferLength)
{
    HRESULT hr = S_OK;
    const WCHAR* pInternalBuffer = NULL;
    UINT32 InternalBufferLength = 0;

    IFCPTR(ppNameBuffer);
    IFCPTR(pNameBufferLength);

    *ppNameBuffer = m_Name;
    *pNameBufferLength = m_NameLength;

Cleanup:
    return hr;
}

CXMLLiteXMLText::CXMLLiteXMLText(IXmlReader* pReader) : m_Reader(pReader)
{
}

HRESULT CXMLLiteXMLText::GetText(const WCHAR** ppText, UINT32* pTextLength)
{
    HRESULT hr = S_OK;

    IFCPTR(ppText);
    IFCPTR(pTextLength);

    IFC(m_Reader->GetValue(ppText, pTextLength));

Cleanup:
    return hr;
}

CXMLLiteXMLAttribute::CXMLLiteXMLAttribute(IXmlReader* pReader) : m_Reader(pReader)
{
}

HRESULT CXMLLiteXMLAttribute::GetName(const WCHAR** ppNameBuffer, UINT32* pNameBufferLength)
{
    HRESULT hr = S_OK;

    IFCPTR(ppNameBuffer);
    IFCPTR(pNameBufferLength);

    IFC(m_Reader->GetLocalName(ppNameBuffer, pNameBufferLength));

Cleanup:
    return hr;
}

HRESULT CXMLLiteXMLAttribute::GetValue(const WCHAR** ppValueBuffer, UINT32* pValueBufferLength)
{
    HRESULT hr = S_OK;

    IFCPTR(ppValueBuffer);
    IFCPTR(pValueBufferLength);

    IFC(m_Reader->GetValue(ppValueBuffer, pValueBufferLength));

Cleanup:
    return hr;
}

HRESULT CXMLLiteXMLAttribute::GetPrefix(const WCHAR** ppPrefix, UINT32* pPrefixBufferLength)
{
    HRESULT hr = S_OK;

    IFCPTR(ppPrefix);
    IFCPTR(pPrefixBufferLength);

    IFC(m_Reader->GetPrefix(ppPrefix, pPrefixBufferLength));

Cleanup:
    return hr;
}

HRESULT CXMLLiteXMLAttribute::GetNamespaceUri(const WCHAR** ppNamespaceUri, UINT32* pNamespaceUriLength)
{
    HRESULT hr = S_OK;

    IFCPTR(ppNamespaceUri);
    IFCPTR(pNamespaceUriLength);

    IFC(m_Reader->GetNamespaceUri(ppNamespaceUri, pNamespaceUriLength));

Cleanup:
    return hr;
}