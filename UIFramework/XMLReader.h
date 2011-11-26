#pragma once

#include "RefCounted.h"

struct IReadStream;

class CXMLElementStart
{
    public:
        virtual __checkReturn HRESULT GetName(
			__deref_out_ecount(*pNameBufferLength) const WCHAR** ppNameBuffer, 
			__out UINT32* pNameBufferLength 
			) = 0;
};

class CXMLElementEnd
{
    public:
        virtual __checkReturn HRESULT GetName( 
			__deref_out_ecount(*pNameBufferLength) const WCHAR** ppNameBuffer, 
			__out UINT32* pNameBufferLength 
			) = 0;
};

class CXMLText
{
    public:
        virtual __checkReturn HRESULT GetText( 
			__deref_out_ecount(*pTextLength) const WCHAR** ppText, 
			__out UINT32* pTextLength 
			) = 0;
};

class CXMLAttribute
{
    public:
        virtual __checkReturn HRESULT GetPrefix( 
			__deref_out_ecount(*pPrefixBufferLength) const WCHAR** ppPrefix,
			__out UINT32* pPrefixBufferLength 
			) = 0;

        virtual __checkReturn HRESULT GetNamespaceUri(
			__deref_out_ecount(*pNamespaceUriLength) const WCHAR** ppNamespaceUri, 
			__out UINT32* pNamespaceUriLength 
			) = 0;

        virtual __checkReturn HRESULT GetName( 
			__deref_out_ecount(*pNameBufferLength) const WCHAR** ppNameBuffer, 
			__out UINT32* pNameBufferLength 
			) = 0;

        virtual __checkReturn HRESULT GetValue( 
			__deref_out_ecount(*pValueBufferLength) const WCHAR** ppValueBuffer,
			__out UINT32* pValueBufferLength 
			) = 0;
};

class CXMLReaderCallback
{
    public:
        virtual __checkReturn HRESULT OnElementStart( 
			__in CXMLElementStart* pElementStart 
			) = 0;

        virtual __checkReturn HRESULT OnElementEnd( 
			__in CXMLElementEnd* pElementEnd 
			) = 0;

        virtual __checkReturn HRESULT OnText(
			__in CXMLText* pElementEnd 
			) = 0;

        virtual __checkReturn HRESULT OnAttribute(
			__in CXMLAttribute* pAttribute
			) = 0;
};

class CXMLReader : public CRefCountedObject
{
    public:
        virtual __checkReturn HRESULT LoadFromFile(
			__in_z const WCHAR* pPath, 
			__in CXMLReaderCallback* pCallback 
			) = 0;

        virtual __checkReturn HRESULT LoadFromString( 
			__in_z const WCHAR* pText,
			__in CXMLReaderCallback* pCallback 
			) = 0;
    
        virtual __checkReturn HRESULT LoadFromStream(
            __in IReadStream* pStream,
            __in CXMLReaderCallback* pCallback
            ) = 0;
};

__checkReturn HRESULT CreateXMLReader( 
	__deref_out CXMLReader** ppReader 
	);
