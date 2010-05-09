#pragma once

#include "ParseContext.h"
#include "XMLReader.h"
#include "ElementNodeCallback.h"
#include "PropertyNodeCallback.h"
#include "TypeConverter.h"
#include "PropertyObject.h"

HRESULT ElementStartToParserCallback( CParseContext* pContext, CXMLElementStart* pStart, CElementNodeCallback** ppCallback );
HRESULT ElementStartToParserCallback( CParseContext* pContext, CPropertyObject* pParent, CXMLElementStart* pStart, CElementNodeCallback** ppCallback );
HRESULT ElementStartToParserCallback( CParseContext* pContext, CPropertyObject* pParent, CPropertyInformation* pPropertyInformation, CXMLElementStart* pStart, CPropertyNodeCallback** ppCallback );
HRESULT TextToParserCallback( CParseContext* pContext, CPropertyObject* pParent, CPropertyInformation* pPropertyInformation, CXMLText* pText, CPropertyNodeCallback** ppCallback );

HRESULT AttributeStringToValue( CParseContext* pContext, const WCHAR* pValue, UINT32 ValueLength, CObjectWithType** ppValue );

HRESULT AssignProperty( CPropertyObject* pElement, CProperty* pProperty, CObjectWithType* pValue, CParseContext* pContext, CObjectWithType* pKey = NULL );