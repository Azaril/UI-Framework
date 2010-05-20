#pragma once

#include "InputEventArgs.h"
#include "BasicTypes.h"

class CTextEventArgs : public CInputEventArgs
{
    public:
        DECLARE_FACTORY3( CTextEventArgs, CRoutedEvent*, const WCHAR*, UINT32 );

        DECLARE_TYPE_WITH_BASE( TypeIndex::TextEventArgs, CInputEventArgs );

        const WCHAR* GetText();
        UINT32 GetTextLength();

    protected:
        CTextEventArgs();
        virtual ~CTextEventArgs();

        HRESULT Initialize( CRoutedEvent* pRoutedEvent, const WCHAR* pText, UINT32 TextLength );

        CStringValue* m_Text;
};

template< >
struct ObjectTypeTraits< CTextEventArgs >
{
    static const TypeIndex::Value Type = TypeIndex::TextEventArgs;
};