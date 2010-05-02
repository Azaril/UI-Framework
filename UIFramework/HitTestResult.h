#pragma once

#include "RefCounted.h"
#include "Factory.h"

class CVisual;

class CHitTestResult : public CRefCountedObject
{
    public:
        DECLARE_FACTORY1( CHitTestResult, CVisual* );

        virtual HRESULT GetHitVisual( CVisual** ppVisual );

    protected:
        CHitTestResult();
        virtual ~CHitTestResult();

        HRESULT Initialize( CVisual* pVisual );

        CVisual* m_HitVisual;
};