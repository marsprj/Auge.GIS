#ifndef __AUGE_DBS_REFRESH_DATASET_REQUEST_H__
#define __AUGE_DBS_REFRESH_DATASET_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class RefreshDatasetRequest : public WebRequest
	{
	public:
		RefreshDatasetRequest();
		virtual ~RefreshDatasetRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetSourceName();
		virtual const char*		RefreshDatasetName();
	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetSourceName(const char* name);
		void		SetDataSetName(const char* name);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_source_name;
		std::string m_dataset_name;
		std::string m_encoding;
	};
}

#endif //__AUGE_DBS_REFRESH_DATASET_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=dbs&version=1.0.0&request=Refresh&sourceName=db1&dataSetName=cities
http://127.0.0.1:8088/ows/user1/mgr?service=dbs&version=1.0.0&request=Refresh&sourceName=db1&dataSetName=cities

************************************************************************/