#pragma once

#include "TypeConverter.h"

HRESULT CreateBasicTypeConverter( CTypeConverter** ppTypeConverter );

HRESULT ConvertStringToFloat( CConversionContext* pContext, CObjectWithType* pValue, CObjectWithType** ppConvertedValue );
HRESULT ConvertStringToVisibility( CConversionContext* pContext, CObjectWithType* pValue, CObjectWithType** ppConvertedValue );
HRESULT ConvertStringToColorF( CConversionContext* pContext, CObjectWithType* pValue, CObjectWithType** ppConvertedValue );
HRESULT ConvertStringToRectF( CConversionContext* pContext, CObjectWithType* pValue, CObjectWithType** ppConvertedValue );
HRESULT ConvertStringToRectangleEdge( CConversionContext* pContext, CObjectWithType* pValue, CObjectWithType** ppConvertedValue );
HRESULT ConvertStringToBool( CConversionContext* pContext, CObjectWithType* pValue, CObjectWithType** ppConvertedValue );
HRESULT ConvertStringToPoint2F( CConversionContext* pContext, CObjectWithType* pValue, CObjectWithType** ppConvertedValue );
HRESULT ConvertStringToRoutedEvent( CConversionContext* pContext, CObjectWithType* pValue, CObjectWithType** ppConvertedValue );
HRESULT ConvertStringToBrush( CConversionContext* pContext, CObjectWithType* pValue, CObjectWithType** ppConvertedValue );
HRESULT ConvertStringToHorizontalAlignment( CConversionContext* pContext, CObjectWithType* pValue, CObjectWithType** ppConvertedValue );
HRESULT ConvertStringToVerticalAlignment( CConversionContext* pContext, CObjectWithType* pValue, CObjectWithType** ppConvertedValue );
HRESULT ConvertStringToCommand( CConversionContext* pContext, CObjectWithType* pValue, CObjectWithType** ppConvertedValue );