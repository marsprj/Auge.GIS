#ifndef __AUGE_CSV_IMPORT_REQUEST_H__
#define __AUGE_CSV_IMPORT_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class CsvImportRequest : public WebRequest
	{
	public:
		CsvImportRequest();
		virtual ~CsvImportRequest();

	public:
		virtual const char*		GetUser();
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetEncoding();

	public:
		const char*				GetSourceName();
		const char*				GetTypeName();

		const char*				GetCsvPath();
		const char*				GetCsvName();
		g_uint					GetSrid();

		bool					Create(rude::CGI& cgi);

	private:
		void					SetSourceName(const char* sourceName);
		void					SetTypeName(const char* typeName);

		void					SetCsvPath(const char* path);
		void					SetCsvName(const char* name);
		void					SetSrid(const char* srid);

	private:
		std::string m_user;
		std::string m_version;
		std::string m_mime_type;
		std::string m_host;
		std::string m_request_method;
		std::string m_encoding;

		std::string	m_source_name;
		std::string m_type_name;

		std::string m_csv_path;
		std::string m_csv_name;
		g_uint		m_srid;

	};
}

#endif //__AUGE_CSV_IMPORT_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
1)
user=user1&service=gps&version=1.0.0&request=CsvImport&sourceName=db1&typeName=people_age_60&csvpath=/&csvname=60_people.csv&srid=4326

user=user1&service=gps&version=1.0.0&request=CsvImport&sourceName=db1&typeName=cities_xx&csvpath=/ddd&csvname=cities&srid=4326
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=CsvImport&sourceName=db1&typeName=bbb&csvpath=D:/GeoBeans/data/world&csvname=cities&srid=4326


service=gps&version=1.0.0&request=CsvImport&sourceName=db1&typeName=aaaa&csvname=cities.zip&srid=4326
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=CsvImport&sourceName=db1&typeName=aaaa&csvname=cities.zip&srid=4326

************************************************************************/
