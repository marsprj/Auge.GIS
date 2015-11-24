#ifndef __AUGE_ENUM_SUBSCRIPTION_THEME_IMPL_H___
#define __AUGE_ENUM_SUBSCRIPTION_THEME_IMPL_H___

#include "AugeWebCore.h"
#include <vector>

namespace auge
{
	class EnumSubscriptionThemeImpl : public EnumSubscriptionTheme
	{
	public:
		EnumSubscriptionThemeImpl();
		virtual ~EnumSubscriptionThemeImpl();
	public:
		virtual bool				IsEOF();
		virtual void				Reset();
		virtual SubscriptionTheme*	Next();

	public:
		void	AddSubscriptionTheme(SubscriptionTheme* theme);
		void	Cleanup();
		void	SetUser(g_int user);

	private:
		g_int	m_user;
		std::vector<SubscriptionTheme*>	m_themes;
		std::vector<SubscriptionTheme*>::iterator m_iter;
	};
}

#endif //__AUGE_ENUM_SUBSCRIPTION_THEME_IMPL_H___
