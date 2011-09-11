#include "XMLReader.h"
#include "ErrorChecking.h"

#ifdef _WINDOWS
#include "XMLLiteReader.h"
#else
#include "LibXMLReader.h"
#endif

__checkReturn HRESULT 
CreateXMLReader(
	__deref_out CXMLReader** ppReader
	)
{
    HRESULT hr = S_OK;
    
#ifdef _WINDOWS    
    CXMLLiteReader* pXMLLiteReader = NULL;
#else
    CLibXMLReader* pLibXMLReader = NULL;
#endif

    IFCPTR(ppReader);

#ifdef _WINDOWS    
    if(SUCCEEDED(CXMLLiteReader::Create(&pXMLLiteReader)))
    {
        *ppReader = pXMLLiteReader;
        pXMLLiteReader = NULL;
        goto Cleanup;
    }
#else
    if(SUCCEEDED(CLibXMLReader::Create(&pLibXMLReader)))
    {
        *ppReader = pLibXMLReader;
        pLibXMLReader = NULL;
        goto Cleanup;
    }
#endif
    
    IFC(E_FAIL);

Cleanup:
    
#ifdef _WINDOWS
    ReleaseObject(pXMLLiteReader);
#else
    ReleaseObject(pLibXMLReader);
#endif

    return hr;
}