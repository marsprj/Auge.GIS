#ifndef __AUGE_IMS_DATASET_CREATE_REQUEST_H__
#define __AUGE_IMS_DATASET_CREATE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GField;
	class GFields;
	class GeometryDef;
	class XNode;
	class XElement;
	class XDocument;

	class RemoveDataSetRequest : public WebRequest
	{
	public:
		RemoveDataSetRequest();
		virtual ~RemoveDataSetRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

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
		std::string m_encoding;
	};
}

#endif //__AUGE_IMS_DATASET_CREATE_REQUEST_H__

/************************************************************************
service=dbs&version=1.0.0&request=RemoveDataSet&sourceName=db1&dataSetName=table1
http://127.0.0.1:8088/ows/user1/mgr?service=dbs&version=1.0.0&request=RemoveDataSet&sourceName=db1&dataSetName=table1

[ HTTP Post ]

************************************************************************/