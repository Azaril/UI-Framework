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

    IFCPTR(pReader);
    IFCPTR(pCallback);

    XmlNodeType NodeType = XmlNodeType_None;

    while(S_OK == (hr = pReader->Read(&NodeType)))
    {
        switch(NodeType)
        {
            case XmlNodeType_Element:
                {
                    IFC(RaiseElementStart(pReader, pCallback));

                    if(pReader->IsEmptyElement())
                    {
                        IFC(RaiseElementEnd(pReader, pCallback));
                    }

                    break;
                }

            case XmlNodeType_EndElement:
                {
                    IFC(RaiseElementEnd(pReader, pCallback));

                    break;
                }

            case XmlNodeType_Text:
                {
                    IFC(RaiseText(pReader, pCallback));
                    
                    break;
                }
        }
    }

Cleanup:
    return hr;
}

HRESULT CXMLLiteReader::RaiseElementStart(IXmlReader* pReader, CXMLReaderCallback* pCallback)
{
    HRESULT hr = S_OK;

    IFCPTR(pReader);
    IFCPTR(pCallback);
    
    {
        CXMLLiteXMLElementStart ElementStart(pReader);

        IFC(pCallback->OnElementStart(&ElementStart));
    }

Cleanup:
    return hr;
}

HRESULT CXMLLiteReader::RaiseElementEnd(IXmlReader* pReader, CXMLReaderCallback* pCallback)
{
    HRESULT hr = S_OK;

    IFCPTR(pReader);
    IFCPTR(pCallback);
    
    {
        CXMLLiteXMLElementEnd ElementEnd(pReader);

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

CXMLLiteXMLElementStart::CXMLLiteXMLElementStart(IXmlReader* pReader) : m_Reader(pReader)
{
}

HRESULT CXMLLiteXMLElementStart::GetName(WCHAR* pNameBuffer, UINT32 NameBufferLength)
{
    HRESULT hr = S_OK;
    const WCHAR* pInternalBuffer = NULL;
    UINT32 InternalBufferLength = 0;

    IFCPTR(pNameBuffer);

    IFC(m_Reader->GetLocalName(&pInternalBuffer, &InternalBufferLength));

    IFC(StringCchCopyN(pNameBuffer, NameBufferLength, pInternalBuffer, InternalBufferLength));

Cleanup:
    return hr;
}

CXMLLiteXMLElementEnd::CXMLLiteXMLElementEnd(IXmlReader* pReader) : m_Reader(pReader)
{
}

HRESULT CXMLLiteXMLElementEnd::GetName(WCHAR* pNameBuffer, UINT32 NameBufferLength)
{
    HRESULT hr = S_OK;
    const WCHAR* pInternalBuffer = NULL;
    UINT32 InternalBufferLength = 0;

    IFCPTR(pNameBuffer);

    IFC(m_Reader->GetLocalName(&pInternalBuffer, &InternalBufferLength));

    IFC(StringCchCopyN(pNameBuffer, NameBufferLength, pInternalBuffer, InternalBufferLength));

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