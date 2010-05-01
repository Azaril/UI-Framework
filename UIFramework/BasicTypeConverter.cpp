#include "BasicTypeConverter.h"
#include "StaticTypeConverter.h"
#include "TypeIndex.h"
#include "BasicTypes.h"

StaticTypeConverter BasicConverters[] =
{
    { TypeIndex::String, TypeIndex::Float, ConvertStringToFloat },
    { TypeIndex::String, TypeIndex::Visibility, ConvertStringToVisibility },
    { TypeIndex::String, TypeIndex::ColorF, ConvertStringToColorF }, 
    { TypeIndex::String, TypeIndex::RectF, ConvertStringToRectF },
};

StaticTypeConverterInformation BasicConverterInfo =
{
    BasicConverters,
    ARRAYSIZE(BasicConverters)
};

HRESULT CreateBasicTypeConverter(CTypeConverter** ppTypeConverter)
{
    HRESULT hr = S_OK;
    CStaticTypeConverter* pStaticTypeConverter = NULL;

    IFCPTR(ppTypeConverter);

    IFC(CStaticTypeConverter::Create(&BasicConverterInfo, &pStaticTypeConverter));

    *ppTypeConverter = pStaticTypeConverter;
    pStaticTypeConverter = NULL;

Cleanup:
    ReleaseObject(pStaticTypeConverter);

    return hr;
}

HRESULT ConvertStringToFloat(CObjectWithType* pValue, TypeIndex::Value TargetType, CObjectWithType** ppConvertedValue)
{
    HRESULT hr = S_OK;
    FLOAT Value = 0;
    CStringValue* pStringValue = NULL;
    CFloatValue* pFloatValue = NULL;

    IFCPTR(pValue);
    IFCPTR(ppConvertedValue);

    IFCEXPECT(pValue->GetType() == TypeIndex::String);

    pStringValue = (CStringValue*)pValue;

    Value = (FLOAT)_wtof(pStringValue->GetValue());

    IFC(CFloatValue::Create(Value, &pFloatValue));

    *ppConvertedValue = pFloatValue;
    pFloatValue = NULL;

Cleanup:
    ReleaseObject(pFloatValue);

    return hr;
}

HRESULT ConvertStringToVisibility(CObjectWithType* pValue, TypeIndex::Value TargetType, CObjectWithType** ppConvertedValue)
{
    HRESULT hr = S_OK;
    CStringValue* pStringValue = NULL;
    CVisibilityValue* pVisibilityValue = NULL;
    Visibility::Value Value;

    IFCPTR(pValue);
    IFCPTR(ppConvertedValue);

    IFCEXPECT(pValue->GetType() == TypeIndex::String);

    pStringValue = (CStringValue*)pValue;

    if(wcscmp(pStringValue->GetValue(), L"Visible") == 0)
    {
        Value = Visibility::Visible;
    }
    else if(wcscmp(pStringValue->GetValue(), L"Hidden") == 0)
    {
        Value = Visibility::Hidden;
    }
    else if(wcscmp(pStringValue->GetValue(), L"Collapsed") == 0)
    {
        Value = Visibility::Collapsed;
    }
    else
    {
        IFC(E_FAIL);
    }

    IFC(CVisibilityValue::Create(Value, &pVisibilityValue));

    *ppConvertedValue = pVisibilityValue;
    pVisibilityValue = NULL;

Cleanup:
    ReleaseObject(pVisibilityValue);

    return hr;
}

struct PredefinedColor
{
    const WCHAR* ColorName;
    ColorF ColorValue;
};

//TODO: Remove relying on the ColorF constructor.
#define PREDEFINED_COLOR(color) { L#color, D2D1::ColorF(D2D1::ColorF::##color) }

