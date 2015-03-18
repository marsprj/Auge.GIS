#include "AddStyleRequest.h"

namespace auge
{
	AddStyleRequest::AddStyleRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
	}

	AddStyleRequest::~AddStyleRequest()
	{

	}

	bool AddStyleRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		SetType(cgi["type"]);
		SetStyle(cgi["style"]);
		return true;
	}

	const char*	AddStyleRequest::GetEngine()
	{
		return "ims";
	}

	const char*	AddStyleRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	AddStyleRequest::GetRequest()
	{
		return "AddStyle";
	}

	void AddStyleRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	AddStyleRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool AddStyleRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void AddStyleRequest::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		else if(strlen(name)==0)
		{
			m_name.clear();
		}
		else
		{
			m_name = name;
		}
		
	}

	const char*	AddStyleRequest::GetName()
	{
		return m_name.c_str();
	}

	void AddStyleRequest::SetStyle(const char* style)
	{
		if(style==NULL)
		{
			m_style.clear();
		}
		else
		{
			m_style = style;
		}
	}

	const char*	AddStyleRequest::GetStyle()
	{
		return m_style.c_str();
	}

	void AddStyleRequest::SetType(const char* type)
	{
		if(type==NULL)
		{
			m_type.clear();
		}
		else
		{
			m_type = type;
		}
	}

	const char*	AddStyleRequest::GetType()
	{
		return m_type.c_str();
	}

	void AddStyleRequest::Info()
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

		value = GetName();
		g_sprintf(str,"\t%s:%s", "Name", (value==NULL)?"":value);
		pLogger->Debug(str);

		value = GetType();
		g_sprintf(str,"\t%s:%s", "Type", (value==NULL)?"":value);
		pLogger->Debug(str);

		value = GetStyle();
		if(value==NULL)
		{
			g_sprintf(str,"\tStyle:");
			pLogger->Debug(str);
		}
		else
		{
			pLogger->Debug(value);
		}
		
	}
}