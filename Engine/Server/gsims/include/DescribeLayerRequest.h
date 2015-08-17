#ifndef __AUGE_IMS_DESCRIBE_LAYER_REQUEST_H__
#define __AUGE_IMS_DESCRIBE_LAYER_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class DescribeLayerRequest : public WebRequest
	{
	public:
		DescribeLayerRequest();
		virtual ~DescribeLayerRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetMapName();
		virtual const char*		GetLayerName();
	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetMapName(const char* name);
		void		SetLayerName(const char* name);
		
	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_map_name;
		std::string m_layer_name;
		std::string m_encoding;
	};
}

#endif //__AUGE_IMS_DESCRIBE_LAYER_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
user=user1&service=ims&version=1.0.0&request=DescribeLayer&name=world
http://127.0.0.1:8088/ows/user1/mgr?service=ims&version=1.0.0&request=DescribeLayer&mapName=world&layerName=cities

************************************************************************/