PredefinedColor PredefinedColors[] =
{
    PREDEFINED_COLOR( AliceBlue ),
    PREDEFINED_COLOR( AntiqueWhite ),
    PREDEFINED_COLOR( Aqua ),
    PREDEFINED_COLOR( Aquamarine ),
    PREDEFINED_COLOR( Azure ),
    PREDEFINED_COLOR( Beige ),
    PREDEFINED_COLOR( Bisque ),
    PREDEFINED_COLOR( Black ),
    PREDEFINED_COLOR( BlanchedAlmond ),
    PREDEFINED_COLOR( Blue ),
    PREDEFINED_COLOR( BlueViolet ),
    PREDEFINED_COLOR( Brown ),
    PREDEFINED_COLOR( BurlyWood ),
    PREDEFINED_COLOR( CadetBlue ),
    PREDEFINED_COLOR( Chartreuse ),
    PREDEFINED_COLOR( Chocolate ),
    PREDEFINED_COLOR( Coral ),
    PREDEFINED_COLOR( CornflowerBlue ),
    PREDEFINED_COLOR( Cornsilk ),
    PREDEFINED_COLOR( Crimson ),
    PREDEFINED_COLOR( Cyan ),
    PREDEFINED_COLOR( DarkBlue ),
    PREDEFINED_COLOR( DarkCyan ),
    PREDEFINED_COLOR( DarkGoldenrod ),
    PREDEFINED_COLOR( DarkGray ),
    PREDEFINED_COLOR( DarkGreen ),
    PREDEFINED_COLOR( DarkKhaki ),
    PREDEFINED_COLOR( DarkMagenta ),
    PREDEFINED_COLOR( DarkOliveGreen ),
    PREDEFINED_COLOR( DarkOrange ),
    PREDEFINED_COLOR( DarkOrchid ),
    PREDEFINED_COLOR( DarkRed ),
    PREDEFINED_COLOR( DarkSalmon ),
    PREDEFINED_COLOR( DarkSeaGreen ),
    PREDEFINED_COLOR( DarkSlateBlue ),
    PREDEFINED_COLOR( DarkSlateGray ),
    PREDEFINED_COLOR( DarkTurquoise ),
    PREDEFINED_COLOR( DarkViolet ),
    PREDEFINED_COLOR( DeepPink ),
    PREDEFINED_COLOR( DeepSkyBlue ),
    PREDEFINED_COLOR( DimGray ),
    PREDEFINED_COLOR( DodgerBlue ),
    PREDEFINED_COLOR( Firebrick ),
    PREDEFINED_COLOR( FloralWhite ),
    PREDEFINED_COLOR( ForestGreen ),
    PREDEFINED_COLOR( Fuchsia ),
    PREDEFINED_COLOR( Gainsboro ),
    PREDEFINED_COLOR( GhostWhite ),
    PREDEFINED_COLOR( Gold ),
    PREDEFINED_COLOR( Goldenrod ),
    PREDEFINED_COLOR( Gray ),
    PREDEFINED_COLOR( Green ),
    PREDEFINED_COLOR( GreenYellow ),
    PREDEFINED_COLOR( Honeydew ),
    PREDEFINED_COLOR( HotPink ),
    PREDEFINED_COLOR( IndianRed ),
    PREDEFINED_COLOR( Indigo ),
    PREDEFINED_COLOR( Ivory ),
    PREDEFINED_COLOR( Khaki ),
    PREDEFINED_COLOR( Lavender ),
    PREDEFINED_COLOR( LavenderBlush ),
    PREDEFINED_COLOR( LawnGreen ),
    PREDEFINED_COLOR( LemonChiffon ),
    PREDEFINED_COLOR( LightBlue ),
    PREDEFINED_COLOR( LightCoral ),
    PREDEFINED_COLOR( LightCyan ),
    PREDEFINED_COLOR( LightGoldenrodYellow ),
    PREDEFINED_COLOR( LightGreen ),
    PREDEFINED_COLOR( LightGray ),
    PREDEFINED_COLOR( LightPink ),
    PREDEFINED_COLOR( LightSalmon ),
    PREDEFINED_COLOR( LightSeaGreen ),
    PREDEFINED_COLOR( LightSkyBlue ),
    PREDEFINED_COLOR( LightSlateGray ),
    PREDEFINED_COLOR( LightSteelBlue ),
    PREDEFINED_COLOR( LightYellow ),
    PREDEFINED_COLOR( Lime ),
    PREDEFINED_COLOR( LimeGreen ),
    PREDEFINED_COLOR( Linen ),
    PREDEFINED_COLOR( Magenta ),
    PREDEFINED_COLOR( Maroon ),
    PREDEFINED_COLOR( MediumAquamarine ),
    PREDEFINED_COLOR( MediumBlue ),
    PREDEFINED_COLOR( MediumOrchid ),
    PREDEFINED_COLOR( MediumPurple ),
    PREDEFINED_COLOR( MediumSeaGreen ),
    PREDEFINED_COLOR( MediumSlateBlue ),
    PREDEFINED_COLOR( MediumSpringGreen ),
    PREDEFINED_COLOR( MediumTurquoise ),
    PREDEFINED_COLOR( MediumVioletRed ),
    PREDEFINED_COLOR( MidnightBlue ),
    PREDEFINED_COLOR( MintCream ),
    PREDEFINED_COLOR( MistyRose ),
    PREDEFINED_COLOR( Moccasin ),
    PREDEFINED_COLOR( NavajoWhite ),
    PREDEFINED_COLOR( Navy ),
    PREDEFINED_COLOR( OldLace ),
    PREDEFINED_COLOR( Olive ),
    PREDEFINED_COLOR( OliveDrab ),
    PREDEFINED_COLOR( Orange ),
    PREDEFINED_COLOR( OrangeRed ),
    PREDEFINED_COLOR( Orchid ),
    PREDEFINED_COLOR( OrangeRed ),
    PREDEFINED_COLOR( PaleGoldenrod ),
    PREDEFINED_COLOR( PaleGreen ),
    PREDEFINED_COLOR( PaleTurquoise ),
    PREDEFINED_COLOR( PaleVioletRed ),
    PREDEFINED_COLOR( PapayaWhip ),
    PREDEFINED_COLOR( PeachPuff ),
    PREDEFINED_COLOR( Peru ),
    PREDEFINED_COLOR( Pink ),
    PREDEFINED_COLOR( Plum ),
    PREDEFINED_COLOR( PowderBlue ),
    PREDEFINED_COLOR( Purple ),
    PREDEFINED_COLOR( Red ),
    PREDEFINED_COLOR( RosyBrown ),
    PREDEFINED_COLOR( RoyalBlue ),
    PREDEFINED_COLOR( SaddleBrown ),
    PREDEFINED_COLOR( Salmon ),
    PREDEFINED_COLOR( SandyBrown ),
    PREDEFINED_COLOR( SeaGreen ),
    PREDEFINED_COLOR( SeaShell ),
    PREDEFINED_COLOR( Sienna ),
    PREDEFINED_COLOR( Silver ),
    PREDEFINED_COLOR( SkyBlue ),
    PREDEFINED_COLOR( SlateBlue ),
    PREDEFINED_COLOR( SlateGray ),
    PREDEFINED_COLOR( Snow ),
    PREDEFINED_COLOR( SpringGreen ),
    PREDEFINED_COLOR( SteelBlue ),
    PREDEFINED_COLOR( Tan ),
    PREDEFINED_COLOR( Teal ),
    PREDEFINED_COLOR( Thistle ),
    PREDEFINED_COLOR( Tomato ),
    PREDEFINED_COLOR( Turquoise ),
    PREDEFINED_COLOR( Violet ),
    PREDEFINED_COLOR( Wheat ),
    PREDEFINED_COLOR( White ),
    PREDEFINED_COLOR( WhiteSmoke ),
    PREDEFINED_COLOR( Yellow ),
    PREDEFINED_COLOR( YellowGreen )
};

