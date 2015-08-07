#include "GetSpatialReferenceCountRequest.h"
#include "AugeField.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include "AugeXML.h"
#include "AugeFilter.h"
#include "AugeWebCore.h"

namespace auge
{	
	GetSpatialReferenceCountRequest::GetSpatialReferenceCountRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml"; 
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	GetSpatialReferenceCountRequest::~GetSpatialReferenceCountRequest()
	{
	}

	const char*	GetSpatialReferenceCountRequest::GetEngine()
	{
		return "gps";
	}

	const char*	GetSpatialReferenceCountRequest::GetVersion()
	{
		return m_version.c_str();
	}
	
	const char*	GetSpatialReferenceCountRequest::GetRequest()
	{
		return "GetSpatialReferenceCount";
	}

	void GetSpatialReferenceCountRequest::SetVersion(const char* version)
	{
		if((version==NULL)||(strlen(version)==0))
		{
			m_version = "1.0.0";
		}
		else
		{
			m_version = version;
		}
	}

	void GetSpatialReferenceCountRequest::SetEncoding(const char* encoding)
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

	const char* GetSpatialReferenceCountRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	const char*	GetSpatialReferenceCountRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}
	
	const char* GetSpatialReferenceCountRequest::GetUser()
	{
		return m_user.c_str();
	}

	void GetSpatialReferenceCountRequest::SetUser(const char* user)
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

	const char* GetSpatialReferenceCountRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* GetSpatialReferenceCountRequest::GetHost()
	{
		return m_host.c_str();
	}

	bool GetSpatialReferenceCountRequest::Create(rude::CGI& cgi)
	{
		SetUser(cgi["user"]);
		SetVersion(cgi["version"]);
		SetEncoding(cgi["encoding"]);
		
		return true;
	}
}