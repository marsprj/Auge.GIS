#include "GetSubscriptionRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	GetSubscriptionRequest::GetSubscriptionRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	GetSubscriptionRequest::~GetSubscriptionRequest()
	{

	}

	bool GetSubscriptionRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetTheme(cgi["theme"]);
		return true;
	}

	const char*	GetSubscriptionRequest::GetEngine()
	{
		return "ims";
	}

	const char*	GetSubscriptionRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetSubscriptionRequest::GetRequest()
	{
		return "GetSubscription";
	}

	const char* GetSubscriptionRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void GetSubscriptionRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetSubscriptionRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool GetSubscriptionRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void GetSubscriptionRequest::SetTheme(const char* theme)
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

	const char*	GetSubscriptionRequest::GetTheme()
	{
		return m_theme.empty() ? NULL : m_theme.c_str();
	}

	void GetSubscriptionRequest::Info()
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