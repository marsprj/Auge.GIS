#ifndef __AUGE_FEATURE_IMPORT_REQUEST_H__
#define __AUGE_FEATURE_IMPORT_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class FeatureImportRequest : public WebRequest
	{
	public:
		FeatureImportRequest();
		virtual ~FeatureImportRequest();

	public:
		virtual const char*		GetUser();
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();

	public:
		const char*				GetSourceName();
		const char*				GetTypeName();

		const char*				GetShapePath();
		const char*				GetShapeName();
		g_uint					GetSrid();

		bool					Create(rude::CGI& cgi);

	private:
		void					SetSourceName(const char* sourceName);
		void					SetTypeName(const char* typeName);

		void					SetShapePath(const char* path);
		void					SetShapeName(const char* name);
		void					SetSrid(const char* srid);

	private:
		std::string m_user;
		std::string m_version;
		std::string m_mime_type;
		std::string m_host;
		std::string m_request_method;

		std::string	m_source_name;
		std::string m_type_name;

		std::string m_shp_path;
		std::string m_shp_name;
		g_uint		m_srid;

	};
}

#endif //__AUGE_FEATURE_IMPORT_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
1)
service=gps&version=1.0.0&request=FeatureImport&sourceName=db1&typeName=aaaa&shppath=D:/GeoBeans/data/world&shpname=cities&srid=4326
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=FeatureImport&sourceName=db1&typeName=bbb&shppath=D:/GeoBeans/data/world&shpname=cities&srid=4326


service=gps&version=1.0.0&request=FeatureImport&sourceName=db1&typeName=aaaa&shpname=cities.zip&srid=4326
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=FeatureImport&sourceName=db1&typeName=aaaa&shpname=cities.zip&srid=4326

************************************************************************/
