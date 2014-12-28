#include "IDFilterImpl.h"

namespace auge
{

	IDFilterImpl::IDFilterImpl():
	m_operator(augeComOprEqual)
	{

	}

	IDFilterImpl::~IDFilterImpl()
	{

	}

	//////////////////////////////////////////////////////////////////////////

	augeFilterType IDFilterImpl::GetType()
	{
		return augeFilterFID;
	}

	void IDFilterImpl::Add(g_uint id)
	{
		m_ids.push_back(id);
	}

	g_uint IDFilterImpl::Count()
	{
		return m_ids.size();
	}

	g_uint IDFilterImpl::GetID(g_uint i)
	{
		return m_ids[i];
	}

	void IDFilterImpl::Clear()
	{
		m_ids.clear();
	}

	augeComparisonOperator IDFilterImpl::GetOperator()
	{
		return m_operator;
	}

	void IDFilterImpl::SetOperator(augeComparisonOperator oper)
	{
		m_operator = oper;
	}

	void IDFilterImpl::Release()
	{
		if(ReleaseRef()<=0)
		{
			delete this;
		}
	}

}

