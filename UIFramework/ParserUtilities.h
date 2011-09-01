#pragma once

#include "ParseContext.h"
#include "XMLReader.h"
#include "ElementNodeCallback.h"
#include "PropertyNodeCallback.h"
#include "TypeConverter.h"
#include "PropertyObject.h"

__checkReturn HRESULT ElementStartToParserCallback( 
	__in CParseContext* pContext,
	__in CXMLElementStart* pStart, 
	__deref_out CElementNodeCallback** ppCallback 
	);

__checkReturn HRESULT ElementStartToParserCallback( 
	__in CParseContext* pContext, 
	__in CPropertyInformation* pPropertyInformation, 
	__in CXMLElementStart* pStart,
	__deref_out CPropertyNodeCallback** ppCallback 
	);

__checkReturn HRESULT TextToParserCallback( 
	__in CParseContext* pContext,
	__in CPropertyInformation* pPropertyInformation,
	__in CXMLText* pText, 
	__deref_out CPropertyNodeCallback** ppCallback 
	);

__checkReturn HRESULT AttributeStringToValue(
	__in CParseContext* pContext, 
	__in_ecount(ValueLength) const WCHAR* pValue, 
	UINT32 ValueLength, 
	__deref_out CObjectWithType** ppValue 
	);

__checkReturn HRESULT AddCreateObjectCommand(
	__in CParseContext* pContext,
	__in CResolvedClass* pClass 
	);

__checkReturn HRESULT AddPushValueCommand( 
	__in CParseContext* pContext,
	__in CObjectWithType* pValue 
	);

__checkReturn HRESULT AddSetPropertyCommand( 
	__in CParseContext* pContext, 
	__in CProperty* pProperty,
	__in_opt CStringValue* pKeyString = NULL 
	);

__checkReturn HRESULT AddEvaluateMarkupExtensionCommand( 
	__in CParseContext* pContext 
	);

__checkReturn HRESULT EvaluateAndAddAttribute( 
	__in CParseContext* pContext,
	__in_ecount(ValueLength) const WCHAR* pValue, 
	UINT32 ValueLength 
	);