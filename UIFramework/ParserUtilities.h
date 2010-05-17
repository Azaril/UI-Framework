#pragma once

#include "ParseContext.h"
#include "XMLReader.h"
#include "ElementNodeCallback.h"
#include "PropertyNodeCallback.h"
#include "TypeConverter.h"
#include "PropertyObject.h"

HRESULT ElementStartToParserCallback( CParseContext* pContext, CXMLElementStart* pStart, CElementNodeCallback** ppCallback );
HRESULT ElementStartToParserCallback( CParseContext* pContext, CPropertyInformation* pPropertyInformation, CXMLElementStart* pStart, CPropertyNodeCallback** ppCallback );
HRESULT TextToParserCallback( CParseContext* pContext, CPropertyInformation* pPropertyInformation, CXMLText* pText, CPropertyNodeCallback** ppCallback );

HRESULT AttributeStringToValue( CParseContext* pContext, const WCHAR* pValue, UINT32 ValueLength, CObjectWithType** ppValue );

HRESULT AddCreateObjectCommand( CParseContext* pContext, CResolvedClass* pClass );
HRESULT AddPushValueCommand( CParseContext* pContext, CObjectWithType* pValue );
HRESULT AddSetPropertyCommand( CParseContext* pContext, CProperty* pProperty, CStringValue* pKeyString = NULL );
HRESULT AddEvaluateMarkupExtensionCommand( CParseContext* pContext );

HRESULT EvaluateAndAddAttribute( CParseContext* pContext, const WCHAR* pValue, UINT32 ValueLength );