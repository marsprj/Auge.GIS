#ifndef __AUGE_IMS_LOGIN_REQUEST_H__
#define __AUGE_IMS_LOGIN_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class LoginRequest : public WebRequest
	{
	public:
		LoginRequest();
		virtual ~LoginRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();
		virtual void			Release();

	public:
		const char*	GetName();
		const char* GetPassord();

	public:
		void		SetName(const char* name);
		void		SetPassword(const char* password);

	public:
		bool		Create(rude::CGI& cgi);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_encoding;
		
		std::string m_name;
		std::string m_password;
	};
}

#endif //__AUGE_IMS_LOGIN_REQUEST_H__

/************************************************************************
[ HTTP Get ]
service=wus&version=1.0.0&request=Login&name=user2
http://127.0.0.1:8088/ows/mgr?service=wus&version=1.0.0&request=Login&name=user2


************************************************************************/