#ifndef __AUGE_FILTER_FACTORY_IMPL_H__
#define __AUGE_FILTER_FACTORY_IMPL_H__

#include "AugeFilter.h"

namespace auge
{
	class FilterFactoryImpl : public FilterFactory
	{
	public:
		FilterFactoryImpl();
		virtual ~FilterFactoryImpl();
	public:
		//---------------------------------------------------------------
		// Expression
		virtual	GLiteral*				CreateLiteral();
		virtual	GLiteral*				CreateLiteral(GValue* pValue);
		virtual GPropertyName*			CreatePropertyName(const char* szName);

		//---------------------------------------------------------------
		// Filter
		virtual	GBBoxFilter*			CreateBBoxFilter();
		virtual	GBBoxFilter*			CreateBBoxFilter(GExpression* pPropertyName, GEnvelope& bbox);

		virtual GIDFilter*				CreateIDFilter();

		virtual GIsBetweenFilter*		CreateIsBetweenFilter();
		virtual GIsBetweenFilter*		CreateIsBetweenFilter(GExpression* pPropertyName,GExpression* pLowerBound,GExpression* pUpperBound);

		virtual GBinaryComparisonFilter*	CreateBinaryComparisonFilter();
		virtual GBinaryLogicFilter*			CreateBinaryLogicFilter();

		virtual GQuery*					CreateQuery();
	};

}

#endif //__AUGE_FILTER_FACTORY_IMPL_H__
