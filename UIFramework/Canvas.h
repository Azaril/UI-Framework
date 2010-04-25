#pragma once

#include "Panel.h"

class CCanvasLayoutInformation : public CRefCountedObject
{
    public:
        DECLARE_FACTORY1( CCanvasLayoutInformation, CUIElement* );

        CUIElement* GetUIElement();

        Point2F GetPosition();
        HRESULT SetPosition( Point2F Position );
        
    protected:
        CCanvasLayoutInformation();
        virtual ~CCanvasLayoutInformation();

        HRESULT Initialize( CUIElement* pElement );

        CUIElement* m_Element;
        Point2F m_Position;
};

class CCanvas : public CPanel
{
    typedef std::vector< CCanvasLayoutInformation* > LayoutInformationCollection;

    public:
        DECLARE_FACTORY( CCanvas );

        virtual HRESULT AddChild( CUIElement* pElement );
        virtual HRESULT RemoveChild( CUIElement* pElement );

        virtual HRESULT SetChildPosition( CUIElement* pElement, Point2F Position );

        virtual HRESULT Arrange( SizeF Size );

    public:
        CCanvas();
        virtual ~CCanvas();

        virtual HRESULT Initialize();

        virtual HRESULT GetChildLayout( CUIElement* pElement, CCanvasLayoutInformation** ppLayoutInfo );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );

        LayoutInformationCollection m_LayoutInformation;
};
