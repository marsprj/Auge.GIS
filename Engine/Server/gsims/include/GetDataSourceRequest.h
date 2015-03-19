#ifndef __AUGE_IMS_DATASOURCE_GET_REQUEST_H__
#define __AUGE_IMS_DATASOURCE_GET_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GetDataSourceRequest : public WebRequest
	{
	public:
		GetDataSourceRequest();
		virtual ~GetDataSourceRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();

		virtual const char*		GetName();
	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetName(const char* name);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_name;
	};
}

#endif //__AUGE_IMS_DATASOURCE_GET_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=ims&version=1.0.0&request=GetDataSource&name=db1
http://127.0.0.1:8088/ows/user1/mgr?service=ims&version=1.0.0&request=GetDataSource&name=db1

************************************************************************/