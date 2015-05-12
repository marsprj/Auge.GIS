#ifndef __AUGE_WPS_CAPABILITIES_REQUEST_H__
#define __AUGE_WPS_CAPABILITIES_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class DescribeProcessRequest : public WebRequest
	{
		friend class DescribeProcessHandler;

	public:
		DescribeProcessRequest();
		virtual ~DescribeProcessRequest();

	public:
		virtual const char*		GetUser();
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetLanguage();
		virtual const char*		GetIdentifier(g_uint i);
		virtual g_uint			GetIdentifierCount();

		virtual const char*		GetHost();
		virtual const char*		GetRequestMethod();

	public:
		bool		Create(rude::CGI& cgi);
		void		SetUser(const char* user);
		void		SetVersion(const char* value);

		void		SetHost(const char* host);
		void		SetRequestMethod(const char* method);

		void		SetLanguage(const char* language);
		void		SetIdentifiers(const char* identifiers);

		void		Info();

	private:
		std::string m_name;
		std::string m_user;
		std::string m_version;
		std::string m_mime_type;
		std::string m_host;
		std::string m_request_method;
		std::string m_language;

		std::vector<std::string> m_identifiers;

	private:
		static std::string	DEFAULT_HOST;
		static std::string	DEFAULT_METHOD;
	};

}

#endif //__AUGE_WPS_CAPABILITIES_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=wps&version=1.0.0&request=DescribeProcess&Language=zh&Identifier=buffer
service=wps&version=1.0.0&request=DescribeProcess&Language=zh&Identifier=intersection,union

http://127.0.0.1:8088/ows/admin/mgr?service=wps&version=1.0.0&request=DescribeProcess&Language=zh&Identifier=buffer
http://123.57.207.198/ows/admin/mgr?service=wps&version=1.0.0&request=DescribeProcess&Language=zh&Identifier=buffer

[ HTTP Post ]
<DescribeProcess service="WPS" version="1.0.0" language="en">
	<ows:Identifier>buffer</ows:Identifier>
</DescribeProcess"

<DescribeProcess service="WPS" version="1.0.0" language="en">
	<ows:Identifier>intersection</ows:Identifier>
	<ows:Identifier>union</ows:Identifier>
</DescribeProcess">

************************************************************************/
