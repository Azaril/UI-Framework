#pragma once

#include "Types.h"

class CGraphicsDevice;
class CVisual;

class CVisualAttachContext 
{
    public:
        CVisualAttachContext() : m_Parent(NULL),
                                 m_GraphicsDevice(NULL)
        {
        }

        CVisualAttachContext( CVisual* pParent ) : m_Parent(pParent),
                                                   m_GraphicsDevice(NULL)
        {
        }

        CVisualAttachContext( CVisual* pParent, CGraphicsDevice* pGraphicsDevice ) : m_Parent(pParent),
                                                                                     m_GraphicsDevice(pGraphicsDevice)
        {
        }

        CVisual* GetParent();

        CGraphicsDevice* GetGraphicsDevice()
        {
            return m_GraphicsDevice;
        }

        void Reset()
        {
            m_GraphicsDevice = NULL;
            m_Parent = NULL;
        }

    protected:
        CVisual* m_Parent;
        CGraphicsDevice* m_GraphicsDevice;
};

class CVisualDetachContext 
{
    public:
        CVisualDetachContext() : m_Parent(NULL),
                                 m_GraphicsDevice(NULL)
        {
        }

        CVisualDetachContext( CVisual* pParent ) : m_Parent(pParent),
                                                   m_GraphicsDevice(NULL)
        {
        }

        CVisualDetachContext( CVisual* pParent, CGraphicsDevice* pGraphicsDevice ) : m_Parent(pParent),
                                                                                     m_GraphicsDevice(pGraphicsDevice)
        {
        }

        CVisual* GetParent();

        CGraphicsDevice* GetGraphicsDevice()
        {
            return m_GraphicsDevice;
        }

        void Reset()
        {
            m_GraphicsDevice = NULL;
            m_Parent = NULL;
        }

    protected:
        CVisual* m_Parent;
        CGraphicsDevice* m_GraphicsDevice;
};