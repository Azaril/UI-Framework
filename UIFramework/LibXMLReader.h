#pragma once

#if defined(FRAMEWORK_LIBXML)

#include <libxml/xmlreader.h>

#include "XMLReader.h"
#include "Factory.h"

class CLibXMLElementStart : public CXMLElementStart
{
    public:
        CLibXMLElementStart( 
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

class CLibXMLElementEnd : public CXMLElementEnd
{
    public:
        CLibXMLElementEnd( 
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

class CLibXMLText : public CXMLText
{
    public:
        CLibXMLText( 
       		__in_ecount(TextLength) const WCHAR* pText, 
			UINT32 TextLength 
			);

        __override virtual __checkReturn HRESULT GetText( 
			__deref_out_ecount(*pTextLength) const WCHAR** ppText, 
			__out UINT32* pTextLength 
			);

    protected:
        const WCHAR* m_Text;
        UINT32 m_TextLength;
};

class CLibXMLAttribute : public CXMLAttribute
{
    public:
        CLibXMLAttribute( 
			__in_ecount(NameLength) const WCHAR* pName,
			UINT32 NameLength,
			__in_ecount(ValueLength) const WCHAR* pValue,
			UINT32 ValueLength,
			__in_ecount(PrefixLength) const WCHAR* pPrefix,
			UINT32 PrefixLength,
			__in_ecount(NamespaceUriLength) const WCHAR* pNamespaceUri,
			UINT32 NamespaceUriLength
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
    	const WCHAR* m_pName;
    	UINT32 m_NameLength;
    	const WCHAR* m_pValue;
    	UINT32 m_ValueLength;
    	const WCHAR* m_pPrefix;
    	UINT32 m_PrefixLength;
    	const WCHAR* m_pNamespaceUri;
    	UINT32 m_NamespaceUriLength;
};

class CLibXMLReader : public CXMLReader
{
    public:
        DECLARE_FACTORY( CLibXMLReader );

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
        CLibXMLReader(
			);

        virtual ~CLibXMLReader(
			);

        __checkReturn HRESULT Initialize(
			);

        __checkReturn HRESULT ProcessReader(
            __in xmlTextReaderPtr pReader, 
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
			__in_ecount(TextLength) const WCHAR* pText,
			UINT32 TextLength,
			__in CXMLReaderCallback* pCallback 
			);

        __checkReturn HRESULT RaiseAttribute( 
			__in_ecount(NameLength) const WCHAR* pName,
			UINT32 NameLength,
			__in_ecount(ValueLength) const WCHAR* pValue,
			UINT32 ValueLength,
			__in_ecount(PrefixLength) const WCHAR* pPrefix,
			UINT32 PrefixLength,
			__in_ecount(NamespaceUriLength) const WCHAR* pNamespaceUri,
			UINT32 NamespaceUriLength,
			__in CXMLReaderCallback* pCallback 
			);

        static int ReadStreamCallback(
            void* pContext, 
            char* pBuffer, 
            int bufferLength
            );
    
        static int CloseStreamCallback(
            void* pContext
            );
};

#endif