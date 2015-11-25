#include "UnsubscribeRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	UnsubscribeRequest::UnsubscribeRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	UnsubscribeRequest::~UnsubscribeRequest()
	{

	}

	bool UnsubscribeRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetTheme(cgi["theme"]);
		SetKeyWords(cgi["keywords"]);
		return true;
	}

	const char*	UnsubscribeRequest::GetEngine()
	{
		return "ims";
	}

	const char*	UnsubscribeRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	UnsubscribeRequest::GetRequest()
	{
		return "Unsubscribe";
	}

	const char* UnsubscribeRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void UnsubscribeRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	UnsubscribeRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool UnsubscribeRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void UnsubscribeRequest::SetTheme(const char* theme)
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

	const char*	UnsubscribeRequest::GetTheme()
	{
		return m_theme.c_str();
	}

	void UnsubscribeRequest::SetKeyWords(const char* keywords)
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

	g_uint	UnsubscribeRequest::GetKeywordCount()
	{
		return m_keywords.size();
	}

	const char* UnsubscribeRequest::GetKeyword(g_uint index)
	{
		if(index>=m_keywords.size())
		{
			return NULL;
		}
		return m_keywords[index].c_str();
	}

	void UnsubscribeRequest::Info()
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