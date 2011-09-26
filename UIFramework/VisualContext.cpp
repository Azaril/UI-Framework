#include "VisualContext.h"

CVisualAttachContext::CVisualAttachContext(
    )
    : m_Parent(NULL)
    , m_ChangeCallback(NULL)
    , m_GraphicsDevice(NULL)
    , m_TimeSource(NULL)
{
}

CVisualAttachContext::CVisualAttachContext(
    __in CVisual* pParent,
    __in_opt OnValueChangeFunc ChangeFunc,
    __in CTimeSource* pTimeSource
    )
    : m_Parent(pParent)
    , m_ChangeCallback(ChangeFunc)
    , m_TimeSource(pTimeSource)
    , m_GraphicsDevice(NULL)    
{
}

CVisualAttachContext::CVisualAttachContext(
    __in CVisual* pParent,
    __in_opt OnValueChangeFunc ChangeFunc,
    __in CTimeSource* pTimeSource,
    __in CGraphicsDevice* pGraphicsDevice
    )
    : m_Parent(pParent)
    , m_ChangeCallback(ChangeFunc)
    , m_TimeSource(pTimeSource)
    , m_GraphicsDevice(pGraphicsDevice)
{
}

__out_opt CVisual*
CVisualAttachContext::GetParent(
    )
{
    return m_Parent;
}

__out_opt OnValueChangeFunc
CVisualAttachContext::GetChangeCallback(
    )
{
    return m_ChangeCallback;
}

__out_opt CGraphicsDevice*
CVisualAttachContext::GetGraphicsDevice(
    )
{
    return m_GraphicsDevice;
}

__out_opt CTimeSource*
CVisualAttachContext::GetTimeSource(
    )
{
    return m_TimeSource;
}

void
CVisualAttachContext::Reset(
    )
{
    m_Parent = NULL;
    m_ChangeCallback = NULL;
    m_GraphicsDevice = NULL;
}

CVisualDetachContext::CVisualDetachContext(
    )
    : m_Parent(NULL)
    , m_ChangeCallback(NULL)
    , m_GraphicsDevice(NULL)
{
}

CVisualDetachContext::CVisualDetachContext(
    __in CVisual* pParent,
    __in_opt OnValueChangeFunc ChangeFunc
    )
    : m_Parent(pParent)
    , m_ChangeCallback(ChangeFunc)
    , m_GraphicsDevice(NULL)
{
}

CVisualDetachContext::CVisualDetachContext(
    __in CVisual* pParent,
    __in_opt OnValueChangeFunc ChangeFunc,
    __in CGraphicsDevice* pGraphicsDevice
    )
    : m_Parent(pParent)
    , m_ChangeCallback(ChangeFunc)
    , m_GraphicsDevice(pGraphicsDevice)
{
}

__out_opt CVisual*
CVisualDetachContext::GetParent(
    )
{
    return m_Parent;
}

__out_opt OnValueChangeFunc
CVisualDetachContext::GetChangeCallback(
    )
{
    return m_ChangeCallback;
}

__out_opt CGraphicsDevice*
CVisualDetachContext::GetGraphicsDevice(
    )
{
    return m_GraphicsDevice;
}

void
CVisualDetachContext::Reset(
    )
{
    m_Parent = NULL;
    m_ChangeCallback = NULL;
    m_GraphicsDevice = NULL;
}