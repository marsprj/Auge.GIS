#include "BinaryLogicFilterImpl.h"

namespace auge
{

	BinaryLogicFilterImpl::BinaryLogicFilterImpl():
	m_operator(augeLogOprAnd)
	{

	}

	BinaryLogicFilterImpl::~BinaryLogicFilterImpl()
	{

	}

	augeFilterType BinaryLogicFilterImpl::GetType()
	{
		return augeFilterLogic;
	}

	bool BinaryLogicFilterImpl::AddFilter(GFilter* pFilter)
	{
		return m_filters.AddFilter(pFilter);
	}

	EnumFilter* BinaryLogicFilterImpl::GetFilters()
	{
		return &m_filters;
	}

	void BinaryLogicFilterImpl::SetOperator(augeLogicalOperator oper)
	{
		m_operator = oper;
	}

	augeLogicalOperator	BinaryLogicFilterImpl::GetOperator()
	{
		return m_operator;
	}

	void BinaryLogicFilterImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}	

}
