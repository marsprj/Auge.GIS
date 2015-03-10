#include "RegisterHandlerRequest.h"

namespace auge
{
	RegisterHandlerRequest::RegisterHandlerRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
	}

	RegisterHandlerRequest::~RegisterHandlerRequest()
	{

	}

	bool RegisterHandlerRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		return true;
	}

	const char*	RegisterHandlerRequest::GetEngine()
	{
		return "ims";
	}

	const char*	RegisterHandlerRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	RegisterHandlerRequest::GetRequest()
	{
		return "RegisterHandler";
	}

	void RegisterHandlerRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	RegisterHandlerRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool RegisterHandlerRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void RegisterHandlerRequest::SetName(const char* name)
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

	const char*	RegisterHandlerRequest::GetName()
	{
		return m_name.c_str();
	}

	void RegisterHandlerRequest::SetHandler(const char* handler)
	{
		if(handler==NULL)
		{
			m_handler.clear();
		}
		else if(strlen(handler)==0)
		{
			m_handler.clear();
		}
		else
		{
			m_handler = handler;
		}
	}

	const char*	RegisterHandlerRequest::GetHandler()
	{
		return m_handler.c_str();
	}

}