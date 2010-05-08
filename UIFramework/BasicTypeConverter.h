#pragma once

#include "TypeConverter.h"

HRESULT CreateBasicTypeConverter( CTypeConverter** ppTypeConverter );

HRESULT ConvertStringToFloat( CObjectWithType* pValue, TypeIndex::Value TargetType, CObjectWithType** ppConvertedValue );
HRESULT ConvertStringToVisibility( CObjectWithType* pValue, TypeIndex::Value TargetType, CObjectWithType** ppConvertedValue );
HRESULT ConvertStringToColorF( CObjectWithType* pValue, TypeIndex::Value TargetType, CObjectWithType** ppConvertedValue );
HRESULT ConvertStringToRectF( CObjectWithType* pValue, TypeIndex::Value TargetType, CObjectWithType** ppConvertedValue );
HRESULT ConvertStringToRectangleEdge( CObjectWithType* pValue, TypeIndex::Value TargetType, CObjectWithType** ppConvertedValue );
HRESULT ConvertStringToBool( CObjectWithType* pValue, TypeIndex::Value TargetType, CObjectWithType** ppConvertedValue );
HRESULT ConvertStringToPoint2F( CObjectWithType* pValue, TypeIndex::Value TargetType, CObjectWithType** ppConvertedValue );