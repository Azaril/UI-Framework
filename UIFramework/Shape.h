#pragma once

#include "FrameworkElement.h"
#include "Brush.h"

//class UIFRAMEWORK_API CShape : public CFrameworkElement
//{
//    public:
//        DECLARE_TYPE_WITH_BASE( TypeIndex::Shape, CFrameworkElement );
//
//        static __checkReturn HRESULT CreatePropertyInformation(
//            __deref_out CPropertyInformation** ppInformation 
//            );
//
//        __override virtual __checkReturn HRESULT OnAttach( 
//            CUIAttachContext& Context 
//            );
//
//        __override virtual __checkReturn HRESULT OnDetach(
//            CUIDetachContext& Context
//            );
//
//    protected:
//        CShape(
//            );
//
//        virtual ~CShape(
//            );
//
//        __checkReturn HRESULT Initialize(
//            __in CProviders* pProviders 
//            );
//
//        __override virtual __checkReturn HRESULT MeasureInternal( 
//            const SizeF& AvailableSize,
//            SizeF& DesiredSize
//            );
//
//        __override virtual __checkReturn HRESULT ArrangeInternal(
//            const SizeF& AvailableSize,
//            SizeF& UsedSize 
//            );
//};
//
