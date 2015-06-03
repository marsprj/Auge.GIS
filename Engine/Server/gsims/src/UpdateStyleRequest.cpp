#include "UpdateStyleRequest.h"

namespace auge
{
	UpdateStyleRequest::UpdateStyleRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_style = NULL;
	}

	UpdateStyleRequest::~UpdateStyleRequest()
	{
		if(m_style)
		{
			free(m_style);
			m_style = NULL;
		}
	}

	bool UpdateStyleRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		SetStyle(cgi["type"]);
		SetStyle(cgi["style"]);
		return true;
	}

	const char*	UpdateStyleRequest::GetEngine()
	{
		return "ims";
	}

	const char*	UpdateStyleRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	UpdateStyleRequest::GetRequest()
	{
		return "UpdateStyle";
	}

	void UpdateStyleRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	UpdateStyleRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool UpdateStyleRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void UpdateStyleRequest::SetName(const char* name)
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

	const char*	UpdateStyleRequest::GetName()
	{
		return m_name.c_str();
	}

	void UpdateStyleRequest::SetStyle(const char* style)
	{
		if(style==NULL)
		{
			if(m_style!=NULL)
			{
				free(m_style);
				m_style = NULL;
			}
		}
		else
		{
			if(m_style!=NULL)
			{
				free(m_style);
				m_style = NULL;
			}
			size_t len = strlen(style);
			m_style = (char*)malloc(len+1);
			memset(m_style, 0, len+1);
			memcpy(m_style, style, len);

			//size_t len = strlen(style);
			//size_t len_gbk = len << 1;
			//m_style = (char*)malloc(len_gbk);
			//memset(m_style, 0, len_gbk);
			//g_int ret = auge_encoding_convert_2("UTF-8", "GBK", style, len, m_style, &len_gbk);
			//GLogger *pLogger = augeGetLoggerInstance();
			//pLogger->Debug(m_style, __FILE__,__LINE__);
			//if(ret<0)
			//{
			//	strcpy(m_style, style);
			//	//free(m_style);
			//	//m_style = NULL;
			//}
		}
		//if(style==NULL)
		//{
		//	m_style.c_str();
		//}
		//else
		//{
		//	m_style = style;
		//}
	}
	
	const char*	UpdateStyleRequest::GetStyle()
	{
		//return m_style.c_str();
		return m_style;
	}

	void UpdateStyleRequest::SetType(const char* type)
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

	const char*	UpdateStyleRequest::GetType()
	{
		return m_type.c_str();
	}
}