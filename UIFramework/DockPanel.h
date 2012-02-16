#include "Panel.h"

class UIFRAMEWORK_API CDockPanel : public CPanel
{
    public:
        DECLARE_FACTORY1( CDockPanel, CProviders* );

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

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        __override virtual __checkReturn HRESULT MeasureInternal( 
            const SizeF& AvailableSize,
            SizeF& DesiredSize
            );

        __override virtual __checkReturn HRESULT ArrangeInternal(
            const SizeF& AvailableSize,
            SizeF& UsedSize 
            );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnLastChildFillChanged );

        HRESULT OnLastChildFillChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        HRESULT GetEffectiveLastChildFill( bool* pLastChildFill );

        CTypedLayeredValue< CDockPanel, CBoolValue > m_LastChildFill;
};

template< >
struct ObjectTypeTraits< CDockPanel >
{
    static const TypeIndex::Value Type = TypeIndex::DockPanel;
};