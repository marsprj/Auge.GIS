#include "TransactionRequest.h"

namespace auge
{
	TransactionRequest::TransactionRequest()
	{
		m_version = "1.1.0";
		m_mime_type = "text/xml";
	}

	TransactionRequest::~TransactionRequest()
	{

	}

	const char*	TransactionRequest::GetEngine()
	{
		return "wfs";
	}

	const char*	TransactionRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char* TransactionRequest::GetTypeName()
	{
		return m_type_name.c_str();
	}

	const char*	TransactionRequest::GetRequest()
	{
		return "Transaction";
	}

	void TransactionRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	void TransactionRequest::SetTypeName(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		const char* sep = strstr(value,":");
		m_full_name = value;
		m_type_name = sep==NULL ? value : sep+1;
	}

	const char*	TransactionRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool TransactionRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetTypeName(cgi["typeName"]);
		return true;
	}

}