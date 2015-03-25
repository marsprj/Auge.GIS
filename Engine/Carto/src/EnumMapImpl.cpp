#include "EnumMapImpl.h"

namespace auge
{
	EnumMapImpl::EnumMapImpl()
	{
		m_iter = m_maps.begin();
	}

	EnumMapImpl::~EnumMapImpl()
	{
		Cleanup();
	}

	Map* EnumMapImpl::Next()
	{
		if(m_iter==m_maps.end())
		{
			return NULL;
		}
		return *(m_iter++);
	}

	void EnumMapImpl::Reset()
	{
		m_iter = m_maps.begin();
	}

	void EnumMapImpl::Release()
	{
		delete this;
	}

	void EnumMapImpl::Cleanup()
	{
		Map* pMap = NULL;
		std::vector<Map*>::iterator iter;
		for(iter=m_maps.begin(); iter!=m_maps.end(); iter++)
		{
			pMap = *iter;
			pMap->Release();
		}
		m_maps.clear();
	}

	void EnumMapImpl::Add(Map* pMap)
	{
		if(pMap!=NULL)
		{
			m_maps.push_back(pMap);
		}
	}
}
