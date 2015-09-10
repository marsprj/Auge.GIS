#ifndef __AUGE_IMS_CREATE_USER_REQUEST_H__
#define __AUGE_IMS_CREATE_USER_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class CreateUserRequest : public WebRequest
	{
	public:
		CreateUserRequest();
		virtual ~CreateUserRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();
		virtual void			Release();

	public:
		const char*	GetName();
		const char*	GetAlias();
		const char*	GetPassword();
		const char*	GetEmail();
		const char* GetRole();

	public:
		void	SetName(const char* name);
		void	SetAlias(const char* alias);
		void	SetPassword(const char* passwd);
		void	SetEmail(const char* email);
		void	SetRole(const char* role);

	public:
		bool		Create(rude::CGI& cgi);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_encoding;
		
		std::string m_name;
		std::string m_alias;
		std::string m_passwd;
		std::string m_email;
		std::string m_role;

	};
}

#endif //__AUGE_IMS_CREATE_USER_REQUEST_H__

/************************************************************************
[ HTTP Get ]
service=was&version=1.0.0&request=CreateUser&name=user1&alias=user1&password=qwer1234&email=user@mapcloud.org&role=normal
http://127.0.0.1:8088/ows/mgr?service=was&version=1.0.0&request=CreateUser&name=user1&alias=user1&password=qwer1234&email=user@mapcloud.org&role=normal

[ HTTP Post ]
-------------------------------------------------------------------------
http://127.0.0.1:8088/ows/user1/mgr?

<CreateUser> 
	service="was">
	<Field>
		<Name>user1</Name>
		<Alias>user1</Alias>
		<Password>qwer1234</Password>
		<Email>aaa@mapcloud.org</Email>
		<Role>normal</Role>
	</Field>
</CreateUser>

************************************************************************/