#pragma once

#include "RefCounted.h"

class CXMLElementStart
{
    public:
        virtual HRESULT GetName( const WCHAR** ppNameBuffer, UINT32* pNameBufferLength ) = 0;
};

class CXMLElementEnd
{
    public:
        virtual HRESULT GetName( const WCHAR** ppNameBuffer, UINT32* pNameBufferLength ) = 0;
};

class CXMLText
{
    public:
        virtual HRESULT GetText( const WCHAR** ppText, UINT32* pTextLength ) = 0;
};

class CXMLAttribute
{
    public:
        virtual HRESULT GetPrefix( const WCHAR** ppPrefix, UINT32* pPrefixBufferLength ) = 0;
        virtual HRESULT GetNamespaceUri( const WCHAR** ppNamespaceUri, UINT32* pNamespaceUriLength ) = 0;
        virtual HRESULT GetName( const WCHAR** ppNameBuffer, UINT32* pNameBufferLength ) = 0;
        virtual HRESULT GetValue( const WCHAR** ppValueBuffer, UINT32* pValueBufferLength ) = 0;
};

class CXMLReaderCallback
{
    public:
        virtual HRESULT OnElementStart( CXMLElementStart* pElementStart ) = 0;
        virtual HRESULT OnElementEnd( CXMLElementEnd* pElementEnd ) = 0;
        virtual HRESULT OnText( CXMLText* pElementEnd ) = 0;
        virtual HRESULT OnAttribute( CXMLAttribute* pAttribute ) = 0;
};

class CXMLReader : public CRefCountedObject
{
    public:
        virtual HRESULT LoadFromFile( const WCHAR* pPath, CXMLReaderCallback* pCallback ) = 0;
};

HRESULT CreateXMLReader( CXMLReader** ppReader );
