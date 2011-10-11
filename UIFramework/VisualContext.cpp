#include "VisualContext.h"

CVisualAttachContext::CVisualAttachContext(
    )
    : m_pParent(NULL)
    , m_pChangeCallback(NULL)
    , m_pGraphicsDevice(NULL)
    , m_pTimeSource(NULL)
{
}

CVisualAttachContext::CVisualAttachContext(
    __in CVisual* pParent,
    __in_opt OnValueChangeFunc ChangeFunc,
    __in CTimeSource* pTimeSource,
    __in CGraphicsDevice* pGraphicsDevice
    )
    : m_pParent(pParent)
    , m_pChangeCallback(ChangeFunc)
    , m_pTimeSource(pTimeSource)
    , m_pGraphicsDevice(pGraphicsDevice)
{
}

__out_opt CVisual*
CVisualAttachContext::GetParent(
    )
{
    return m_pParent;
}

__out_opt OnValueChangeFunc
CVisualAttachContext::GetChangeCallback(
    )
{
    return m_pChangeCallback;
}

__out_opt CGraphicsDevice*
CVisualAttachContext::GetGraphicsDevice(
    )
{
    return m_pGraphicsDevice;
}

__out_opt CTimeSource*
CVisualAttachContext::GetTimeSource(
    )
{
    return m_pTimeSource;
}

void
CVisualAttachContext::Reset(
    )
{
    m_pParent = NULL;
    m_pChangeCallback = NULL;
    m_pGraphicsDevice = NULL;
    m_pTimeSource = NULL;
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