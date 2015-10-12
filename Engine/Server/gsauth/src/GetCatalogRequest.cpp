#include "GetCatalogRequest.h"
#include "AugeXML.h"
#include "AugeField.h"

namespace auge
{
	GetCatalogRequest::GetCatalogRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
		
	}

	GetCatalogRequest::~GetCatalogRequest()
	{
		
	}

	bool GetCatalogRequest::Create(rude::CGI& cgi)
	{
		SetUser(cgi["user"]);
		return true;
	}

	const char* GetCatalogRequest::GetRequest()
	{
		return "GetCatalog";
	}
	 
	const char* GetCatalogRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char* GetCatalogRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char*	GetCatalogRequest::GetEngine()
	{
		return "was";
	}

	const char* GetCatalogRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	const char*	GetCatalogRequest::GetUser()
	{
		return m_user.c_str();
	}

	void GetCatalogRequest::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	void GetCatalogRequest::SetUser(const char* user)
	{
		if(user)
		{
			m_user = user;
		}
		else
		{
			m_user.clear();
		}
	}
}