#include "GetSpatialReferenceRequest.h"
#include "AugeField.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include "AugeXML.h"
#include "AugeFilter.h"
#include "AugeWebCore.h"

namespace auge
{	
	GetSpatialReferenceRequest::GetSpatialReferenceRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml"; 
		m_encoding = AUGE_DEFAULT_ENCODING;
		m_srid  = -1;
		m_count = 50;
		m_offset= 0;
	}

	GetSpatialReferenceRequest::~GetSpatialReferenceRequest()
	{
	}

	const char*	GetSpatialReferenceRequest::GetEngine()
	{
		return "gps";
	}

	const char*	GetSpatialReferenceRequest::GetVersion()
	{
		return m_version.c_str();
	}
	
	const char*	GetSpatialReferenceRequest::GetRequest()
	{
		return "GetSpatialReference";
	}

	void GetSpatialReferenceRequest::SetSRID(const char* srid)
	{
		if(srid==NULL)
		{
			m_srid = -1;
		}
		else if(!isdigit(srid[0]))
		{
			m_srid = -1;
		}
		else
		{
			m_srid = atoi(srid);
		}
	}

	g_int GetSpatialReferenceRequest::GetSRID()
	{
		return m_srid;
	}

	void GetSpatialReferenceRequest::SetVersion(const char* version)
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

	void GetSpatialReferenceRequest::SetEncoding(const char* encoding)
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

	const char* GetSpatialReferenceRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	const char*	GetSpatialReferenceRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	g_uint GetSpatialReferenceRequest::GetCount()
	{
		return m_count;
	}

	g_uint GetSpatialReferenceRequest::GetOffset()
	{
		return m_offset;
	}
	
	void GetSpatialReferenceRequest::SetCount(const char* count)
	{
		if(count==NULL)
		{
			m_count = 50;
		}
		else if(!isdigit(count[0]))
		{
			m_count = 50;
		}
		else
		{
			m_count = atoi(count);
		}
	}

	void GetSpatialReferenceRequest::SetOffset(const char* offset)
	{
		if(offset==NULL)
		{
			m_offset = 0;
		}
		else if(!isdigit(offset[0]))
		{
			m_offset = 10;
		}
		else
		{
			m_offset = atoi(offset);
		}
	}

	const char* GetSpatialReferenceRequest::GetUser()
	{
		return m_user.c_str();
	}

	void GetSpatialReferenceRequest::SetUser(const char* user)
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

	const char* GetSpatialReferenceRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* GetSpatialReferenceRequest::GetHost()
	{
		return m_host.c_str();
	}

	bool GetSpatialReferenceRequest::Create(rude::CGI& cgi)
	{
		SetUser(cgi["user"]);
		SetVersion(cgi["version"]);
		SetCount(cgi["count"]);
		SetOffset(cgi["offset"]);
		SetSRID(cgi["srid"]);
		SetEncoding(cgi["encoding"]);
		
		return true;
	}
}