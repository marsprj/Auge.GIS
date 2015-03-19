#ifndef __AUGE_IMS_REGISTER_LAYER_REQUEST_H__
#define __AUGE_IMS_REGISTER_LAYER_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
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

		void		SetVersion(const char* value);
		void		SetMapName(const char* mapName);
		void		SetDataSource(const char* dataSource);
		void		SetLayerName(const char* layerName);
		void		SetTableName(const char* tableName);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_map_name;
		std::string m_data_source;
		std::string m_layer_name;
		std::string m_table_name;
	};
}

#endif //__AUGE_IMS_REGISTER_LAYER_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=ims&version=1.0.0&request=RegisterLayer&mapName=world&datasource=db221&layerName=cities&tableName=cities

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