#include "XMLReader.h"
#include "ErrorChecking.h"

#ifdef _WINDOWS
#include "XMLLiteReader.h"
#endif

HRESULT CreateXMLReader(CXMLReader** ppReader)
{
    HRESULT hr = S_OK;
    
#ifdef _WINDOWS    
    CXMLLiteReader* pXMLLiteReader = NULL;
#endif

    IFCPTR(ppReader);

#ifdef _WINDOWS    
    if(SUCCEEDED(CXMLLiteReader::Create(&pXMLLiteReader)))
    {
        *ppReader = pXMLLiteReader;
        pXMLLiteReader = NULL;
        goto Cleanup;
    }
#endif
    
    IFC(E_FAIL);

Cleanup:
    
#ifdef _WINDOWS
    ReleaseObject(pXMLLiteReader);
#endif

    return hr;
}