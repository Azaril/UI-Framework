#pragma once

#include "RefCounted.h"

class CXMLElementStart
{
    public:
        virtual HRESULT GetName( WCHAR* pNameBuffer, UINT32 NameBufferLength ) = 0;
};

class CXMLElementEnd
{
    public:
        virtual HRESULT GetName( WCHAR* pNameBuffer, UINT32 NameBufferLength ) = 0;
};

class CXMLText
{
    public:
        virtual HRESULT GetText( const WCHAR** ppText, UINT32* pTextLength ) = 0;
};

class CXMLReaderCallback
{
    public:
        virtual HRESULT OnElementStart( CXMLElementStart* pElementStart ) = 0;
        virtual HRESULT OnElementEnd( CXMLElementEnd* pElementEnd ) = 0;
        virtual HRESULT OnText( CXMLText* pElementEnd ) = 0;
};

class CXMLReader : public CRefCountedObject
{
    public:
        virtual HRESULT LoadFromFile( const WCHAR* pPath, CXMLReaderCallback* pCallback ) = 0;
};

HRESULT CreateXMLReader( CXMLReader** ppReader );
