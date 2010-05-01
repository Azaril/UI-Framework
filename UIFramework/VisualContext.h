#pragma once

#include "GraphicsDevice.h"

class CVisualAttachContext 
{
    public:
        CVisualAttachContext() : m_GraphicsDevice(NULL)
        {
        }

        CVisualAttachContext( CGraphicsDevice* pGraphicsDevice ) : m_GraphicsDevice(pGraphicsDevice)
        {
        }

        CGraphicsDevice* GetGraphicsDevice()
        {
            return m_GraphicsDevice;
        }

        void Reset()
        {
            m_GraphicsDevice = NULL;
        }

    protected:
        CGraphicsDevice* m_GraphicsDevice;
};

class CVisualDetachContext 
{
    public:
        CVisualDetachContext() : m_GraphicsDevice(NULL)
        {
        }

        CVisualDetachContext( CGraphicsDevice* pGraphicsDevice ) : m_GraphicsDevice(pGraphicsDevice)
        {
        }

        CGraphicsDevice* GetGraphicsDevice()
        {
            return m_GraphicsDevice;
        }

        void Reset()
        {
            m_GraphicsDevice = NULL;
        }

    protected:
        CGraphicsDevice* m_GraphicsDevice;
};