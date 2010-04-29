#pragma once

#include "XMLReader.h"
#include "Factory.h"

#include <XmlLite.h>

typedef HRESULT (WINAPI *CreateXmlReaderFunc)( REFIID riid, void** ppvObject, IMalloc* pMalloc );

typedef HRESULT (WINAPI *SHCreateStreamOnFileWFunc)( LPCWSTR pszFile, DWORD grfMode, IStream** ppstm );

class CXMLLiteXMLElementStart : public CXMLElementStart
{
    public:
        CXMLLiteXMLElementStart( const WCHAR* pElementName, UINT32 NameLength );

        virtual HRESULT GetName( const WCHAR** ppNameBuffer, UINT32* pNameBufferLength );

    protected:
        const WCHAR* m_Name;
        UINT32 m_NameLength;
};

class CXMLLiteXMLElementEnd : public CXMLElementEnd
{
    public:
        CXMLLiteXMLElementEnd( const WCHAR* pElementName, UINT32 NameLength );

        virtual HRESULT GetName( const WCHAR** ppNameBuffer, UINT32* pNameBufferLength );

    protected:
        const WCHAR* m_Name;
        UINT32 m_NameLength;
};

class CXMLLiteXMLText : public CXMLText
{
    public:
        CXMLLiteXMLText( IXmlReader* pReader );

        virtual HRESULT GetText( const WCHAR** ppText, UINT32* pTextLength );

    protected:
        IXmlReader* m_Reader;
};

class CXMLLiteXMLAttribute : public CXMLAttribute
{
    public:
        CXMLLiteXMLAttribute( IXmlReader* pReader );

        virtual HRESULT GetName( const WCHAR** ppNameBuffer, UINT32* pNameBufferLength );
        virtual HRESULT GetValue( const WCHAR** ppValueBuffer, UINT32* pValueBufferLength );

    protected:
        IXmlReader* m_Reader;
};

class CXMLLiteReader : public CXMLReader
{
    public:
        DECLARE_FACTORY( CXMLLiteReader );

        virtual HRESULT LoadFromFile( const WCHAR* pPath, CXMLReaderCallback* pCallback );
        virtual HRESULT LoadFromStream( IStream* pStream, CXMLReaderCallback* pCallback );

    protected:
        CXMLLiteReader();
        virtual ~CXMLLiteReader();

        HRESULT Initialize();

        HRESULT ProcessReader( IXmlReader* pReader, CXMLReaderCallback* pCallback );
        HRESULT RaiseElementStart( const WCHAR* pName, UINT32 NameLength, CXMLReaderCallback* pCallback );
        HRESULT RaiseElementEnd( const WCHAR* pName, UINT32 NameLength, CXMLReaderCallback* pCallback );
        HRESULT RaiseText( IXmlReader* pReader, CXMLReaderCallback* pCallback );
        HRESULT RaiseAttribute( IXmlReader* pReader, CXMLReaderCallback* pCallback );

        HMODULE m_XMLLiteModule;
        CreateXmlReaderFunc m_CreateXmlReader;

        HMODULE m_ShlwapiModule;
        SHCreateStreamOnFileWFunc m_SHCreateStreamOnFileW;
};
