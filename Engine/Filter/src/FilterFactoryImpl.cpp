#include "FilterFactoryImpl.h"
#include "LiteralImpl.h"
#include "BBoxFilterImpl.h"
#include "IsBetweenFilterImpl.h"
#include "IDFilterImpl.h"
#include "BinaryComparisonFilterImpl.h"
#include "BinaryLogicFilterImpl.h"
#include "PropertyNameImpl.h"
#include "LiteralImpl.h"
#include "QueryImpl.h"

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

	Literal* FilterFactoryImpl::CreateLiteral()
	{
		return (new LiteralImpl());
	}

	Literal* FilterFactoryImpl::CreateLiteral(GValue* pValue)
	{
		if(pValue==NULL)
		{
			return NULL;
		}
		LiteralImpl *pLiteral = new LiteralImpl();
		pLiteral->SetValue(pValue);
		return pLiteral;
	}

	PropertyName* FilterFactoryImpl::CreatePropertyName(const char* szName)
	{
		if(szName==NULL)
		{
			return NULL;
		}

		PropertyNameImpl *pPropertyName = new PropertyNameImpl();
		pPropertyName->SetName(szName);
		return pPropertyName;
	}

	BBoxFilter* FilterFactoryImpl::CreateBBoxFilter()
	{
		return (new BBoxFilterImpl());
	}

	BBoxFilter* FilterFactoryImpl::CreateBBoxFilter(Expression* pPropertyName, GEnvelope& bbox)
	{
		BBoxFilterImpl* pBBoxFilter = NULL;
		pBBoxFilter = new BBoxFilterImpl();
		pBBoxFilter->SetPropertyName(pPropertyName);
		pBBoxFilter->SetExtent(bbox);

		return pBBoxFilter;
	}

	IDFilter* FilterFactoryImpl::CreateIDFilter()
	{
		return (new IDFilterImpl());
	}

	IsBetweenFilter* FilterFactoryImpl::CreateIsBetweenFilter()
	{
		return (new IsBetweenFilterImpl());
	}

	IsBetweenFilter* FilterFactoryImpl::CreateIsBetweenFilter(Expression* pPropertyName,Expression* pLowerBound,Expression* pUpperBound)
	{
		if(pLowerBound==NULL||pLowerBound==NULL||pUpperBound==NULL)
		{
			return NULL;
		}

		IsBetweenFilterImpl* pIsBetween = new IsBetweenFilterImpl();
		pIsBetween->SetExpression(pPropertyName);
		pIsBetween->SetLowerBound(pLowerBound);
		pIsBetween->SetUpperBound(pUpperBound);

		return pIsBetween;
	}

	BinaryComparisonFilter* FilterFactoryImpl::CreateBinaryComparisonFilter()
	{
		return (new BinaryComparisonFilterImpl());
	}

	BinaryLogicFilter*	FilterFactoryImpl::CreateBinaryLogicFilter()
	{
		return (new BinaryLogicFilterImpl());
	}

	GQuery*	FilterFactoryImpl::CreateQuery()
	{
		return (new GQueryImpl());
	}
}