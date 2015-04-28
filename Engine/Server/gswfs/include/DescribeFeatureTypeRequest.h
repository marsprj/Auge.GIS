#ifndef __AUGE_WFS_DESCRIBE_FEATURETYPE_REQUEST_H__
#define __AUGE_WFS_DESCRIBE_FEATURETYPE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>
#include "WFeatureRequest.h"

namespace auge
{
	class DescribeFeatureTypeRequest : public WFeatureRequest
	{
	public:
		DescribeFeatureTypeRequest();
		virtual ~DescribeFeatureTypeRequest();

	public:
		//virtual const char*		GetEngine();
		//virtual const char*		GetVersion();
		virtual const char*		GetTypeName();
		virtual const char*		GetRequest();
		//virtual const char*		GetMimeType();

	public:
		//void		SetVersion(const char* value);
		void		SetTypeName(const char* typeName);
		bool		Create(rude::CGI& cgi);

	private:
		//std::string m_version;
		std::string m_type_name;
		std::string m_full_name;
		std::string m_mime_type;
	};
}

#endif //__AUGE_WFS_DESCRIBE_FEATURETYPE_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=wfs&version=1.0.0&request=DescribeFeatureType&typeName=world:cities
service=wfs&version=1.0.0&request=DescribeFeatureType&typeName=world:cities&mapName=world
service=wfs&version=1.0.0&request=DescribeFeatureType&typeName=world:cities&sourceName=world
user=user1&servicename=world&service=wfs&version=1.1.0&request=DescribeFeatureType&typeName=cities
http://127.0.0.1:8088/ows/user1/world/ims?service=wfs&version=1.0.0&request=DescribeFeatureType&typeName=cities

http://127.0.0.1:8088/ows/user1/world/ims?service=wfs&version=1.0.0&request=DescribeFeatureType&typeName=cities

************************************************************************/
