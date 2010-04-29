#pragma once

#include "TypeConverter.h"

HRESULT CreateBasicTypeConverter( CTypeConverter** ppTypeConverter );

HRESULT ConvertStringToFloat( CObjectWithType* pValue, TypeIndex::Value TargetType, CObjectWithType** ppConvertedValue );