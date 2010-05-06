#include "Panel.h"

class CDockPanel : public CPanel
{
    public:
        DECLARE_FACTORY( CDockPanel );

        DECLARE_TYPE_WITH_BASE( TypeIndex::DockPanel, CPanel );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

        //
        // Properties
        //
        static CStaticProperty DockProperty;
        static CStaticProperty LastChildFillProperty;

    public:
        CDockPanel();
        virtual ~CDockPanel();

        HRESULT Initialize();

        virtual HRESULT SetValueInternal( CProperty* pProperty, CObjectWithType* pValue );
        virtual HRESULT GetValueInternal( CProperty* pProperty, CObjectWithType** ppValue );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF Size );

        HRESULT SetFillLastChildInternal( BOOL Fill );

        BOOL m_FillLastChild;
};

template< >
struct ObjectTypeTraits< CDockPanel >
{
    static const TypeIndex::Value Type = TypeIndex::DockPanel;
};