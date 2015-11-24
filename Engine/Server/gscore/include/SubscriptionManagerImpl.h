#ifndef __AUGE_SUBSCRIPTION_MANAGER_H__
#define __AUGE_SUBSCRIPTION_MANAGER_H__

#include "AugeType.h"
#include "AugeWebCore.h"

namespace auge
{
	class GConnection;
	
	class SubscriptionManagerImpl : public SubscriptionManager
	{
		friend class SubscriptionThemeImpl;

	public:
		SubscriptionManagerImpl();
		virtual ~SubscriptionManagerImpl();
	public:
		virtual bool		IsSubscribed(g_uint user, const char* theme, const char* keyword);
		virtual RESULTCODE	Subscribe(g_uint user, const char* theme, const char* keyword);
		virtual EnumSubscriptionTheme*	GetThemes(g_uint user);
		virtual SubscriptionTheme*		GetTheme(g_uint user, const char* theme);

		virtual RESULTCODE	Initialize(GConnection* pConnection);
		virtual RESULTCODE	Unload();

	private:
		GConnection	*m_pConnection;
	};
}

#endif //__AUGE_SUBSCRIPTION_MANAGER_H__
