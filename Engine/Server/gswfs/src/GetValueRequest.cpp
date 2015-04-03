#include "GetValueRequest.h"

namespace auge
{

	GetValueRequest::GetValueRequest():
	m_version("1.0.0"),
	m_mime_type("text/xml"),
	m_order(augeOrderAsc),
	m_encoding("GBK")
	{

	}

	GetValueRequest::~GetValueRequest()
	{

	}

	const char*	GetValueRequest::GetEngine()
	{
		return "wfs";
	}

	const char*	GetValueRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetValueRequest::GetRequest()
	{
		return "GetValue";
	}

	const char*	GetValueRequest::GetServiceName()
	{
		return m_service_name.c_str();
	}

	void GetValueRequest::SetServiceName(const char* serviceName)
	{
		if(serviceName==NULL)
		{
			m_service_name.clear();
		}
		else
		{
			m_service_name = serviceName;
		}
	}

	void GetValueRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	void GetValueRequest::SetUser(const char* user)
	{
		if(user==NULL)
		{
			m_user.clear();
		}
		else
		{
			m_user = user;
		}
	}

	const char* GetValueRequest::GetUser()
	{
		return m_user.c_str();
	}

	const char*	GetValueRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool GetValueRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);
		SetServiceName(cgi["servicename"]);
		SetTypeName(cgi["typeName"]);
		SetField(cgi["field"]);
		SetOrder(cgi["order"]);
		return true;
	}

	void GetValueRequest::SetField(const char* field)
	{
		if(!field)
		{
			m_field.clear();
		}
		else
		{
			m_field = field;
		}
	}

	void GetValueRequest::SetOrder(const char* order)
	{
		if(!order)
		{
			m_order = augeOrderAsc;
		}
		if(!g_stricmp(order, "Desc"))
		{
			m_order = augeOrderDesc;
		}
		else
		{
			m_order = augeOrderAsc;
		}
	}

	const char* GetValueRequest::GetField()
	{
		return m_field.c_str();
	}

	augeOrderType GetValueRequest::GetOrder()
	{
		return m_order;
	}

	void GetValueRequest::SetTypeName(const char* typeName)
	{
		if(typeName==NULL)
		{
			m_type_name.clear();
		}
		else
		{
			m_type_name = typeName;
		}
	}

	const char* GetValueRequest::GetTypeName()
	{
		return m_type_name.c_str();
	}

	const char* GetValueRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}
}