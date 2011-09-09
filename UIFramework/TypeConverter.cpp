#include "TypeConverter.h"

CConversionContext::CConversionContext(
    __in CPropertyObject* pTargetObject, 
    __in CProperty* pTargetProperty, 
    __in CProviders* pProviders
    ) 
    : m_TargetObject(pTargetObject)
    , m_TargetProperty(pTargetProperty)
    , m_Providers(pProviders)
{
}

__out CPropertyObject*
CConversionContext::GetTargetObject(
    )
{
    return m_TargetObject;
}

__out CProperty*
CConversionContext::GetTargetProperty(
    )
{
    return m_TargetProperty;
}

__out CProviders* 
CConversionContext::GetProviders(
    )
{
    return m_Providers;
}

TypeIndex::Value 
CConversionContext::GetTargetType(
    )
{
    return m_TargetProperty->GetType();
}