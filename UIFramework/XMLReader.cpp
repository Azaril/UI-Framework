#include "XMLReader.h"
#include "ErrorChecking.h"

#if defined(FRAMEWORK_XMLLITE)
#include "XMLLiteReader.h"
#elif defined(FRAMEWORK_LIBXML)
#include "LibXMLReader.h"
#elif defined(FRAMEWORK_TINYXML2)
#include "TinyXml2Reader.h"
#endif

__checkReturn HRESULT 
CreateXMLReader(
	__deref_out CXMLReader** ppReader
	)
{
    HRESULT hr = S_OK;
    
#if defined(FRAMEWORK_XMLLITE)
    CXMLLiteReader* pXMLLiteReader = NULL;
#elif defined(FRAMEWORK_LIBXML)
    CLibXMLReader* pLibXMLReader = NULL;
#elif defined(FRAMEWORK_TINYXML2)
	CTinyXML2Reader* pTinyXML2Reader = NULL;
#endif

    IFCPTR(ppReader);

#if defined(FRAMEWORK_XMLLITE)
    if(SUCCEEDED(CXMLLiteReader::Create(&pXMLLiteReader)))
    {
        *ppReader = pXMLLiteReader;
        pXMLLiteReader = NULL;
        goto Cleanup;
    }
#elif defined(FRAMEWORK_LIBXML)
    if(SUCCEEDED(CLibXMLReader::Create(&pLibXMLReader)))
    {
        *ppReader = pLibXMLReader;
        pLibXMLReader = NULL;
        goto Cleanup;
    }
#elif defined(FRAMEWORK_TINYXML2)
	if(SUCCEEDED(CTinyXML2Reader::Create(&pTinyXML2Reader)))
	{
		*ppReader = pTinyXML2Reader;
		pTinyXML2Reader = NULL;
		goto Cleanup;
	}
#endif
    
    IFC(E_FAIL);

Cleanup:
    
#if defined(FRAMEWORK_XMLLITE)
    ReleaseObject(pXMLLiteReader);
#elif defined(FRAMEWORK_LIBXML)
    ReleaseObject(pLibXMLReader);
#elif defined(FRAMEWORK_TINYXML2)
	ReleaseObject(pTinyXML2Reader);
#endif

    return hr;
}