HRESULT ConvertStringToColorF(CObjectWithType* pValue, TypeIndex::Value TargetType, CObjectWithType** ppConvertedValue)
{
    HRESULT hr = S_OK;
    CStringValue* pStringValue = NULL;
    CColorFValue* pColorFValue = NULL;
    ColorF Value;
    BOOL GotColor = FALSE;

    IFCPTR(pValue);
    IFCPTR(ppConvertedValue);

    IFCEXPECT(pValue->GetType() == TypeIndex::String);

    pStringValue = (CStringValue*)pValue;

    for(UINT32 i = 0; i < ARRAYSIZE(PredefinedColors) && !GotColor; i++)
    {
        if(wcscmp(PredefinedColors[i].ColorName, pStringValue->GetValue()) == 0)
        {
            Value = PredefinedColors[i].ColorValue;
            GotColor = TRUE;
        }
    }

    IFCEXPECT(GotColor);

    IFC(CColorFValue::Create(Value, &pColorFValue));

    *ppConvertedValue = pColorFValue;
    pColorFValue = NULL;

Cleanup:
    ReleaseObject(pColorFValue);

    return hr;
}

HRESULT ConvertStringToRectF(CObjectWithType* pValue, TypeIndex::Value TargetType, CObjectWithType** ppConvertedValue)
{
    HRESULT hr = S_OK;
    CStringValue* pStringValue = NULL;
    CRectFValue* pRectFValue = NULL;
    RectF Value = { 0 };

    IFCPTR(pValue);
    IFCPTR(ppConvertedValue);

    IFCEXPECT(pValue->GetType() == TypeIndex::String);

    pStringValue = (CStringValue*)pValue;

    //TODO: Implement!
    __debugbreak();

    IFC(CRectFValue::Create(Value, &pRectFValue));

    *ppConvertedValue = pRectFValue;
    pRectFValue = NULL;

Cleanup:
    ReleaseObject(pRectFValue);

    return hr;
}