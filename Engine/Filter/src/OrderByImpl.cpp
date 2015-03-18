#include "OrderByImpl.h"

namespace auge
{
	OrderByImpl::OrderByImpl():
m_order(augeOrderAsc)
{

}

OrderByImpl::~OrderByImpl()
{

}

RESULTCODE OrderByImpl::AddField(const char* szFieldName)
{
	if(szFieldName==NULL)
	{
		return AG_FAILURE;
	}
	m_fields.push_back(szFieldName);
	return AG_SUCCESS;
}

const char*	OrderByImpl::GetField(g_uint index)
{
	if(index<0||index>m_fields.size())
	{
		return NULL;
	}
	return m_fields[index].c_str();
}

g_uint OrderByImpl::GetFieldCount()
{
	return m_fields.size();
}

void OrderByImpl::Clear()
{
	m_fields.clear();
}

void OrderByImpl::SetOrder(augeOrderType order)
{
	m_order = order;
}


bool OrderByImpl::IsAsc()
{
	return (m_order==augeOrderAsc);
}

void OrderByImpl::Release()
{
	if(ReleaseRef()<=0)
	{
		delete this;
	}
}

}
