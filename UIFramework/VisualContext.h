#pragma once

#include "Types.h"
#include "PropertyObject.h"

class CGraphicsDevice;
class CVisual;
class CProperty;

class UIFRAMEWORK_API CVisualAttachContext 
{
    public:
        CVisualAttachContext(
            );

        CVisualAttachContext(
            __in CVisual* pParent,
            __in_opt OnValueChangeFunc ChangeFunc
            );

        CVisualAttachContext( 
            __in CVisual* pParent,
            __in_opt OnValueChangeFunc ChangeFunc,
            __in CGraphicsDevice* pGraphicsDevice
            );

        __out_opt CVisual* GetParent(
            );

        __out_opt OnValueChangeFunc GetChangeCallback(
            );

        __out_opt CGraphicsDevice* GetGraphicsDevice(
            );

        void Reset(
            );

    protected:
        CVisual* m_Parent;
        OnValueChangeFunc m_ChangeCallback;
        CGraphicsDevice* m_GraphicsDevice;
};

class UIFRAMEWORK_API CVisualDetachContext 
{
    public:
        CVisualDetachContext(
            );

        CVisualDetachContext(
            __in CVisual* pParent,
            __in_opt OnValueChangeFunc ChangeFunc
            );

        CVisualDetachContext( 
            __in CVisual* pParent,
            __in_opt OnValueChangeFunc ChangeFunc,
            __in CGraphicsDevice* pGraphicsDevice
            );

        __out_opt CVisual* GetParent(
            );

        __out_opt OnValueChangeFunc GetChangeCallback(
            );

        __out_opt CGraphicsDevice* GetGraphicsDevice(
            );

        void Reset(
            );

    protected:
        CVisual* m_Parent;
        OnValueChangeFunc m_ChangeCallback;
        CGraphicsDevice* m_GraphicsDevice;
};