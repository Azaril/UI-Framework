#pragma once

#include "RoutedEventArgs.h"

class CDragDeltaEventArgs : public CRoutedEventArgs
{
    public:
        DECLARE_FACTORY3( CDragDeltaEventArgs, CRoutedEvent*, FLOAT, FLOAT );

        DECLARE_TYPE_WITH_BASE( TypeIndex::DragDeltaEventArgs, CRoutedEventArgs );

        FLOAT GetHorizontalDelta();
        FLOAT GetVerticalDelta();

    protected:
        CDragDeltaEventArgs();
        virtual ~CDragDeltaEventArgs();

        HRESULT Initialize( CRoutedEvent* pEvent, FLOAT HorizontalDelta, FLOAT VerticalDelta );

        FLOAT m_HorizontalDelta;
        FLOAT m_VerticalDelta;
};

template< >
struct ObjectTypeTraits< CDragDeltaEventArgs >
{
    static const TypeIndex::Value Type = TypeIndex::DragDeltaEventArgs;
};