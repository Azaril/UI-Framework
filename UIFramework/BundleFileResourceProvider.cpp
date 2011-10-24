#include "BundleFileResourceProvider.h"
#include "StackHeapBuffer.h"
#include "StringConversion.h"
#include "FileResourceStream.h"

#include <CoreFoundation/CFBundle.h>
#include <CoreFoundation/CFURL.h>
#include <CoreFoundation/CFString.h>

CBundleFileResourceProvider::CBundleFileResourceProvider(
	)
{
}

CBundleFileResourceProvider::~CBundleFileResourceProvider(
	)
{
}

__checkReturn HRESULT
CBundleFileResourceProvider::Initialize(
	)
{
	HRESULT hr = S_OK;

	return hr;
}

__override __checkReturn HRESULT 
CBundleFileResourceProvider::ReadResource(
    __in_ecount(identiferLength) const WCHAR* pIdentifier,
    UINT32 identifierLength,
    __deref_out IReadStream** ppStream
    )
{
	HRESULT hr = S_OK;
	CFBundleRef pMainBundle = NULL;
	CFStringRef pDirectoryPath = NULL;
	CFStringRef pIdentifierName = NULL;
	CFStringRef pExtensionName = NULL;
	CFURLRef pFileUrl = NULL;
	const WCHAR* pDelimiter = NULL;
	const WCHAR* pFileNameStart = NULL;
    FILE* pFile = NULL;
    CFileResourceStream* pFileStream = NULL;
    StackHeapBuffer< CHAR, 2048 > stringBuffer;
    UINT32 stringSize = 0;    

	pMainBundle = CFBundleGetMainBundle();
	IFCPTR(pMainBundle);

	for (INT32 i = ((INT32)identifierLength) - 1; i >= 0; --i)
	{
		if (pIdentifier[i] == L'.')
		{
			pDelimiter = &pIdentifier[i];
            
            break;
		}
	}

	IFCPTR(pIdentifier);

	for (INT32 i = pDelimiter - pIdentifier; i >= 0; --i)
	{
		if (pIdentifier[i] == L'\\' || pIdentifier[i] == L'/')
		{
			pFileNameStart = (&pIdentifier[i]) + 1;
            
            break;
		}
	}

	if (pFileNameStart == NULL)
	{
		pFileNameStart = pIdentifier;
	}

	IFCEXPECT(pFileNameStart != pDelimiter);

    IFC(ConvertWCHARToUTF8< 2048 >(pFileNameStart, (pDelimiter - pFileNameStart), &stringBuffer, &stringSize));

    pIdentifierName = CFStringCreateWithBytes(NULL, (const BYTE*)stringBuffer.GetBuffer(), stringSize, kCFStringEncodingUTF8, false);
    IFCPTR(pIdentifierName);

    IFC(ConvertWCHARToUTF8< 2048 >(pDelimiter + 1, (pIdentifier + identifierLength) - (pDelimiter + 1), &stringBuffer, &stringSize));

    pExtensionName = CFStringCreateWithBytes(NULL, (const BYTE*)stringBuffer.GetBuffer(), stringSize, kCFStringEncodingUTF8, false);
    IFCPTR(pExtensionName);

    if (pIdentifier != pFileNameStart)
    {
        IFC(ConvertWCHARToUTF8< 2048 >(pIdentifier, (pFileNameStart - 1) - pIdentifier, &stringBuffer, &stringSize));

        pDirectoryPath = CFStringCreateWithBytes(NULL, (const BYTE*)stringBuffer.GetBuffer(), stringSize, kCFStringEncodingUTF8, false);
        IFCPTR(pDirectoryPath);
    }
    else
    {
        pDirectoryPath = CFSTR("");
    }

	pFileUrl = CFBundleCopyResourceURL(pMainBundle, pIdentifierName, pExtensionName, pDirectoryPath);
    IFCPTR(pFileUrl);
    
    IFCEXPECT(CFURLGetFileSystemRepresentation(pFileUrl, FALSE, (BYTE*)stringBuffer.GetBuffer(), stringBuffer.GetBufferSize()));
    
    pFile = fopen((const CHAR*)stringBuffer.GetBuffer(), "rb");
    IFCPTR(pFile);
    
    IFC(CFileResourceStream::Create(pFile, &pFileStream));
    
    pFile = NULL;
    
    *ppStream = pFileStream;
    pFileStream = NULL;

Cleanup:
    ReleaseObject(pFileStream);
    
	if (pIdentifierName != NULL)
	{
		CFRelease(pIdentifierName);
	}

	if (pExtensionName != NULL)
	{
		CFRelease(pExtensionName);
	}

	if (pDirectoryPath != NULL)
	{
		CFRelease(pDirectoryPath);
	}

	if (pFileUrl != NULL)
	{
		CFRelease(pFileUrl);
	}
    
    if (pFile != NULL)
    {
        fclose(pFile);
    }

	return hr;
}