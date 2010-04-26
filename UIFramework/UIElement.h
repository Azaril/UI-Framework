#pragma once

#include "Visual.h"
#include "RectangleVisual.h"
#include "RenderTarget.h"

class CUIElement;

class CUIAttachContext
{
    public:
        CUIAttachContext() : m_Parent(NULL)
        {
        }

        CUIAttachContext( CUIElement* pParent ) : m_Parent(pParent)
        {
        }
    
        CUIElement* GetParent()
        {
            return m_Parent;
        }
    
    protected:
        CUIElement* m_Parent;
};

class CUIDetachContext
{
    public:
        CUIDetachContext() : m_Parent(NULL)
        {
        }

        CUIDetachContext( CUIElement* pParent ) : m_Parent(pParent)
        {
        }
    
        CUIElement* GetParent()
        {
            return m_Parent;
        }
        
    protected:
        CUIElement* m_Parent;
};

class CUIElement : public CVisual
{
    public:
        virtual HRESULT OnAttach( CUIAttachContext& Context );
        virtual HRESULT OnDetach( CUIDetachContext& Context );

        BOOL IsAttached();

        HRESULT SetSize( SizeF Size );

        virtual HRESULT Measure( SizeF Size );
        virtual SizeF GetDesiredSize();

        virtual HRESULT Arrange( SizeF Size );
    
    protected:
        CUIElement();
        virtual ~CUIElement();
    
        HRESULT Initialize();

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
   
        BOOL m_Attached;
        CUIAttachContext m_Context;
        /*CRectangleVisual* m_Background;*/

        SizeF m_Size;

    private:
        SizeF m_DesiredSize;
        SizeF m_FinalSize;
};