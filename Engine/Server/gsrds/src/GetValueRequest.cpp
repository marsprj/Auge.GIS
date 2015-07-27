#include "GetValueRequest.h"

namespace auge
{
	GetValueRequest::GetValueRequest()
	{
		m_version = "1.0.0";
		m_encoding = AUGE_DEFAULT_ENCODING;
		m_image_x = -1;
		m_image_y = -1;
		m_x = AUGE_DOUBLE_MAX;
		m_y = AUGE_DOUBLE_MAX;
	}

	GetValueRequest::~GetValueRequest()
	{
		
	}

	bool GetValueRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetPath(cgi["path"]);
		SetRasterName(cgi["rasterName"]);
		SetSourceName(cgi["sourceName"]);
		SetIX(cgi["sx"]);
		SetIY(cgi["sy"]);
		SetX(cgi["x"]);
		SetY(cgi["y"]);
		return true;
	}

	const char*	GetValueRequest::GetEngine()
	{
		return "rds";
	}

	const char*	GetValueRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetValueRequest::GetRequest()
	{
		return "GetValue";
	}

	void GetValueRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetValueRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void GetValueRequest::SetPath(const char* path)
	{
		if(path==NULL)
		{
			m_path.clear();
		}
		else
		{
			m_path = path;
		}
	}

	const char* GetValueRequest::GetPath()
	{
		return m_path.empty() ? NULL : m_path.c_str();
	}

	const char*	GetValueRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void GetValueRequest::SetRasterName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		else
		{
			m_name = name;
		}
	}

	const char* GetValueRequest::GetRasterName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void GetValueRequest::SetIX(const char* ix)
	{
		if((ix==NULL)||(strlen(ix)==0))
		{
			m_image_x = -1;
		}
		else
		{
			m_image_x = atoi(ix);
		}
	}

	void GetValueRequest::SetIY(const char* iy)
	{
		if((iy==NULL)||(strlen(iy)==0))
		{
			m_image_y = -1;
		}
		else
		{
			m_image_y = atoi(iy);
		}
	}

	void GetValueRequest::SetX(const char* x)
	{
		if(x==NULL)
		{
			m_x = AUGE_DOUBLE_MAX;
		}
		else
		{
			m_x = atof(x);
		}
	}

	void GetValueRequest::SetY(const char* y)
	{
		if(y==NULL)
		{
			m_y = AUGE_DOUBLE_MAX;
		}
		else
		{
			m_y = atof(y);
		}
	}

	g_int GetValueRequest::GetIX()
	{
		return m_image_x;
	}

	g_int GetValueRequest::GetIY()
	{
		return m_image_y;
	}

	double GetValueRequest::GetX()
	{
		return m_x;
	}

	double GetValueRequest::GetY()
	{
		return m_y;
	}

	void GetValueRequest::SetSourceName(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_source_name.clear();
		}
		else
		{
			m_source_name = sourceName;
		}
	}

	const char* GetValueRequest::GetSourceName()
	{
		return m_source_name.empty() ? NULL : m_source_name.c_str();
	}
}