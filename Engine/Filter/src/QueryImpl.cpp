#include "QueryImpl.h"

#include "string.h"

namespace auge
{
	GQueryImpl::GQueryImpl():
	m_pOrderBy(NULL),
	m_pFilter(NULL),
	m_maxFeatures(-1),
	m_offset(0)
	{

	}

	GQueryImpl::~GQueryImpl()
	{
		if(m_pOrderBy!=NULL)
		{
			m_pOrderBy->Release();
			m_pOrderBy = NULL;
		}
		if(m_pFilter!=NULL)
		{
			m_pFilter->Release();
			m_pFilter = NULL;
		}
	}

	const char* GQueryImpl::GetFeatureClassName() const
	{
		return m_strClassName.c_str();
	}

	RESULTCODE GQueryImpl::SetFeatureClassName(const char* szClassName)
	{
		if(szClassName==NULL)
		{
			return AG_FAILURE;
		}
		m_strClassName = szClassName;
		return AG_SUCCESS;
	}

	int GQueryImpl::GetSubFieldCount() const
	{
		return m_subfields.size();
	}

	const char* GQueryImpl::GetSubField(const int index) const
	{
		if(index<0||index>=m_subfields.size())
		{
			return NULL;
		}
		return m_subfields[index].c_str();
	}

	RESULTCODE GQueryImpl::AddSubField(const char* szName)
	{
		if(szName==NULL)
		{
			return AG_FAILURE;
		}
		m_subfields.push_back(szName);
		return AG_SUCCESS;
	}

	RESULTCODE GQueryImpl::RemoveSubField(const char* szName)
	{
		int index = FindSubField(szName);
		if(index<0)
		{
			return AG_FAILURE;
		}
		m_subfields.erase(m_subfields.begin()+index);
		return AG_SUCCESS;
	}

	bool GQueryImpl::HasSubField(const char* szName)
	{
		return (FindSubField(szName)>=0);
	}

	bool GQueryImpl::SetFilter(GFilter* pFilter)
	{
		if(m_pFilter!=NULL)
		{
			m_pFilter->Release();
			m_pFilter = NULL;
		}
		m_pFilter = pFilter;
		return true;
	}

	const GFilter* GQueryImpl::GetFilter() const
	{
		return m_pFilter;
	}

	RESULTCODE GQueryImpl::SetOrderBy(OrderBy* pOrderBy)
	{
		if(m_pOrderBy!=NULL)
		{
			m_pOrderBy->Release();
		}
		m_pOrderBy = pOrderBy;
		return AG_SUCCESS;
	}

	OrderBy* GQueryImpl::GetOrderBy()
	{
		return m_pOrderBy;
	}

	bool GQueryImpl::SetSQL(const char* szsql)
	{
		return true;
	}

	const char* GQueryImpl::GetSQL() const
	{
		return NULL;
	}

	void GQueryImpl::Release()
	{
		delete this;
	}

	int	GQueryImpl::FindSubField(const char* szFieldName)
	{
		if(szFieldName==NULL)
		{
			return -1;
		}
		int index = 0;
		SUBFIELD_VECTOR::iterator iter;
		const char* szstr;
		for(iter=m_subfields.begin(); iter!=m_subfields.end(); iter++, index++)
		{
			szstr = (*iter).c_str();
			if(strcmp(szstr, szFieldName)==0)
			{
				return index;
			}
		}
		return -1;
	}

	bool GQueryImpl::IsStarFields()
	{
		if(m_subfields.size()!=1)
		{
			return false;
		}

		std::string& strFieldName = m_subfields[0];

		if(g_stricmp(strFieldName.c_str(), "*")!=0)
		{
			return false;
		}

		return true;
	}

	void GQueryImpl::SetMaxFeatures(g_int maxFeatures)
	{
		m_maxFeatures = maxFeatures;
	}

	g_int GQueryImpl::GetMaxFeatures()
	{
		return m_maxFeatures;
	}

	void GQueryImpl::SetOffset(g_int offset)
	{
		m_offset = offset;
	}

	g_int GQueryImpl::GetOffset()
	{
		return m_offset;
	}

}
