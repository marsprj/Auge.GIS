#ifndef __AUGE_IMS_DESCRIBE_TILESTORE_REQUEST_H__
#define __AUGE_IMS_DESCRIBE_TILESTORE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class DescribeTileStoreRequest : public WebRequest
	{
	public:
		DescribeTileStoreRequest();
		virtual ~DescribeTileStoreRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetSourceName();
		virtual const char*		GetStoreName();
	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetSourceName(const char* name);
		void		SetStoreName(const char* name);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_source_name;
		std::string m_store_name;
		std::string m_encoding;
	};
}

#endif //__AUGE_IMS_DESCRIBE_TILESTORE_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
user=user1&service=dbs&version=1.0.0&request=DescribeTileStore&sourceName=tfs&storeName=google

http://127.0.0.1:8088/ows/user1/mgr?service=dbs&version=1.0.0&request=DescribeTileStore&sourceName=tfs&storeName=cities

************************************************************************/