#include "DescribeTaskRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	DescribeTaskRequest::DescribeTaskRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	DescribeTaskRequest::~DescribeTaskRequest()
	{

	}

	bool DescribeTaskRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);

		char str[AUGE_NAME_MAX];
		WebContext* pWebContext = augeGetWebContextInstance();

		const char* name = pWebContext->ParameterEncoding(cgi["name"]);
		//auge_web_parameter_encoding(cgi["name"], str, AUGE_NAME_MAX, pWebContext->IsIE());
		SetName(name);

		SetRole(cgi["role"]);

		return true;
	}

	const char*	DescribeTaskRequest::GetEngine()
	{
		return "ims";
	}

	const char*	DescribeTaskRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	DescribeTaskRequest::GetRequest()
	{
		return "DescribeTask";
	}

	const char* DescribeTaskRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void DescribeTaskRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	DescribeTaskRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool DescribeTaskRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void DescribeTaskRequest::SetName(const char* name)
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

	const char*	DescribeTaskRequest::GetName()
	{
		if(m_name.empty())
		{
			return NULL;
		}
		return m_name.c_str();
	}

	void DescribeTaskRequest::SetRole(const char* role)
	{
		if(role==NULL)
		{
			m_role = -1;
			return;
		}

		if(strlen(role)==0)
		{
			m_role = -1;
			return;
		}

		if(g_stricmp(role, "owner")==0)
		{
			m_role = 0;
		}
		else if(g_stricmp(role, "joined")==0)
		{
			m_role = 1;
		}
		else
		{
			m_role = -1;
		}
	}

	g_int DescribeTaskRequest::GetRole()
	{
		return m_role;
	}
}