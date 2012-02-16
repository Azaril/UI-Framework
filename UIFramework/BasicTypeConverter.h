#pragma once

#include "TypeConverter.h"

__checkReturn HRESULT UIFRAMEWORK_API CreateBasicTypeConverter(
    __deref_out CTypeConverter** ppTypeConverter 
    );

__checkReturn HRESULT ConvertStringToFloat(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

__checkReturn HRESULT ConvertStringToVisibility(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

__checkReturn HRESULT ConvertStringToColorF(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

__checkReturn HRESULT ConvertStringToRectF(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

__checkReturn HRESULT ConvertStringToRectangleEdge(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

__checkReturn HRESULT ConvertStringToBool(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

__checkReturn HRESULT ConvertStringToPoint2F(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

__checkReturn HRESULT ConvertStringToRoutedEvent(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

__checkReturn HRESULT ConvertStringToBrush(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

__checkReturn HRESULT ConvertStringToHorizontalAlignment(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

__checkReturn HRESULT ConvertStringToVerticalAlignment(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

__checkReturn HRESULT ConvertStringToCommand(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

__checkReturn HRESULT ConvertStringToBindingDirection(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

__checkReturn HRESULT ConvertStringToStretch(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

__checkReturn HRESULT ConvertStringToStretchDirection(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

__checkReturn HRESULT ConvertStringToOrientation(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

__checkReturn HRESULT ConvertStringToDuration(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );

__checkReturn HRESULT ConvertStringToGraphicsGeometry(
    __in CConversionContext* pContext, 
    __in CObjectWithType* pValue, 
    __deref_out CObjectWithType** ppConvertedValue 
    );