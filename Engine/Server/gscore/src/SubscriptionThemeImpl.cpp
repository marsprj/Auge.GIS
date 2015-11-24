#include "SubscriptionThemeImpl.h"
#include "SubscriptionManagerImpl.h"
#include "AugeData.h"

namespace auge
{
	SubscriptionThemeImpl::SubscriptionThemeImpl()
	{

	}

	SubscriptionThemeImpl::~SubscriptionThemeImpl()
	{

	}

	const char* SubscriptionThemeImpl::GetTheme()
	{
		return m_theme.c_str();
	}

	const char* SubscriptionThemeImpl::GetKeyword(g_uint index)
	{
		if(m_keywords.empty())
		{
			GetKeywords();
		}

		if(index>=m_keywords.size())
		{
			return NULL;
		}
		return m_keywords[index].c_str();
	}

	g_uint SubscriptionThemeImpl::GetKeywordCount()
	{
		if(m_keywords.empty())
		{
			GetKeywords();
		}
		return m_keywords.size();
	}

	void SubscriptionThemeImpl::Release()
	{
		delete this;
	}

	void SubscriptionThemeImpl::SetTheme(const char* theme)
	{
		if(theme==NULL)
		{
			m_theme.c_str();
		}
		else
		{
			m_theme = theme;
		}
	}

	void SubscriptionThemeImpl::SetUser(g_int user)
	{
		m_user = user;
	}

	void SubscriptionThemeImpl::GetKeywords()
	{
		SubscriptionManagerImpl* pManager = static_cast<SubscriptionManagerImpl*>(augeGetSubscriptionManagerInstance());
		GConnection* pConnection = pManager->m_pConnection;
		
		const char* format = "select gid,keyword from g_subscribe where user_id=%d and theme='%s'";
		char sql[AUGE_SQL_MAX];
		g_sprintf(sql, format, m_user, m_theme.c_str());

		m_keywords.clear();

		GResultSet* pResult = pConnection->ExecuteQuery(sql);
		if(pResult!=NULL)
		{
			g_uint count = pResult->GetCount();
			for(g_uint i=0; i<count; i++)
			{
				const char* keyword = pResult->GetString(i, 1);
				m_keywords.push_back(auge_encoding_convert(AUGE_ENCODING_GBK, AUGE_ENCODING_UTF8, keyword, strlen(keyword)));
			}
			pResult->Release();
		}
	}
}