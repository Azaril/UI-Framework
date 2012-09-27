#include "TinyXML2Reader.h"
#include "StringConversion.h"
#include "FileResourceStream.h"

#if defined(FRAMEWORK_TINYXML2)

CTinyXML2Reader::CTinyXML2Reader(
	)
{
}

CTinyXML2Reader::~CTinyXML2Reader(
	)
{
}

__checkReturn HRESULT 
CTinyXML2Reader::Initialize(
	)
{
    HRESULT hr = S_OK;

    return hr;
}

__checkReturn HRESULT 
CTinyXML2Reader::LoadFromFile(
	__in_z const WCHAR* pPath, 
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;
    CFileResourceStream* pStream = NULL;

    IFCPTR(pPath);
    IFCPTR(pCallback);

	IFC(CFileResourceStream::CreateOnPath(pPath, &pStream));

	IFC(LoadFromStream(pStream, pCallback));

Cleanup:
	ReleaseObject(pStream);

    return hr;
}

__checkReturn HRESULT 
CTinyXML2Reader::LoadFromString(
	__in_z const WCHAR* pText, 
	__in CXMLReaderCallback* pCallback
	)
{
    HRESULT hr = S_OK;
	StackHeapBuffer< CHAR, 2048 > buffer;
	UINT32 bufferLength = 0;

    IFCPTR(pText);
    IFCPTR(pCallback);

	IFC(ConvertWCHARToUTF8(pText, &buffer, &bufferLength));

	{
		tinyxml2::XMLDocument document;

		IFCEXPECT(document.Parse(buffer.GetBuffer()) == tinyxml2::XML_NO_ERROR);

		{
			CTinyXML2Visitor visitor(pCallback);

			document.Accept(&visitor);
		}
	}

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CTinyXML2Reader::LoadFromStream(
    __in IReadStream* pStream,
    __in CXMLReaderCallback* pCallback
    )
{
	HRESULT hr = S_OK;
	UINT64 dataSize = 0;
	StackHeapBuffer< WCHAR, 2048 > streamBuffer;
	StackHeapBuffer< WCHAR, 2048 > readBuffer;
	UINT64 bytesRead = 0;
	UINT64 characterLength = 0;
     
	IFC(pStream->GetSize(&dataSize));

	characterLength = dataSize / sizeof(WCHAR);

	IFC(streamBuffer.EnsureBufferSize((size_t)(characterLength + 1)));

	IFC(pStream->Seek(SeekType::Begin, 0, NULL));

	IFC(pStream->Read(streamBuffer.GetBuffer(), characterLength * sizeof(WCHAR), &bytesRead));

	IFCEXPECT(bytesRead >= sizeof(WCHAR));

	const WCHAR* pData = streamBuffer.GetBuffer();

	switch (pData[0])
	{
		case 0xFEFF:
			{
				++pData;

				streamBuffer.GetBuffer()[characterLength] = L'\0';

                break;
			}

		case 0xFFFE:
			{
#if defined(PLATFORM_LITTLE_ENDIAN)
				IFC(ConvertUTF16BEToWCHAR((const UINT16*)streamBuffer.GetBuffer(), (UINT32)characterLength, &readBuffer, NULL));
#elif defined(PLATFORM_BIG_ENDIAN)
				IFC(ConvertUTF16LEToWCHAR((const UINT16*)streamBuffer.GetBuffer(), (UINT32)characterLength, &readBuffer, NULL));
#else
#error Endianness not defined!
#endif
				pData = readBuffer.GetBuffer();

                break;
			}
	}

	IFC(LoadFromString(pData, pCallback));

Cleanup:
     return hr;
}

const WCHAR namespaceIdentifier[] = L"xmlns";
const UINT32 namespaceIdentifierLength = ARRAYSIZE(namespaceIdentifier) - 1;

const WCHAR namespaceUri[] = L"http://www.w3.org/2000/xmlns/";
const UINT32 namespaceUriLength = ARRAYSIZE(namespaceUri) - 1;

CTinyXML2Visitor::CTinyXML2Visitor(
	__in CXMLReaderCallback* pCallback 
	)
	: m_pCallback(pCallback)
{
	m_contexts.push_back(TinyXML2ElementContext());

	TinyXML2ElementContext& context = m_contexts.back();

	TinyXML2NamespaceData namespaceData;

	namespaceData.name = namespaceIdentifier;
	namespaceData.uri = namespaceUri;

	context.namespaces.push_back(namespaceData);
}

bool 
CTinyXML2Visitor::VisitEnter( 
	const tinyxml2::XMLDocument& document
	)
{
	HRESULT hr = S_OK;

	return (SUCCEEDED(hr));
}

bool
CTinyXML2Visitor::VisitExit(
	const tinyxml2::XMLDocument& document 
	)
{
	HRESULT hr = S_OK;

	return (SUCCEEDED(hr));
}

bool 
CTinyXML2Visitor::VisitEnter(
	const tinyxml2::XMLElement& element,
	const tinyxml2::XMLAttribute* pFirstAttribute
	)
{
	HRESULT hr = S_OK;

	m_contexts.push_back(TinyXML2ElementContext());

	TinyXML2ElementContext& context = m_contexts.back();

	{
		CTinyXML2XMLElementStart start(element);

		IFC(m_pCallback->OnElementStart(&start));
	}

	for (const tinyxml2::XMLAttribute* pCurrentAttribute = pFirstAttribute; pCurrentAttribute != NULL; pCurrentAttribute = pCurrentAttribute->Next())
	{
		CTinyXML2XMLAttribute attribute(*pCurrentAttribute, m_contexts);

		//
		// TODO: wiarchbe: Fix this, giant hack for not supporting namespaces in tinyxml2.
		//
		const WCHAR* pNamespaceUri = NULL;
		UINT32 namespaceLength = 0;

		IFC(attribute.GetNamespaceUri(&pNamespaceUri, &namespaceLength));

		if (wcscmp(pNamespaceUri, namespaceUri) == 0)
		{
			const WCHAR* pAttributeName = NULL;
			UINT32 nameLength = 0;

			const WCHAR* pAttributeValue = NULL;
			UINT32 valueLength = 0;

			IFC(attribute.GetName(&pAttributeName, &nameLength));

			IFC(attribute.GetValue(&pAttributeValue, &valueLength));

			TinyXML2NamespaceData namespaceData;

			namespaceData.name = std::wstring(pAttributeName, nameLength);
			namespaceData.uri = std::wstring(pAttributeValue, valueLength);

			context.namespaces.push_back(namespaceData);
		}
		else
		{
			IFC(m_pCallback->OnAttribute(&attribute));
		}
	}

Cleanup:
	if (FAILED(hr) && SUCCEEDED(m_hr))
	{
		m_hr = hr;
	}

	return (SUCCEEDED(hr));
}

bool 
CTinyXML2Visitor::VisitExit(
	const tinyxml2::XMLElement& element 
	)
{
	HRESULT hr = S_OK;

	{
		CTinyXML2XMLElementEnd end(element);

		IFC(m_pCallback->OnElementEnd(&end));
	}

	m_contexts.pop_back();

Cleanup:
	if (FAILED(hr) && SUCCEEDED(m_hr))
	{
		m_hr = hr;
	}

	return (SUCCEEDED(hr));
}

bool 
CTinyXML2Visitor::Visit( 
	const tinyxml2::XMLDeclaration& declaration
	)
{
	HRESULT hr = S_OK;

	return (SUCCEEDED(hr));
}

bool 
CTinyXML2Visitor::Visit(
	const tinyxml2::XMLText& text 
	)
{
	HRESULT hr = S_OK;

	{
		CTinyXML2XMLText elementText(text);

		IFC(m_pCallback->OnText(&elementText));
	}

Cleanup:
	if (FAILED(hr) && SUCCEEDED(m_hr))
	{
		m_hr = hr;
	}

	return (SUCCEEDED(hr));
}

bool 
CTinyXML2Visitor::Visit( 
	const tinyxml2::XMLComment& comment
	)
{
	HRESULT hr = S_OK;

	return (SUCCEEDED(hr));
}

bool 
CTinyXML2Visitor::Visit( 
	const tinyxml2::XMLUnknown& unknown
	)
{
	HRESULT hr = S_OK;

	return (SUCCEEDED(hr));
}

CTinyXML2XMLElementStart::CTinyXML2XMLElementStart(
	const tinyxml2::XMLElement& element
	) 
	: m_element(element)
	, m_haveName(false)
	, m_nameLength(0)
{
}

__override __checkReturn HRESULT 
CTinyXML2XMLElementStart::GetName(
	__deref_out_ecount(*pNameBufferLength) const WCHAR** ppNameBuffer, 
	__out UINT32* pNameBufferLength
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppNameBuffer);
    IFCPTR(pNameBufferLength);

	if (!m_haveName)
	{
		IFC(ConvertUTF8ToWCHAR(m_element.Name(), &m_nameBuffer, &m_nameLength));

		m_haveName = true;
	}

    *ppNameBuffer = m_nameBuffer.GetBuffer();
    *pNameBufferLength = m_nameLength;

Cleanup:
    return hr;
}

