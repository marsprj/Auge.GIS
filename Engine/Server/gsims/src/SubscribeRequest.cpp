#include "SubscribeRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	SubscribeRequest::SubscribeRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	SubscribeRequest::~SubscribeRequest()
	{

	}

	bool SubscribeRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetTheme(cgi["theme"]);
		SetKeyWords(cgi["keywords"]);
		return true;
	}

	const char*	SubscribeRequest::GetEngine()
	{
		return "ims";
	}

	const char*	SubscribeRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	SubscribeRequest::GetRequest()
	{
		return "Subscribe";
	}

	const char* SubscribeRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void SubscribeRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	SubscribeRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool SubscribeRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void SubscribeRequest::SetTheme(const char* theme)
	{
		if(theme==NULL)
		{
			m_theme.clear();
		}
		else
		{
			m_theme = theme;
		}
	}

	const char*	SubscribeRequest::GetTheme()
	{
		return m_theme.c_str();
	}

	void SubscribeRequest::SetKeyWords(const char* keywords)
	{
		if(keywords==NULL)
		{
			return;
		}
		if(strlen(keywords)==0)
		{
			return;
		}

		const char* key = NULL;
		WebContext* pWebContext = augeGetWebContextInstance();
		m_keywords.clear();

		char* str = strdup(keywords);
		char* ptr = strtok(str,",");
		while(ptr!=NULL)
		{
			key = pWebContext->ParameterEncoding(ptr);
			if(key!=NULL)
			{
				m_keywords.push_back(std::string(key));
			}

			ptr = strtok(NULL,",");
		}

		free(str);
	}

	g_uint	SubscribeRequest::GetKeywordCount()
	{
		return m_keywords.size();
	}

	const char* SubscribeRequest::GetKeyword(g_uint index)
	{
		if(index>=m_keywords.size())
		{
			return NULL;
		}
		return m_keywords[index].c_str();
	}

	void SubscribeRequest::Info()
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