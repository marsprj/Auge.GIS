#ifndef __AUGE_IMS_DESCRIBE_CSV_REQUEST_H__
#define __AUGE_IMS_DESCRIBE_CSV_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class DescribeCsvRequest : public WebRequest
	{
	public:
		DescribeCsvRequest();
		virtual ~DescribeCsvRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetPath();
	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetPath(const char* name);
		
	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_path;
		std::string m_encoding;
	};
}

#endif //__AUGE_IMS_DESCRIBE_CSV_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
user=user1&service=ufs&version=1.0.0&request=DescribeCsv&path=/60_people.csv
http://127.0.0.1:8088/ows/user1/mgr?service=ufs&version=1.0.0&request=DescribeCsv&path=/60_people.csv

************************************************************************/