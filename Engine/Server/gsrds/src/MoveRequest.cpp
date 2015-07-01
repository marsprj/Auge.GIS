#include "MoveRequest.h"

namespace auge
{
	MoveRequest::MoveRequest()
	{
		m_version = "1.0.0";
		m_encoding = "GBK";
	}

	MoveRequest::~MoveRequest()
	{

	}

	bool MoveRequest::Create(rude::CGI& cgi) 
	{
		SetVersion(cgi["version"]);
		SetSrc(cgi["src"]);
		SetDes(cgi["des"]);
		return true;
	}

	const char*	MoveRequest::GetEngine()
	{
		return "rds";
	}

	const char*	MoveRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	MoveRequest::GetRequest()
	{
		return "Move";
	}

	void MoveRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	MoveRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void MoveRequest::SetSrc(const char* path)
	{
		if(path==NULL)
		{
			m_src_path.clear();
		}
		else
		{
			m_src_path = path;
		}
	}

	void MoveRequest::SetDes(const char* path)
	{
		if(path==NULL)
		{
			m_des_path.clear();
		}
		else
		{
			m_des_path = path;
		}
	}

	const char* MoveRequest::GetSrc()
	{
		return m_src_path.empty() ? NULL : m_src_path.c_str();
	}

	const char* MoveRequest::GetDes()
	{
		return m_des_path.empty() ? NULL : m_des_path.c_str();
	}

	const char*	MoveRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

}