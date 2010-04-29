#pragma once

#include "XMLReader.h"
#include "Factory.h"

#include <XmlLite.h>

typedef HRESULT (WINAPI *CreateXmlReaderFunc)( REFIID riid, void** ppvObject, IMalloc * pMalloc );

typedef HRESULT (WINAPI *SHCreateStreamOnFileWFunc)( LPCWSTR pszFile, DWORD grfMode, IStream** ppstm );

class CXMLLiteXMLElementStart : public CXMLElementStart
{
    public:
        CXMLLiteXMLElementStart( IXmlReader* pReader );

        virtual HRESULT GetName( WCHAR* pNameBuffer, UINT32 NameBufferLength );

    protected:
        IXmlReader* m_Reader;
};

class CXMLLiteXMLElementEnd : public CXMLElementEnd
{
    public:
        CXMLLiteXMLElementEnd( IXmlReader* pReader );

        virtual HRESULT GetName( WCHAR* pNameBuffer, UINT32 NameBufferLength );

    protected:
        IXmlReader* m_Reader;
};

class CXMLLiteXMLText : public CXMLText
{
    public:
        CXMLLiteXMLText( IXmlReader* pReader );

        virtual HRESULT GetText( const WCHAR** ppText, UINT32* pTextLength );

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
        HRESULT RaiseElementStart( IXmlReader* pReader, CXMLReaderCallback* pCallback );
        HRESULT RaiseElementEnd( IXmlReader* pReader, CXMLReaderCallback* pCallback );
        HRESULT RaiseText( IXmlReader* pReader, CXMLReaderCallback* pCallback );

        HMODULE m_XMLLiteModule;
        CreateXmlReaderFunc m_CreateXmlReader;

        HMODULE m_ShlwapiModule;
        SHCreateStreamOnFileWFunc m_SHCreateStreamOnFileW;
};