CTinyXML2XMLElementEnd::CTinyXML2XMLElementEnd(
	const tinyxml2::XMLElement& element
	) 
	: m_element(element)
	, m_haveName(false)
	, m_nameLength(0)
{
}

__override __checkReturn HRESULT 
CTinyXML2XMLElementEnd::GetName(
	__deref_out_ecount(*pNameBufferLength) const WCHAR** ppNameBuffer,
	__out UINT32* pNameBufferLength
	)
{
	HRESULT hr = S_OK;

	IFCPTR(ppNameBuffer);
	IFCPTR(pNameBufferLength);

	if (!m_haveName)
	{
		IFC(ConvertUTF8ToWCHAR(m_element.Name(), &m_nameBuffer, &m_nameLength));

		m_haveName = true;
	}

	*ppNameBuffer = m_nameBuffer.GetBuffer();
	*pNameBufferLength = m_nameLength;

Cleanup:
	return hr;
}

CTinyXML2XMLText::CTinyXML2XMLText(
	const tinyxml2::XMLText& text
	) 
	: m_text(text)
	, m_haveText(false)
	, m_textLength(0)
{
}

__override __checkReturn HRESULT 
CTinyXML2XMLText::GetText(
	__deref_out_ecount(*pTextLength) const WCHAR** ppText, 
	__out UINT32* pTextLength
	)
{
	HRESULT hr = S_OK;

	IFCPTR(ppText);
	IFCPTR(pTextLength);

	if (!m_haveText)
	{
		IFC(ConvertUTF8ToWCHAR(m_text.Value(), &m_textBuffer, &m_textLength));

		m_haveText = true;
	}

	*ppText = m_textBuffer.GetBuffer();
	*pTextLength = m_textLength;

Cleanup:
	return hr;
}

