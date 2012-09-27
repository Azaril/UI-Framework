#include "DirectWriteEditableTextLayout.h"
#include "DirectWriteTextLayout.h"

#if defined(FRAMEWORK_DWRITE)

CDirectWriteEditableTextLayout::CDirectWriteEditableTextLayout(
	)
	: m_TextProvider(NULL)
	, m_TextLayout(NULL)
	, m_TextFormat(NULL)
{
    m_MaxSize.width = 0;
    m_MaxSize.height = 0;
}

CDirectWriteEditableTextLayout::~CDirectWriteEditableTextLayout(
	)
{
    ReleaseObject(m_TextProvider);
    ReleaseObject(m_TextLayout);
    ReleaseObject(m_TextFormat);
}

__checkReturn HRESULT 
CDirectWriteEditableTextLayout::Initialize(
	__in CTextProvider* pTextProvider, 
    __in CTextFormat* pFormat,
	const SizeF &Size
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pTextProvider);

    SetObject(m_TextFormat, pFormat);
    SetObject(m_TextProvider, pTextProvider);

    IFC(SetMaxSize(Size));

Cleanup:
    return hr;
}

__override UINT32 
CDirectWriteEditableTextLayout::GetStartPosition(
	)
{
    return 0;
}

__override UINT32 
CDirectWriteEditableTextLayout::GetEndPosition(
	)
{
    return m_Text.length();
}

__override __checkReturn HRESULT 
CDirectWriteEditableTextLayout::SetText(
	__in_ecount_opt(TextLength) const WCHAR* pText,
	UINT32 TextLength
	)
{
    HRESULT hr = S_OK;

    IFCPTR(pText);

    m_Text.assign(pText, TextLength);

    IFC(InvalidateLayout());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CDirectWriteEditableTextLayout::ClearText(
	)
{
    HRESULT hr = S_OK;

    m_Text.clear();

    IFC(InvalidateLayout());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CDirectWriteEditableTextLayout::InsertText(
	UINT32 Position, 
	__in_ecount(pText) const WCHAR* pText, 
	UINT32 TextLength
	)
{
    HRESULT hr = S_OK;

    IFCEXPECT(Position >= GetStartPosition() && Position <= GetEndPosition());
    IFCPTR(pText);

    if(TextLength > 0)
    {
        m_Text.insert(Position, pText, TextLength);
    }

    IFC(InvalidateLayout());

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CDirectWriteEditableTextLayout::RemoveText(
	UINT32 Position, 
	UINT32 Length
	)
{
    HRESULT hr = S_OK;

    IFCEXPECT(Position >= GetStartPosition() && Position + Length <= GetEndPosition());

    if(Length > 0)
    {
        m_Text.erase(Position, Length);

        IFC(InvalidateLayout());
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT
CDirectWriteEditableTextLayout::InvalidateLayout(
	)
{
    HRESULT hr = S_OK;

    ReleaseObject(m_TextLayout);

    return hr;
}

__checkReturn HRESULT 
CDirectWriteEditableTextLayout::EnsureLayout(
	)
{
    HRESULT hr = S_OK;

    if(m_TextLayout == NULL)
    {
        IFC(m_TextProvider->CreateTextLayout(m_Text.c_str(), m_Text.length(), m_TextFormat, m_MaxSize, &m_TextLayout));
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CDirectWriteEditableTextLayout::GetDirectWriteTextLayout(
	__deref_out IDWriteTextLayout** ppLayout
	)
{
    HRESULT hr = S_OK;
    CDirectWriteTextLayout* pLayout = NULL;

    IFCPTR(ppLayout);

    IFC(EnsureLayout());

    pLayout = (CDirectWriteTextLayout*)m_TextLayout;
    IFCPTR(pLayout);

    IFC(pLayout->GetDirectWriteTextLayout(ppLayout));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CDirectWriteEditableTextLayout::SetMaxSize(
	const SizeF &Size
	)
{
    HRESULT hr = S_OK;

    m_MaxSize = Size;

    if(m_TextLayout != NULL)
    {
        IFC(m_TextLayout->SetMaxSize(Size));
    }

Cleanup:
    return hr;
}

__override __checkReturn HRESULT 
CDirectWriteEditableTextLayout::GetMetrics(
	__deref_out CTextLayoutMetrics** ppMetrics
	)
{
    HRESULT hr = S_OK;

    IFCPTR(ppMetrics);

    IFC(EnsureLayout());

    IFC(m_TextLayout->GetMetrics(ppMetrics));

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CDirectWriteEditableTextLayout::GetText(
    __deref_out_ecount(*pTextLength) const WCHAR** ppText,
    __out UINT32* pTextLength
	)
{
    HRESULT hr = S_OK;

    *ppText = m_Text.c_str();
    *pTextLength = m_Text.length();

    return hr;
}

#endif