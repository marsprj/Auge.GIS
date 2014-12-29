#include "UnaryLogicFilterImpl.h"

namespace auge
{

	UnaryLogicFilterImpl::UnaryLogicFilterImpl():
	m_pFilter(NULL)
	{

	}

	UnaryLogicFilterImpl::~UnaryLogicFilterImpl()
	{
		if(m_pFilter!=NULL)
		{
			m_pFilter->Release();
			m_pFilter = NULL;
		}
	}


	augeFilterType UnaryLogicFilterImpl::GetType()
	{
		return augeFilterLogic;
	}

	augeLogicalOperator	UnaryLogicFilterImpl::GetOperator()
	{
		return augeLogOprOr;
	}

	GFilter* UnaryLogicFilterImpl::GetFilter()
	{
		return m_pFilter;
	}

	void UnaryLogicFilterImpl::SetFilter(GFilter* pFilter)
	{
		if(m_pFilter!=NULL)
		{
			m_pFilter->Release();
			m_pFilter = NULL;
		}
		m_pFilter = pFilter;
	}

	void UnaryLogicFilterImpl::Release()
	{
		if(ReleaseRef()<=0)
		{
			delete this;
		}
	}

}
