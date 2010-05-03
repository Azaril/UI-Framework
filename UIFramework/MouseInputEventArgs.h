#pragma once

#include "Enums.h"
#include "InputEventArgs.h"

class CMouseEventArgs : public CInputEventArgs
{
    public:
        DECLARE_FACTORY2( CMouseEventArgs, CRoutedEvent*, Point2F );
        DECLARE_FACTORY2( CMouseEventArgs, CRoutedEvent*, CMouseEventArgs* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::MouseEventArgs, CInputEventArgs );

        Point2F GetLocation();

    protected:
        CMouseEventArgs();
        virtual ~CMouseEventArgs();

        HRESULT Initialize( CRoutedEvent* pRoutedEvent, Point2F MouseLocation );
        HRESULT Initialize( CRoutedEvent* pRoutedEvent, CMouseEventArgs* pSourceArgs );

        Point2F m_Location;
};

template< >
struct ObjectTypeTraits< CMouseEventArgs >
{
    static const TypeIndex::Value Type = TypeIndex::MouseEventArgs;
};

class CMouseButtonEventArgs : public CMouseEventArgs
{
    public:
        DECLARE_FACTORY4( CMouseButtonEventArgs, CRoutedEvent*, Point2F, MouseButton::Value, MouseButtonState::Value );
        DECLARE_FACTORY2( CMouseButtonEventArgs, CRoutedEvent*, CMouseButtonEventArgs* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::MouseButtonEventArgs, CMouseEventArgs );

        MouseButton::Value GetChangedButton();
        MouseButtonState::Value GetButtonState();

    protected:
        CMouseButtonEventArgs();
        virtual ~CMouseButtonEventArgs();

        HRESULT Initialize( CRoutedEvent* pRoutedEvent, Point2F MouseLocation, MouseButton::Value Button, MouseButtonState::Value State );
        HRESULT Initialize( CRoutedEvent* pRoutedEvent, CMouseButtonEventArgs* pSourceArgs );

        MouseButton::Value m_Button;
        MouseButtonState::Value m_ButtonState;
};

template< >
struct ObjectTypeTraits< CMouseButtonEventArgs >
{
    static const TypeIndex::Value Type = TypeIndex::MouseButtonEventArgs;
};