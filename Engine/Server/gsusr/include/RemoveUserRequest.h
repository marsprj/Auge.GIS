#ifndef __AUGE_IMS_REMOVE_USER_REQUEST_H__
#define __AUGE_IMS_REMOVE_USER_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class RemoveUserRequest : public WebRequest
	{
	public:
		RemoveUserRequest();
		virtual ~RemoveUserRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual void			Release();

	public:
		const char*	GetName();

	public:
		void	SetName(const char* name);

	public:
		bool		Create(rude::CGI& cgi);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_encoding;
		
		std::string m_name;
	};
}

#endif //__AUGE_IMS_REMOVE_USER_REQUEST_H__

/************************************************************************
[ HTTP Get ]
service=wus&version=1.0.0&request=RemoveUser&name=user2
http://127.0.0.1:8088/ows/mgr?service=wus&version=1.0.0&request=RemoveUser&name=user2


************************************************************************/