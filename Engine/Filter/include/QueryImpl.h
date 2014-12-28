#ifndef __AUGE_QUERY_IMPL_H__
#define __AUGE_QUERY_IMPL_H__

#include "AugeFilter.h"
#include <string>
#include <vector>

namespace auge
{
	class GQueryImpl : public GQuery
	{
	public:
		GQueryImpl();
		virtual ~GQueryImpl();
	public:
		//--------------------------------------------------------------------------
		virtual const char*       GetFeatureClassName() const;
		virtual       RESULTCODE  SetFeatureClassName(const char* szClassName);
		//--------------------------------------------------------------------------                                                                                           /**/
		virtual       int         GetSubFieldCount() const;
		virtual const char*       GetSubField(const int index) const;
		virtual       RESULTCODE  AddSubField(const char* szName);
		virtual       RESULTCODE  RemoveSubField(const char* szName);
		virtual       bool		  HasSubField(const char* szName);
		virtual		  bool		  IsStarFields();

		virtual       bool        SetFilter(GFilter* pFilter);     
		virtual const GFilter*	  GetFilter() const;

		virtual		  RESULTCODE  SetOrderBy(OrderBy* pOrderBy);
		virtual		  OrderBy*    GetOrderBy();

		virtual bool			  SetSQL(const char* szsql);
		virtual const char*       GetSQL() const;

		virtual       void        Release();

	public:
		int		FindSubField(const char* szFieldName);

	private:
		typedef std::vector<std::string>	SUBFIELD_VECTOR;

		std::string	m_strClassName;
		SUBFIELD_VECTOR m_subfields;
		OrderBy		*m_pOrderBy;
		GFilter		*m_pFilter;
	};
}

#endif //__AUGE_QUERY_IMPL_H__
