#ifndef __AUGE_IMS_MAP_SAVE_REQUEST_H__
#define __AUGE_IMS_MAP_SAVE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include "AugeXML.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class SaveMapRequest : public WebRequest
	{
	private:
		typedef struct 
		{
			char id[32];
			char name[32];
		}layer_info_t;

	public:
		SaveMapRequest();
		virtual ~SaveMapRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

	public:
		bool		Create(XDocument* pxDoc);

		const char*	GetName();
		GEnvelope&	GetExtent();
		GEnvelope&	GetViewer();
		const char*	GetSRID();
		g_uint		GetLayerCount();
		const char*	GetLayerID(g_uint i);
		const char*	GetLayerName(g_uint i);

		void		SetVersion(const char* value);
		void		SetSRID(const char* srid);
		void		SetName(const char* name);
		void		SetExtent(XElement* pxElement);
		void		SetViewer(XElement* pxElement);		
		void		ParserLayers(XNode* pxLayers);

	private:
		XDocument*		m_pxDoc;
		std::string		m_version;
		std::string		m_mime_type;
		std::string		m_encoding;

		std::string		m_map_id;
		std::string		m_map_name;
		std::string		m_srid;
		GEnvelope		m_extent;
		GEnvelope		m_viewer;

		g_uint			m_layer_count;
		layer_info_t*	m_layers;
	};
}

#endif //__AUGE_IMS_MAP_SAVE_REQUEST_H__

/************************************************************************

[ HTTP Post ]
-------------------------------------------------------------------------
<?xml version="1.0"?>
<SaveMap
	service="ims"
	version="1.0.0"
	user="user1">
	<Name>world</Name>
	<Srid>4326</Srid>
	<Extent xmin="-180" ymin="90" xmax="180" ymax="90"/>
	<Viewer xmin="-180" ymin="90" xmax="180" ymax="90"/>
	<Layers>
		<Layer name="cities" id="2"/>
		<Layer name="rivers" id="3"/>
		<Layer name="country" id="4"/>
	</Layers>
</SaveMap>

************************************************************************/