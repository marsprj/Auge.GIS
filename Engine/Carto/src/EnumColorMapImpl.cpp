#include "EnumColorMapImpl.h"

namespace auge
{
	EnumColorMapImpl::EnumColorMapImpl()
	{
		m_iter = m_maps.begin();
	}

	EnumColorMapImpl::~EnumColorMapImpl()
	{
		Cleanup();
	}

	void EnumColorMapImpl::Reset()
	{
		m_iter = m_maps.begin();
	}

	ColorMap* EnumColorMapImpl::Next()
	{
		if(m_iter==m_maps.end())
		{
			return NULL;
		}
		return *(m_iter++);
	}

	void EnumColorMapImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	void EnumColorMapImpl::Cleanup()
	{
		ColorMap* pColorMap = NULL;
		std::vector<ColorMap*>::iterator iter;
		for(iter=m_maps.begin(); iter!=m_maps.end(); iter++)
		{
			pColorMap = *iter;
			pColorMap->Release();
		}
		m_maps.clear();
	}

	void EnumColorMapImpl::Add(ColorMap* pColorMap)
	{
		if(pColorMap!=NULL)
		{
			m_maps.push_back(pColorMap);
		}
	}
}