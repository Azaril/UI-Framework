#include "TypeConverter.h"

CConversionContext::CConversionContext(CPropertyObject* pTargetObject, CProperty* pTargetProperty, CClassResolver* pClassResolver) : m_TargetObject(pTargetObject),
                                                                                                                                     m_TargetProperty(pTargetProperty),
                                                                                                                                     m_ClassResolver(pClassResolver)
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

CClassResolver* CConversionContext::GetClassResolver()
{
    return m_ClassResolver;
}

TypeIndex::Value CConversionContext::GetTargetType()
{
    return m_TargetProperty->GetType();
}