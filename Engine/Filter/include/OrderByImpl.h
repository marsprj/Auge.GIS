#ifndef __AUGE_ORDER_BY_IMPL_H__
#define __AUGE_ORDER_BY_IMPL_H__

#include "AugeFilter.h"
#include <string>
#include <vector>

namespace auge
{
	class OrderByImpl : public OrderBy
	{
	public:
		OrderByImpl();
		virtual ~OrderByImpl();
	public:
		virtual	RESULTCODE			AddField(const char* szFieldName);
		virtual const char*			GetField(g_uint index);
		virtual	g_uint				GetFieldCount();
		virtual void				Clear();

		virtual void				SetOrder(augeOrderType order);
		virtual augeOrderType		GetOrder();

		virtual void				Release();

	private:
		typedef std::vector<std::string>	FIELD_VECTOR;

		FIELD_VECTOR	m_fields;
		augeOrderType	m_order;
	};

}

#endif //__AUGE_ORDER_BY_IMPL_H__
