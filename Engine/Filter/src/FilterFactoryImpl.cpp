#include "FilterFactoryImpl.h"

namespace auge
{
	FilterFactory*	augeGetFilterFactoryInstance()
	{
		static FilterFactoryImpl g_filterFactory;
		return &g_filterFactory;
	}

	FilterFactoryImpl::FilterFactoryImpl()
	{

	}

	FilterFactoryImpl::~FilterFactoryImpl()
	{

	}

	GLiteral* FilterFactoryImpl::CreateLiteral()
	{
		return NULL;
	}

	GLiteral* FilterFactoryImpl::CreateLiteral(GValue* pValue)
	{
		return NULL;
	}

	GPropertyName* FilterFactoryImpl::CreatePropertyName(const char* szName)
	{
		return NULL;
	}

	GBBoxFilter* FilterFactoryImpl::CreateBBoxFilter()
	{
		return NULL;
	}

	GBBoxFilter* FilterFactoryImpl::CreateBBoxFilter(GExpression* pPropertyName, GEnvelope& bbox)
	{
		return NULL;
	}

	GIDFilter* FilterFactoryImpl::CreateIDFilter()
	{
		return NULL;
	}

	GIsBetweenFilter* FilterFactoryImpl::CreateIsBetweenFilter()
	{
		return NULL;
	}

	GIsBetweenFilter* FilterFactoryImpl::CreateIsBetweenFilter(GExpression* pPropertyName,GExpression* pLowerBound,GExpression* pUpperBound)
	{
		return NULL;
	}

	GBinaryComparisonFilter* FilterFactoryImpl::CreateBinaryComparisonFilter()
	{
		return NULL;
	}

	GBinaryLogicFilter*	FilterFactoryImpl::CreateBinaryLogicFilter()
	{
		return NULL;
	}

	GQuery*	FilterFactoryImpl::CreateQuery()
	{
		return NULL;
	}
}