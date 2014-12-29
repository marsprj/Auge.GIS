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
		virtual	Literal*				CreateLiteral();
		virtual	Literal*				CreateLiteral(GValue* pValue);
		virtual PropertyName*			CreatePropertyName(const char* szName);

		//---------------------------------------------------------------
		// Filter
		virtual	BBoxFilter*				CreateBBoxFilter();
		virtual	BBoxFilter*				CreateBBoxFilter(Expression* pPropertyName, GEnvelope& bbox);

		virtual IDFilter*				CreateIDFilter();

		virtual IsBetweenFilter*		CreateIsBetweenFilter();
		virtual IsBetweenFilter*		CreateIsBetweenFilter(Expression* pPropertyName,Expression* pLowerBound,Expression* pUpperBound);

		virtual BinaryComparisonFilter*	CreateBinaryComparisonFilter();
		virtual BinaryLogicFilter*		CreateBinaryLogicFilter();
		virtual UnaryLogicFilter*		CreateUnaryLogicFilter();

		virtual GQuery*					CreateQuery();

		virtual const char*				AsString(augeLogicalOperator oper);
		virtual const char*				AsString(augeComparisonOperator oper);
		virtual const char*				AsString(augeArithmeticOperator oper);

		virtual FilterReader*			CreateFilerReader(GFields* pFields);
	};

}

#endif //__AUGE_FILTER_FACTORY_IMPL_H__
