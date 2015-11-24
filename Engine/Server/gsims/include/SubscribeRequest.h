#ifndef __AUGE_IMS_SUBSCRIBE_REQUEST_H__
#define __AUGE_IMS_SUBSCRIBE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class SubscribeRequest : public WebRequest
	{
	public:
		SubscribeRequest();
		virtual ~SubscribeRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		
	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetTheme(const char* theme);
		const char*	GetTheme();

		void		SetKeyWords(const char* keywords);
		const char*	GetKeyword(g_uint index);
		g_uint		GetKeywordCount();

		void		Info();
		
	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_theme;
		std::vector<std::string> m_keywords;
		std::string m_encoding;
	};
}

#endif //__AUGE_IMS_SUBSCRIBE_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
user=user1&service=ims&version=1.0.0&request=Subscribe&theme=poi&keywords=医院,学校,银行
http://192.168.111.88:8080/ows/user1/mgr?service=ims&version=1.0.0&request=Subscribe&theme=poi&keywords=医院,学校,银行

************************************************************************/