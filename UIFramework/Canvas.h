#pragma once

#include "Panel.h"

class UIFRAMEWORK_API CCanvas : public CPanel
{
    public:
        DECLARE_FACTORY1( CCanvas, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Canvas, CPanel );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

        static CStaticProperty LeftProperty;
        static CStaticProperty TopProperty;
        static CStaticProperty RightProperty;
        static CStaticProperty BottomProperty;

    public:
        CCanvas();
        virtual ~CCanvas();

        HRESULT Initialize( CProviders* pProviders );

        virtual HRESULT MeasureInternal( SizeF AvailableSize, SizeF& DesiredSize );
        virtual HRESULT ArrangeInternal( SizeF AvailableSize, SizeF& UsedSize );
    
        static __checkReturn HRESULT OnLeftChanged(
            __in CPropertyObject* pObject,
            __in_opt CObjectWithType* pOldValue,
            __in_opt CObjectWithType* pNewValue
            );

        static __checkReturn HRESULT OnRightChanged(
            __in CPropertyObject* pObject,
            __in_opt CObjectWithType* pOldValue,
            __in_opt CObjectWithType* pNewValue
            );
            
        static __checkReturn HRESULT OnTopChanged(
            __in CPropertyObject* pObject,
            __in_opt CObjectWithType* pOldValue,
            __in_opt CObjectWithType* pNewValue
            );
            
        static __checkReturn HRESULT OnBottomChanged(
            __in CPropertyObject* pObject,
            __in_opt CObjectWithType* pOldValue,
            __in_opt CObjectWithType* pNewValue
            );                                    
    
};

template< >
struct ObjectTypeTraits< CCanvas >
{
    static const TypeIndex::Value Type = TypeIndex::Canvas;
};