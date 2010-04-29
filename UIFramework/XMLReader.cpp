#include "XMLReader.h"
#include "ErrorChecking.h"
#include "XMLLiteReader.h"

HRESULT CreateXMLReader(CXMLReader** ppReader)
{
    HRESULT hr = S_OK;
    CXMLLiteReader* pXMLLiteReader = NULL;

    IFCPTR(ppReader);

    if(SUCCEEDED(CXMLLiteReader::Create(&pXMLLiteReader)))
    {
        *ppReader = pXMLLiteReader;
        pXMLLiteReader = NULL;
        goto Cleanup;
    }

Cleanup:
    ReleaseObject(pXMLLiteReader);

    return hr;
}