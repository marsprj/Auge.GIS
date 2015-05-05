#ifndef __AUGE_IMS_TEST_CONNECTION_REQUEST_H__
#define __AUGE_IMS_TEST_CONNECTION_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class TryConnectionRequest : public WebRequest
	{
	public:
		TryConnectionRequest();
		virtual ~TryConnectionRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();

	public:
		bool		Create(rude::CGI& cgi);

		const char*	GetDataEngine();
		const char* GetURI();

		void		SetVersion(const char* value);
		void		SetDataEngine(const char* engine);
		void		SetURI(const char* uri);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_data_engine;
		std::string m_uri;
	};
}

#endif //__AUGE_IMS_TEST_CONNECTION_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=dbs&version=1.0.0&request=TryConnection&name=db221&engine=Postgres&uri=server=127.0.0.1;instance=5432;database=gisdb;user=postgres;password=qwer1234;encoding=GBK
http://127.0.0.1:8088/ows/user1/mgr?service=dbs&version=1.0.0&request=TryConnection&name=db221&engine=Postgres&uri=server=127.0.0.1;instance=5432;database=gisdb;user=postgres;password=qwer1234;encoding=GBK
http://192.168.111.160:8088/ows/user1/mgr?service=dbs&version=1.0.0&request=TryConnection&name=db221&engine=Postgres&uri=server=127.0.0.1;instance=5432;database=gisdb;user=postgres;password=qwer1234;encoding=GBK

[ HTTP Post ]
-------------------------------------------------------------------------
//<?xml version="1.0"?>
//<TryConnection>
//	<ServiceName>world</ServiceName>
//	<Handlers>
//		<Handler name="wms"/>
//		<Handler name="wfs"/>
//	</Handlers>
//</TryConnection>

************************************************************************/