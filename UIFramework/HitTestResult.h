#pragma once

#include "RefCounted.h"
#include "Factory.h"

class CVisual;

class CHitTestResult : public CRefCountedObject
{
    public:
        DECLARE_FACTORY1( CHitTestResult, CVisual* );

        virtual HRESULT GetHitVisual(
            __deref_out CVisual** ppVisual 
            );

    protected:
        CHitTestResult(
            );

        virtual ~CHitTestResult(
            );

        __checkReturn HRESULT Initialize(
            __in CVisual* pVisual 
            );

        CVisual* m_HitVisual;
};