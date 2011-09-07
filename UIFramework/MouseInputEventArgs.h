#pragma once

#include "Enums.h"
#include "InputEventArgs.h"

class CVisual;

class CMouseEventArgs : public CInputEventArgs
{
    public:
        DECLARE_FACTORY2( CMouseEventArgs, CRoutedEvent*, Point2F );
        DECLARE_FACTORY2( CMouseEventArgs, CRoutedEvent*, CMouseEventArgs* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::MouseEventArgs, CInputEventArgs );

        __checkReturn HRESULT GetLocation( 
            __out Point2F* pLocation 
            );

        __checkReturn HRESULT GetLocation( 
            __in CVisual* pVisual, 
            __out Point2F* pLocation 
            );

    protected:
        CMouseEventArgs(
            );

        virtual ~CMouseEventArgs(
            );

        __checkReturn HRESULT Initialize( 
            __in CRoutedEvent* pRoutedEvent,
            Point2F MouseLocation 
            );

        __checkReturn HRESULT Initialize( 
            __in CRoutedEvent* pRoutedEvent,
            __in CMouseEventArgs* pSourceArgs 
            );

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

        MouseButton::Value GetChangedButton(
            );

        MouseButtonState::Value GetButtonState(
            );

    protected:
        CMouseButtonEventArgs(
            );

        virtual ~CMouseButtonEventArgs(
            );

        __checkReturn HRESULT Initialize( 
            __in CRoutedEvent* pRoutedEvent, 
            Point2F MouseLocation, 
            MouseButton::Value Button,
            MouseButtonState::Value State 
            );

        __checkReturn HRESULT Initialize(
            __in CRoutedEvent* pRoutedEvent,
            __in CMouseButtonEventArgs* pSourceArgs 
            );

        MouseButton::Value m_Button;
        MouseButtonState::Value m_ButtonState;
};

template< >
struct ObjectTypeTraits< CMouseButtonEventArgs >
{
    static const TypeIndex::Value Type = TypeIndex::MouseButtonEventArgs;
};