CTinyXML2XMLAttribute::CTinyXML2XMLAttribute(
	const tinyxml2::XMLAttribute& attribute,
	const vector< TinyXML2ElementContext >& contexts
	) 
	: m_attribute(attribute)
	, m_contexts(contexts)
	, m_parsedName(false)
	, m_nameLength(0)
	, m_prefixLength(0)
	, m_haveValue(false)
	, m_valueLength(0)
{
}

__checkReturn HRESULT
CTinyXML2XMLAttribute::EnsureParsedName(
	)
{
	HRESULT hr = S_OK;

	if (!m_parsedName)
	{
		const CHAR* pAttributeName = m_attribute.Name();
		UINT32 totalLength = strlen(pAttributeName);

		const CHAR* pName = NULL;
		const CHAR* pPrefixSeparator = NULL;

		const CHAR* pSearch = pAttributeName;	

		while (*pSearch != '\0')
		{
			if (*pSearch == ':')
			{
				pPrefixSeparator = pSearch;

				break;
			}

			++pSearch;
		}

		if (pPrefixSeparator == NULL)
		{
			pName = pAttributeName;
		}
		else
		{
			pName = pPrefixSeparator + 1;
		}

		UINT32 nameLength = (pAttributeName + totalLength) - pName;
		UINT32 prefixLength = (pPrefixSeparator != NULL) ? (pPrefixSeparator - pAttributeName) : 0;

		IFCEXPECT(nameLength > 0);

		IFC(ConvertUTF8ToWCHAR(pName, nameLength, &m_nameBuffer, &m_nameLength));

		if (prefixLength > 0)
		{
			IFC(ConvertUTF8ToWCHAR(pAttributeName, prefixLength, &m_prefixBuffer, &m_prefixLength));
		}
		else
		{
			m_prefixLength = 0;
		}

		m_parsedName = true;
	}

Cleanup:
	return hr;
}

