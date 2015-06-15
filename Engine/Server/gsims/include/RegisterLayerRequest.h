#ifndef __AUGE_IMS_REGISTER_LAYER_REQUEST_H__
#define __AUGE_IMS_REGISTER_LAYER_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include "AugeCarto.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class RegisterLayerRequest : public WebRequest
	{
	public:
		RegisterLayerRequest();
		virtual ~RegisterLayerRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();

	public:
		bool		Create(rude::CGI& cgi);

		const char*	GetName();
		const char*	GetMapName();
		const char* GetDataSource();
		const char* GetLayerName();
		const char* GetTableName();
		const char* GetWebURL();
		augeLayerType GetType();

		void		SetType(const char* type);
		void		SetVersion(const char* value);
		void		SetMapName(const char* mapName);
		void		SetDataSource(const char* dataSource);
		void		SetLayerName(const char* layerName);
		void		SetTableName(const char* tableName);
		void		SetWebURL(const char* url);

	private:
		augeLayerType m_type;
		std::string m_version;
		std::string m_mime_type;
		std::string m_map_name;
		std::string m_data_source;
		std::string m_layer_name;
		std::string m_table_name;
		std::string m_web_url;
	};
}

#endif //__AUGE_IMS_REGISTER_LAYER_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=ims&version=1.0.0&request=RegisterLayer&mapName=world&datasource=db1&layerName=cities&tableName=cities
http://127.0.0.1:8088/ows/user1/mgr?service=ims&version=1.0.0&request=RegisterLayer&mapName=world&datasource=db1&layerName=cities&tableName=cities

http://192.168.111.160:8088/ows/user1/mgr?service=ims&version=1.0.0&request=RegisterLayer&mapName=world&datasource=db1&layerName=cities&tableName=cities

[ HTP Post ]
-------------------------------------------------------------------------
//<?xml version="1.0"?>
//<RegisterLayer>
//	<ServiceName>world</ServiceName>
//	<Handlers>
//		<Handler name="wms"/>
//		<Handler name="wfs"/>
//	</Handlers>
//</RegisterLayer>

************************************************************************/