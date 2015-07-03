#ifndef __AUGE_IMS_UPDATE_MAP_LAYER_ORDER_REQUEST_H__
#define __AUGE_IMS_UPDATE_MAP_LAYER_ORDER_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class UpdateMapLayerOrderRequest : public WebRequest
	{
	public:
		UpdateMapLayerOrderRequest();
		virtual ~UpdateMapLayerOrderRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

	public:
		bool		Create(rude::CGI& cgi);

		const char*	GetName();
		const char*	GetLayers();

		void		SetVersion(const char* value);
		void		SetName(const char* name);
		void		SetLayers(const char* layers);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_name;
		std::string m_layers;
		std::string m_encoding;
	};
}

#endif //__AUGE_IMS_UPDATE_MAP_LAYER_ORDER_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=ims&version=1.0.0&request=UpdateMapLayerOrder&name=world&extent=-180,-90,180,90&srid=4326
http://127.0.0.1:8088/ows/user1/mgr?service=ims&version=1.0.0&request=UpdateMapLayerOrder&name=world&extent=-180,-90,180,90&srid=4326

[ HTTP Post ]
-------------------------------------------------------------------------
<?xml version="1.0"?>
<UpdateMapLayerOrder>
	<Name>world</Name>
	<Extent>
		<xmin>-180</xmin>
		<ymin>-90</ymin>
		<xmax>180</xmax>
		<ymax>90</ymax>
	</Extent>
	<Srid>4326</Srid>
</UpdateMapLayerOrder>

************************************************************************/