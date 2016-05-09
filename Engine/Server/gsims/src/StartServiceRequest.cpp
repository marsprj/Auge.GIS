#include "StartServiceRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	StartServiceRequest::StartServiceRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding =AUGE_DB_ENCODING;
	}

	StartServiceRequest::~StartServiceRequest()
	{

	}

	bool StartServiceRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);		
		return true;
	}

	const char*	StartServiceRequest::GetEngine()
	{
		return "ims";
	}

	const char*	StartServiceRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	StartServiceRequest::GetRequest()
	{
		return "StartService";
	}

	const char* StartServiceRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void StartServiceRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	StartServiceRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool StartServiceRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void StartServiceRequest::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		else
		{
			WebContext* pWebContext = augeGetWebContextInstance();
			m_name = pWebContext->ParameterEncoding(name);
		}
	}

	const char*	StartServiceRequest::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}
}