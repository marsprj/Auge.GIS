#include "EnumStyleImpl.h"

namespace auge
{
	EnumStyleImpl::EnumStyleImpl() 
	{
		m_iter = m_styles.begin();
	}
	
	EnumStyleImpl::~EnumStyleImpl()
	{
		Cleanup();
	}

	void EnumStyleImpl::Reset()
	{
		m_iter = m_styles.begin();
	}

	Style* EnumStyleImpl::Next()
	{
		if(m_iter==m_styles.end())
		{
			return NULL;
		}
		return *(m_iter++);
	}

	void EnumStyleImpl::Release()
	{
		delete this;
	}

	void EnumStyleImpl::Add(Style* pStyle)
	{
		m_styles.push_back(pStyle);
	}

	void EnumStyleImpl::Cleanup()
	{
		std::vector<Style*>::iterator iter;
		for(iter=m_styles.begin(); iter!=m_styles.end(); iter++)
		{
			(*iter)->Release();
		}
		m_styles.clear();
	}
}