__override __checkReturn HRESULT
CTinyXML2XMLAttribute::GetName(
	__deref_out_ecount(*pNameBufferLength) const WCHAR** ppNameBuffer, 
	__out UINT32* pNameBufferLength
	)
{
	HRESULT hr = S_OK;

	IFCPTR(ppNameBuffer);
	IFCPTR(pNameBufferLength);

	IFC(EnsureParsedName());

	*ppNameBuffer = m_nameBuffer.GetBuffer();
	*pNameBufferLength = m_nameLength;

Cleanup:
	return hr;
}

__override __checkReturn HRESULT 
CTinyXML2XMLAttribute::GetValue(
	__deref_out_ecount(*pValueBufferLength) const WCHAR** ppValueBuffer, 
	__out UINT32* pValueBufferLength
	)
{
	HRESULT hr = S_OK;

	IFCPTR(ppValueBuffer);
	IFCPTR(pValueBufferLength);

	if (!m_haveValue)
	{
		IFC(ConvertUTF8ToWCHAR(m_attribute.Value(), &m_valueBuffer, &m_valueLength));

		m_haveValue = true;
	}

	*ppValueBuffer = m_valueBuffer.GetBuffer();
	*pValueBufferLength = m_valueLength;

Cleanup:
	return hr;
}

__override __checkReturn HRESULT 
CTinyXML2XMLAttribute::GetPrefix(
	__deref_out_ecount(*pPrefixBufferLength) const WCHAR** ppPrefix, 
	__out UINT32* pPrefixBufferLength
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppPrefix);
    IFCPTR(pPrefixBufferLength);

	IFC(EnsureParsedName());

	*ppPrefix = m_prefixBuffer.GetBuffer();
	*pPrefixBufferLength = m_prefixLength;

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CTinyXML2XMLAttribute::GetNamespaceUri(
	__deref_out_ecount(*pNamespaceUriLength) const WCHAR** ppNamespaceUri, 
	__out UINT32* pNamespaceUriLength
	)
{
    HRESULT hr = S_OK;

	const WCHAR* pPrefix = NULL;
	UINT32 prefixLength = 0;

    IFCPTR(ppNamespaceUri);
    IFCPTR(pNamespaceUriLength);

	IFC(GetPrefix(&pPrefix, &prefixLength));

	if (prefixLength > 0)
	{
		bool found = false;

		for (vector< TinyXML2ElementContext >::const_reverse_iterator contextIt = m_contexts.rbegin(); contextIt != m_contexts.rend() && !found; ++contextIt)
		{
			const vector< TinyXML2NamespaceData >& namespaces = contextIt->namespaces;

			for (vector< TinyXML2NamespaceData >::const_iterator namespaceIt = namespaces.begin(); namespaceIt != namespaces.end() && !found; ++namespaceIt)
			{
				if (wcscmp(namespaceIt->name.c_str(), pPrefix) == 0)
				{
					*ppNamespaceUri = namespaceIt->uri.c_str();
					*pNamespaceUriLength = namespaceIt->uri.length();

					found = true;
				}
			}
		}

		IFCEXPECT(found);
	}
	else
	{
		*ppNamespaceUri = L"";
		*pNamespaceUriLength = 0;
	}

Cleanup:
    return hr;
}

#endif