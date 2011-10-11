#pragma once

#include "Types.h"
#include "PropertyObject.h"

class CGraphicsDevice;
class CVisual;
class CProperty;
class CTimeSource;
struct IResourceProvider;

class UIFRAMEWORK_API CVisualAttachContext 
{
    public:
        CVisualAttachContext(
            );

        CVisualAttachContext( 
            __in CVisual* pParent,
            __in_opt OnValueChangeFunc ChangeFunc,
            __in CTimeSource* pTimeSource,
            __in CGraphicsDevice* pGraphicsDevice
            );

        __out_opt CVisual* GetParent(
            );

        __out_opt CTimeSource* GetTimeSource(
            );

        __out_opt OnValueChangeFunc GetChangeCallback(
            );

        __out_opt CGraphicsDevice* GetGraphicsDevice(
            );

        void Reset(
            );

    protected:
        CVisual* m_pParent;
        OnValueChangeFunc m_pChangeCallback;
        CTimeSource* m_pTimeSource;
        CGraphicsDevice* m_pGraphicsDevice;
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