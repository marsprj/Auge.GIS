#include "SubscriptionManagerImpl.h"
#include "EnumSubscriptionThemeImpl.h"
#include "SubscriptionThemeImpl.h"
#include "AugeData.h"

namespace auge
{
	SubscriptionManager* augeGetSubscriptionManagerInstance()
	{
		static SubscriptionManagerImpl g_SubscriptionManagerImpl;
		return &g_SubscriptionManagerImpl;
	}

	SubscriptionManagerImpl::SubscriptionManagerImpl()
	{
		m_pConnection = NULL;
	}

	SubscriptionManagerImpl::~SubscriptionManagerImpl()
	{

	}

	bool SubscriptionManagerImpl::IsSubscribed(g_uint user, const char* theme, const char* keyword)
	{
		if(theme==NULL||keyword==NULL)
		{
			return false;
		}

		const char* format = "select count(*) from g_subscribe where user_id=%d and theme='%s' and keyword='%s'";
		char sql[AUGE_SQL_MAX];
		g_sprintf(sql, format, user, theme, keyword);

		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return false;
		}

		g_int count = pResult->GetInt(0,0);
		pResult->Release();

		return (count>0);
	}

	RESULTCODE SubscriptionManagerImpl::Subscribe(g_uint user, const char* theme, const char* keyword)
	{
		if(theme==NULL||keyword==NULL)
		{
			return false;
		}

		const char* format = "insert into g_subscribe (user_id,theme,keyword) values(%d,'%s','%s')";
		char sql[AUGE_SQL_MAX];
		g_sprintf(sql, format, user, theme, keyword);

		return m_pConnection->ExecuteSQL(sql);
	}

	EnumSubscriptionTheme* SubscriptionManagerImpl::GetThemes(g_uint user)
	{
		EnumSubscriptionThemeImpl* pEnumTheme = new EnumSubscriptionThemeImpl(); 

		const char* format = "select  distinct(theme) from g_subscribe where user_id=%d";
		char sql[AUGE_SQL_MAX];
		g_sprintf(sql, format, user);

		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult!=NULL)
		{
			const char* theme = NULL;
			SubscriptionThemeImpl* pTheme = NULL;

			g_uint count = pResult->GetCount();
			for(g_uint i=0; i<count; i++)
			{
				theme = pResult->GetString(i, 0);
				if(theme!=NULL)
				{
					pTheme = new SubscriptionThemeImpl();
					pTheme->SetUser(user);
					pTheme->SetTheme(theme);
					pEnumTheme->AddSubscriptionTheme(pTheme);
				}
			}

			pResult->Release();
		}

		return pEnumTheme;
	}

	SubscriptionTheme* SubscriptionManagerImpl::GetTheme(g_uint user, const char* theme)
	{
		if(theme==NULL)
		{
			return NULL;
		}

		SubscriptionThemeImpl* pTheme = new SubscriptionThemeImpl();
		pTheme->SetUser(user);
		pTheme->SetTheme(theme);
		return pTheme;
	}

	RESULTCODE SubscriptionManagerImpl::Initialize(GConnection* pConnection)
	{
		m_pConnection = pConnection;
		return AG_SUCCESS;
	}

	RESULTCODE SubscriptionManagerImpl::Unload()
	{
		return AG_SUCCESS;
	}
}