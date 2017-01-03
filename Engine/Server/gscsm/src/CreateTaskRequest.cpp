#include "CreateTaskRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	CreateTaskRequest::CreateTaskRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	CreateTaskRequest::~CreateTaskRequest()
	{

	}

	bool CreateTaskRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);

		char str[AUGE_NAME_MAX];
		WebContext* pWebContext = augeGetWebContextInstance();

		const char* name = pWebContext->ParameterEncoding(cgi["name"]);
		//auge_web_parameter_encoding(cgi["name"], str, AUGE_NAME_MAX, pWebContext->IsIE());
		SetName(name);

		const char* mapName = pWebContext->ParameterEncoding(cgi["mapName"]);
		SetMapName(mapName);

		const char* description = pWebContext->ParameterEncoding(cgi["description"]);
		SetDescription(description);

		return true;
	}

	const char*	CreateTaskRequest::GetEngine()
	{
		return "csm";
	}

	const char*	CreateTaskRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	CreateTaskRequest::GetRequest()
	{
		return "CreateTask";
	}

	const char* CreateTaskRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void CreateTaskRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	CreateTaskRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool CreateTaskRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void CreateTaskRequest::SetName(const char* name)
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

	const char*	CreateTaskRequest::GetName()
	{
		if(m_name.empty())
		{
			return NULL;
		}
		return m_name.c_str();
	}

	void CreateTaskRequest::SetMapName(const char* name)
	{
		if(name==NULL)
		{
			m_mapName.clear();
		}
		else if(strlen(name)==0)
		{
			m_mapName.clear();
		}
		else
		{
			m_mapName = name;
		}
	}

	const char*	CreateTaskRequest::GetMapName()
	{
		if(m_mapName.empty())
		{
			return NULL;
		}
		return m_mapName.c_str();
	}

	void CreateTaskRequest::SetDescription(const char* description)
	{
		if(description==NULL)
		{
			m_description.clear();
		}
		else if(strlen(description)==0)
		{
			m_description.clear();
		}
		else
		{
			m_description = description;
		}

	}

	const char*	CreateTaskRequest::GetDescription()
	{
		if(m_description.empty())
		{
			return "";
		}
		return m_description.c_str();
	}

}