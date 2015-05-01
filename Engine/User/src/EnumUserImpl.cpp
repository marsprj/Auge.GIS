#include "EnumUserImpl.h"

namespace auge
{
	EnumUserImpl::EnumUserImpl()
	{

	}

	EnumUserImpl::~EnumUserImpl()
	{
		Cleanup();
	}

	void EnumUserImpl::Reset()
	{
		m_iter = m_Users.begin();
	}

	User* EnumUserImpl::Next()
	{
		if(m_iter==m_Users.end())
		{
			return NULL;
		}
		return *(m_iter++);
	}

	void EnumUserImpl::Release()
	{
		delete this;
	}

	void EnumUserImpl::Add(User* pUser)
	{
		if(pUser)
		{
			m_Users.push_back(pUser);
		}
	}

	void EnumUserImpl::Cleanup()
	{
		std::vector<User*>::iterator iter;
		for(iter=m_Users.begin(); iter!=m_Users.end(); iter++)
		{
			(*iter)->Release();
		}
		m_Users.clear();
	}
}