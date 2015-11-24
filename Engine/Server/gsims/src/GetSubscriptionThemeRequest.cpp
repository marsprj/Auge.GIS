#include "GetSubscriptionThemeRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	GetSubscriptionThemeRequest::GetSubscriptionThemeRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	GetSubscriptionThemeRequest::~GetSubscriptionThemeRequest()
	{

	}

	bool GetSubscriptionThemeRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetTheme(cgi["theme"]);
		return true;
	}

	const char*	GetSubscriptionThemeRequest::GetEngine()
	{
		return "ims";
	}

	const char*	GetSubscriptionThemeRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetSubscriptionThemeRequest::GetRequest()
	{
		return "GetSubscriptionTheme";
	}

	const char* GetSubscriptionThemeRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void GetSubscriptionThemeRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetSubscriptionThemeRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool GetSubscriptionThemeRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void GetSubscriptionThemeRequest::SetTheme(const char* theme)
	{
		if(theme==NULL)
		{
			m_theme.clear();
		}
		else
		{
			WebContext* pWebContext = augeGetWebContextInstance();
			const char* theme_encoded = pWebContext->ParameterEncoding(theme);
			if(theme_encoded!=NULL)
			{
				m_theme = theme_encoded;
			}
			else
			{
				m_theme.clear();
			}
		}
	}

	const char*	GetSubscriptionThemeRequest::GetTheme()
	{
		return m_theme.empty() ? NULL : m_theme.c_str();
	}

	void GetSubscriptionThemeRequest::Info()
	{
		GLogger* pLogger = augeGetLoggerInstance();
		char str[AUGE_MSG_MAX];
		const char* value = GetRequest();
		pLogger->Debug("[Requet Parameters]");
		g_sprintf(str,"\t%s:%s", "Request", (value==NULL)?"":value);
		pLogger->Debug(str);
		//value = GetUser();
		//g_sprintf(str,"\t%s:%s", "User", (value==NULL)?"":value);
		//pLogger->Debug(str);
		g_sprintf(str,"\t%s:%s", "Service", "ims");
		pLogger->Debug(str);
		value = GetVersion();
		g_sprintf(str,"\t%s:%s", "Version", (value==NULL)?"":value);
		pLogger->Debug(str);

	}
}