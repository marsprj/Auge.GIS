#include "GetValueRequest.h"
#include "AugeWebCore.h"

namespace auge
{

	GetValueRequest::GetValueRequest():
	m_version("1.0.0"),
	m_mime_type("text/xml"),
	m_order(augeOrderAsc),
	m_encoding(AUGE_DEFAULT_ENCODING)
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
		WebContext* pWebContext = augeGetWebContextInstance();
		char parameter[AUGE_NAME_MAX];
		
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);
		//SetServiceName(cgi["servicename"]);
		auge_web_parameter_encoding(cgi["typeName"], parameter, AUGE_NAME_MAX, pWebContext->IsIE());
		SetTypeName(cgi["typeName"]);
		auge_web_parameter_encoding(cgi["field"], parameter, AUGE_NAME_MAX, pWebContext->IsIE());
		SetField(cgi["field"]);
		SetOrder(cgi["order"]);
		auge_web_parameter_encoding(cgi["mapName"], parameter, AUGE_NAME_MAX, pWebContext->IsIE());
		SetMapName(cgi["mapName"]);
		auge_web_parameter_encoding(cgi["sourceName"], parameter, AUGE_NAME_MAX, pWebContext->IsIE());
		SetSourceName(cgi["sourceName"]);
		SetEncoding(cgi["encoding"]);
		SetType(cgi["type"]);
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

	void GetValueRequest::SetEncoding(const char* encoding)
	{
		if((encoding==NULL)||(strlen(encoding)==0))
		{
			m_encoding = AUGE_DEFAULT_ENCODING;
		}
		else
		{
			m_encoding = encoding;
		}
	}

	void GetValueRequest::SetType(const char* type)
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

	const char* GetValueRequest::GetType()
	{
		return m_type.empty() ? NULL : m_type.c_str();
	}
}