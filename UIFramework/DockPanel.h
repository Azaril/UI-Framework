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

    protected:
        CDockPanel();
        virtual ~CDockPanel();

        HRESULT Initialize();

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF Size );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnLastChildFillChanged );

        HRESULT OnLastChildFillChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        HRESULT GetEffectiveLastChildFill( BOOL* pLastChildFill );

        CTypedLayeredValue< CBoolValue > m_LastChildFill;
};

template< >
struct ObjectTypeTraits< CDockPanel >
{
    static const TypeIndex::Value Type = TypeIndex::DockPanel;
};