#ifndef __AUGE_SUBSCRIPTION_THEME_IMPL_H__
#define __AUGE_SUBSCRIPTION_THEME_IMPL_H__

#include <vector>
#include <string>
#include "AugeWebCore.h"

namespace auge
{
	class SubscriptionThemeImpl : public SubscriptionTheme
	{
	public:
		SubscriptionThemeImpl();
		virtual ~SubscriptionThemeImpl();
	public:
		virtual const char* GetTheme();
		virtual const char* GetKeyword(g_uint index);
		virtual g_uint		GetKeywordCount();
		virtual void		Release();

	public:
		void	SetTheme(const char* theme);
		void	SetUser(g_int user);
		void	GetKeywords();

	private:
		g_int			m_user;
		std::string		m_theme;
		std::vector<std::string> m_keywords;
	};
}

#endif //__AUGE_SUBSCRIPTION_THEME_IMPL_H__
