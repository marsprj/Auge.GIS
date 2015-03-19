#ifndef __AUGE_IMS_MAP_CREATE_REQUEST_H__
#define __AUGE_IMS_MAP_CREATE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class CreateMapRequest : public WebRequest
	{
	public:
		CreateMapRequest();
		virtual ~CreateMapRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();

	public:
		bool		Create(rude::CGI& cgi);

		const char*	GetName();
		const char*	GetExtent();
		const char*	GetSRID();

		void		SetVersion(const char* value);
		void		SetName(const char* name);
		void		SetExtent(const char* extent);
		void		SetSRID(const char* srid);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_name;
		std::string m_extent;
		std::string m_srid;

	};
}

#endif //__AUGE_IMS_MAP_CREATE_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=ims&version=1.0.0&request=CreateMap&name=world&extent=-180,-90,180,90&srid=4326
http://127.0.0.1:8088/ows/user1/mgr?service=ims&version=1.0.0&request=CreateMap&name=world&extent=-180,-90,180,90&srid=4326

[ HTTP Post ]
-------------------------------------------------------------------------
<?xml version="1.0"?>
<CreateMap>
	<Name>world</Name>
	<Extent>
		<xmin>-180</xmin>
		<ymin>-90</ymin>
		<xmax>180</xmax>
		<ymax>90</ymax>
	</Extent>
	<Srid>4326</Srid>
</CreateMap>

************************************************************************/