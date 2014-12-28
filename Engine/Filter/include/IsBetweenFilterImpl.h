#ifndef __AUGE_FILTER_IS_BETWEEN_IMPL_H__
#define __AUGE_FILTER_IS_BETWEEN_IMPL_H__

#include "AugeFilter.h"
#include <string>

namespace auge
{
	class IsBetweenFilterImpl : public IsBetweenFilter
	{
	public:
		IsBetweenFilterImpl();
		virtual ~IsBetweenFilterImpl();

	public:	
		virtual		augeFilterType		GetType();
		virtual		augeComparisonOperator	GetOperator();

		virtual		Expression*			GetExpression();
		virtual		bool				SetExpression(Expression* pExpression);

		virtual		Expression*			GetLowerBound();
		virtual		bool				SetLowerBound(Expression* pLowerBound);

		virtual		Expression*			GetUpperBound();
		virtual		bool				SetUpperBound(Expression* pUpperBound);
		virtual		void				Release();

	private:
		Expression* m_pExpression;
		Expression* m_pLowerBound;
		Expression* m_pUpperBound;
	};

}

#endif	//__AUGE_FILTER_IS_BETWEEN_IMPL_H__
