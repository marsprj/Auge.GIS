#ifndef __AUGE_IMS_GET_DATASET_REQUEST_H__
#define __AUGE_IMS_GET_DATASET_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GetDataSetRequest : public WebRequest
	{
	public:
		GetDataSetRequest();
		virtual ~GetDataSetRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();

		virtual const char*		GetSourceName();
		virtual const char*		GetDataSetName();
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
	};
}

#endif //__AUGE_IMS_GET_DATASET_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=ims&version=1.0.0&request=GetDataSet&sourceName=db1&dataSetName=cities
http://127.0.0.1:8088/ows/user1/mgr?service=ims&version=1.0.0&request=GetDataSet&sourceName=db1&dataSetName=cities

************************************************************************/