#include "EnumFilterImpl.h"

namespace auge
{
	EnumFilterImpl::EnumFilterImpl()
	{
		m_iter = m_filters.begin();
	}

	EnumFilterImpl::~EnumFilterImpl()
	{
		Cleanup();
	}

	GFilter* EnumFilterImpl::Next()
	{
		if(m_iter==m_filters.end())
		{
			return NULL;
		}
		GFilter* pFilter = *m_iter;
		m_iter++;
		return pFilter;
	}

	void EnumFilterImpl::Reset()
	{
		m_iter = m_filters.begin();
	}

	void EnumFilterImpl::Release()
	{
		if(ReleaseRef()<=0)
		{
			delete this;
		}
	}

	void EnumFilterImpl::Cleanup()
	{
		GFilter *pFilter = NULL;
		FILTER_VECTOR::iterator iter;
		for(iter=m_filters.begin(); iter!=m_filters.end(); iter++)
		{
			pFilter = *iter;
			if(pFilter!=NULL)
			{
				pFilter->Release();
			}
		}
		m_filters.clear();
	}

	bool EnumFilterImpl::AddFilter(GFilter* pFilter)
	{
		if(pFilter==NULL)
		{
			return false;
		}
		m_filters.push_back(pFilter);
		return true;
	}

}
