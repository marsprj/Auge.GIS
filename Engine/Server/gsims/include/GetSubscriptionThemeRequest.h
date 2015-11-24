#ifndef __AUGE_GET_SUBSCRIPTION_THEME_REQUEST_H__
#define __AUGE_GET_SUBSCRIPTION_THEME_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GetSubscriptionThemeRequest : public WebRequest
	{
	public:
		GetSubscriptionThemeRequest();
		virtual ~GetSubscriptionThemeRequest();

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
		
		void		Info();
		
	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_theme;
		std::string m_encoding;
	};
}

#endif //__AUGE_GET_SUBSCRIPTION_THEME_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
user=user1&service=ims&version=1.0.0&request=GetSubscriptionTheme&theme=poi
http://127.0.0.1:8088/ows/user1/mgr?service=ims&version=1.0.0&request=GetSubscriptionTheme&theme=poi

************************************************************************/