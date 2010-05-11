#include "TypeConverter.h"

CConversionContext::CConversionContext(CPropertyObject* pTargetObject, CProperty* pTargetProperty, CProviders* pProviders) : m_TargetObject(pTargetObject),
                                                                                                                             m_TargetProperty(pTargetProperty),
                                                                                                                             m_Providers(pProviders)
{
}

CPropertyObject* CConversionContext::GetTargetObject()
{
    return m_TargetObject;
}

CProperty* CConversionContext::GetTargetProperty()
{
    return m_TargetProperty;
}

CProviders* CConversionContext::GetProviders()
{
    return m_Providers;
}

TypeIndex::Value CConversionContext::GetTargetType()
{
    return m_TargetProperty->GetType();
}