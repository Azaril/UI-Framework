#pragma once

#include "XMLReader.h"
#include "Factory.h"

#include <XmlLite.h>

typedef HRESULT (WINAPI *CreateXmlReaderFunc)(
	REFIID riid, 
	void** ppvObject, 
	IMalloc* pMalloc 
	);

typedef HRESULT (WINAPI *SHCreateStreamOnFileWFunc)( 
	LPCWSTR pszFile, 
	DWORD grfMode, 
	IStream** ppstm 
	);

typedef IStream* (WINAPI *SHCreateMemStreamFunc)( 
	const BYTE *pInit, 
	UINT cbInit 
	);

class CXMLLiteXMLElementStart : public CXMLElementStart
{
    public:
        CXMLLiteXMLElementStart( 
			__in_ecount(NameLength) const WCHAR* pElementName, 
			UINT32 NameLength 
			);

        __override virtual __checkReturn HRESULT GetName(
			__deref_out_ecount(*pNameBufferLength) const WCHAR** ppNameBuffer, 
			__out UINT32* pNameBufferLength 
			);

    protected:
        const WCHAR* m_Name;
        UINT32 m_NameLength;
};

class CXMLLiteXMLElementEnd : public CXMLElementEnd
{
    public:
        CXMLLiteXMLElementEnd( 
			__in_ecount(NameLength) const WCHAR* pElementName, 
			UINT32 NameLength 
			);

        __override virtual __checkReturn HRESULT GetName( 
			__deref_out_ecount(*pNameBufferLength) const WCHAR** ppNameBuffer, 
			__out UINT32* pNameBufferLength 
			);

    protected:
        const WCHAR* m_Name;
        UINT32 m_NameLength;
};

class CXMLLiteXMLText : public CXMLText
{
    public:
        CXMLLiteXMLText( 
			__in IXmlReader* pReader 
			);

        __override virtual __checkReturn HRESULT GetText( 
			__deref_out_ecount(*pTextLength) const WCHAR** ppText, 
			__out UINT32* pTextLength 
			);

    protected:
        IXmlReader* m_Reader;
};

class CXMLLiteXMLAttribute : public CXMLAttribute
{
    public:
        CXMLLiteXMLAttribute( 
			__in IXmlReader* pReader 
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
        IXmlReader* m_Reader;
};

class CXMLLiteReader : public CXMLReader
{
    public:
        DECLARE_FACTORY( CXMLLiteReader );

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

        virtual __checkReturn HRESULT LoadFromStream(
			__in IStream* pStream,
			__in CXMLReaderCallback* pCallback 
			);

    protected:
        CXMLLiteReader(
			);

        virtual ~CXMLLiteReader(
			);

        __checkReturn HRESULT Initialize(
			);

        __checkReturn HRESULT ProcessReader(
			__in IXmlReader* pReader, 
			__in CXMLReaderCallback* pCallback 
			);

        __checkReturn HRESULT RaiseElementStart( 
			__in_ecount(NameLength) const WCHAR* pName, 
			UINT32 NameLength, 
			__in CXMLReaderCallback* pCallback 
			);

        __checkReturn HRESULT RaiseElementEnd( 
			__in_ecount(NameLength) const WCHAR* pName,
			UINT32 NameLength,
			__in CXMLReaderCallback* pCallback 
			);

        __checkReturn HRESULT RaiseText( 
			__in IXmlReader* pReader,
			__in CXMLReaderCallback* pCallback 
			);

        __checkReturn HRESULT RaiseAttribute( 
			__in IXmlReader* pReader, 
			__in CXMLReaderCallback* pCallback 
			);

        HMODULE m_XMLLiteModule;
        CreateXmlReaderFunc m_CreateXmlReader;

        HMODULE m_ShlwapiModule;
        SHCreateStreamOnFileWFunc m_SHCreateStreamOnFileW;
        SHCreateMemStreamFunc m_SHCreateMemStream;
};
