#pragma once

#if defined(FRAMEWORK_TINYXML2)

#include "XMLReader.h"
#include "Factory.h"
#include "StackHeapBuffer.h"
#include "Noncopyable.h"

#include <TinyXML2.h>

struct TinyXML2NamespaceData
{
	std::wstring name;
	std::wstring uri;
};

struct TinyXML2ElementContext
{
	vector< TinyXML2NamespaceData > namespaces;
};

class CTinyXML2XMLElementStart : public CXMLElementStart
{
	public:
		CTinyXML2XMLElementStart( 
			const tinyxml2::XMLElement& element
			);

		__override virtual __checkReturn HRESULT GetName(
			__deref_out_ecount(*pNameBufferLength) const WCHAR** ppNameBuffer, 
			__out UINT32* pNameBufferLength 
			);

	protected:
		DISABLE_COPY_AND_ASSIGN( CTinyXML2XMLElementStart )

		const tinyxml2::XMLElement& m_element;
		bool m_haveName;
		UINT32 m_nameLength;
		StackHeapBuffer< WCHAR, 32 > m_nameBuffer;
};

class CTinyXML2XMLElementEnd : public CXMLElementEnd
{
	public:
		CTinyXML2XMLElementEnd( 
			const tinyxml2::XMLElement& element
			);

		__override virtual __checkReturn HRESULT GetName(
			__deref_out_ecount(*pNameBufferLength) const WCHAR** ppNameBuffer, 
			__out UINT32* pNameBufferLength 
			);

	protected:
		DISABLE_COPY_AND_ASSIGN( CTinyXML2XMLElementEnd )

		const tinyxml2::XMLElement& m_element;
		bool m_haveName;
		UINT32 m_nameLength;
		StackHeapBuffer< WCHAR, 32 > m_nameBuffer;
};

class CTinyXML2XMLText : public CXMLText
{
	public:
		CTinyXML2XMLText( 
			const tinyxml2::XMLText& text
			);

		__override virtual __checkReturn HRESULT GetText( 
			__deref_out_ecount(*pTextLength) const WCHAR** ppText, 
			__out UINT32* pTextLength 
			);

	protected:
		DISABLE_COPY_AND_ASSIGN( CTinyXML2XMLText )

		const tinyxml2::XMLText& m_text;
		bool m_haveText;
		UINT32 m_textLength;
		StackHeapBuffer< WCHAR, 256 > m_textBuffer;
};

class CTinyXML2XMLAttribute : public CXMLAttribute
{
	public:
		CTinyXML2XMLAttribute( 
			const tinyxml2::XMLAttribute& attribute,
			const vector< TinyXML2ElementContext >& contexts
			);

		__override virtual __checkReturn HRESULT GetPrefix( 
			__deref_out_ecount(*pPrefixBufferLength) const WCHAR** ppPrefix,
			__out UINT32* pPrefixBufferLength 
			);

		__override virtual __checkReturn HRESULT GetNamespaceUri(
			__deref_out_ecount(*pNamespaceUriLength) const WCHAR** ppNamespaceUri, 
			__out UINT32* pNamespaceUriLength 
			);

		__override virtual __checkReturn HRESULT GetName( 
			__deref_out_ecount(*pNameBufferLength) const WCHAR** ppNameBuffer, 
			__out UINT32* pNameBufferLength 
			);

		__override virtual __checkReturn HRESULT GetValue( 
			__deref_out_ecount(*pValueBufferLength) const WCHAR** ppValueBuffer,
			__out UINT32* pValueBufferLength 
			);

	protected:
		DISABLE_COPY_AND_ASSIGN( CTinyXML2XMLAttribute )

		__checkReturn HRESULT EnsureParsedName(
			);

		const tinyxml2::XMLAttribute& m_attribute;
		const vector< TinyXML2ElementContext >& m_contexts;

		bool m_parsedName;
		UINT32 m_nameLength;
		StackHeapBuffer< WCHAR, 32 > m_nameBuffer;
		UINT32 m_prefixLength;
		StackHeapBuffer< WCHAR, 128 > m_prefixBuffer;
		bool m_haveValue;
		UINT32 m_valueLength;
		StackHeapBuffer< WCHAR, 128 > m_valueBuffer;
};

class CTinyXML2Visitor : public tinyxml2::XMLVisitor
{
	public:
		CTinyXML2Visitor(
			__in CXMLReaderCallback* pCallback
			);

		virtual bool VisitEnter( 
			const tinyxml2::XMLDocument& document
			);

		virtual bool VisitExit(
			const tinyxml2::XMLDocument& document 
			);

		virtual bool VisitEnter(
			const tinyxml2::XMLElement& element,
			const tinyxml2::XMLAttribute* pFirstAttribute
			);

		virtual bool VisitExit(
			const tinyxml2::XMLElement& element 
			);

		virtual bool Visit( 
			const tinyxml2::XMLDeclaration& declaration
			);

		virtual bool Visit(
			const tinyxml2::XMLText& text 
			);

		virtual bool Visit( 
			const tinyxml2::XMLComment& comment
			);

		virtual bool Visit( 
			const tinyxml2::XMLUnknown& unknown
			);

	protected:
		HRESULT m_hr;
		CXMLReaderCallback* m_pCallback;
		vector< TinyXML2ElementContext > m_contexts;
};

class CTinyXML2Reader : public CXMLReader
{
public:
	DECLARE_FACTORY( CTinyXML2Reader );

	__override virtual __checkReturn HRESULT LoadFromFile(
		__in_z const WCHAR* pPath, 
		__in CXMLReaderCallback* pCallback 
		);

	__override virtual __checkReturn HRESULT LoadFromString( 
		__in_z const WCHAR* pText,
		__in CXMLReaderCallback* pCallback 
		);

	__override virtual __checkReturn HRESULT LoadFromStream(
		__in IReadStream* pStream,
		__in CXMLReaderCallback* pCallback
		);

protected:
	CTinyXML2Reader(
		);

	virtual ~CTinyXML2Reader(
		);

	__checkReturn HRESULT Initialize(
		);
};

#endif