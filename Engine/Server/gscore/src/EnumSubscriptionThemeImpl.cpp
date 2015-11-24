#include "EnumSubscriptionThemeImpl.h"

namespace auge
{
	EnumSubscriptionThemeImpl::EnumSubscriptionThemeImpl()
	{
		m_iter = m_themes.begin();
	}

	EnumSubscriptionThemeImpl::~EnumSubscriptionThemeImpl()
	{

	}

	bool EnumSubscriptionThemeImpl::IsEOF()
	{
		return (m_iter==m_themes.end());
	}

	void EnumSubscriptionThemeImpl::Reset()
	{
		m_iter = m_themes.begin();
	}

	SubscriptionTheme* EnumSubscriptionThemeImpl::Next()
	{
		if(m_iter==m_themes.end())
		{
			return NULL;
		}
		return *(m_iter++);
	}

	void EnumSubscriptionThemeImpl::AddSubscriptionTheme(SubscriptionTheme* theme)
	{
		if(theme!=NULL)
		{
			m_themes.push_back(theme);
		}
	}

	void EnumSubscriptionThemeImpl::Cleanup()
	{
		SubscriptionTheme* theme = NULL;
		std::vector<SubscriptionTheme*>::iterator iter;
		for(iter=m_themes.begin(); iter!=m_themes.end(); iter++)
		{
			theme = *iter;
			theme->Release();
		}
		m_themes.clear();
	}

	void EnumSubscriptionThemeImpl::SetUser(g_int user)
	{
		m_user = user;
	}
}
