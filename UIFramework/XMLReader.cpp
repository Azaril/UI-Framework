#include "XMLReader.h"
#include "ErrorChecking.h"

#ifdef __WIN32__
#include "XMLLiteReader.h"
#endif

HRESULT CreateXMLReader(CXMLReader** ppReader)
{
    HRESULT hr = S_OK;
    
#ifdef __WIN32__    
    CXMLLiteReader* pXMLLiteReader = NULL;
#endif

    IFCPTR(ppReader);

#ifdef __WIN32__    
    if(SUCCEEDED(CXMLLiteReader::Create(&pXMLLiteReader)))
    {
        *ppReader = pXMLLiteReader;
        pXMLLiteReader = NULL;
        goto Cleanup;
    }
#endif
    
    IFC(E_FAIL);

Cleanup:
    
#ifdef __WIN32__
    ReleaseObject(pXMLLiteReader);
#endif

    return hr;
}