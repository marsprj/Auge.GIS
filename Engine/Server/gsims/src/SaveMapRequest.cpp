#include "SaveMapRequest.h"

namespace auge
{
	SaveMapRequest::SaveMapRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
		m_pxDoc = NULL;
		m_layer_count = 0;
		m_layers = NULL;
	}

	SaveMapRequest::~SaveMapRequest()
	{
		if(m_layers!=NULL)
		{
			free(m_layers);
			m_layers = NULL;
		}
		if(m_pxDoc!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pxDoc)
		}
	}

	/*
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
			<Layer name="cities" id="2">
			<Layer name="rivers" id="3">
			<Layer name="country" id="4">
		</Layers>
	</SaveMap>
	*/
	bool SaveMapRequest::Create(XDocument* pxDoc)
	{
		XElement	*pxRoot = NULL;
		XAttribute	*pxAttr = NULL;

		m_pxDoc = pxDoc;

		pxRoot = pxDoc->GetRootNode();
		pxAttr = pxRoot->GetAttribute("version");
		if(pxAttr!=NULL)
		{
			SetVersion(pxAttr->GetValue());
		}

		XElement* pxMapName = (XElement*)pxRoot->GetFirstChild("Name");
		if(pxMapName==NULL)
		{
			return NULL;
		}
		SetName(pxMapName->GetContent());

		XElement* pxMapSrid = (XElement*)pxRoot->GetFirstChild("Srid");
		if(pxMapSrid==NULL)
		{
			return NULL;
		}
		SetSRID(pxMapSrid->GetContent());

		XElement* pxMapExtent = (XElement*)pxRoot->GetFirstChild("Extent");
		SetExtent(pxMapExtent);

		XElement* pxMapViewer = (XElement*)pxRoot->GetFirstChild("Viewer");
		SetViewer(pxMapViewer);

		XNode* pxLayers = pxRoot->GetFirstChild("Layers");
		ParserLayers(pxLayers);
		

		return true;
	}

	const char*	SaveMapRequest::GetEngine()
	{
		return "ims";
	}

	const char*	SaveMapRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	SaveMapRequest::GetRequest()
	{
		return "SaveMap";
	}

	const char* SaveMapRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void SaveMapRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	SaveMapRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void SaveMapRequest::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_map_name.clear();
		}
		else
		{
			m_map_name = name;
		}
	}
	
	const char*	SaveMapRequest::GetName()
	{
		if(m_map_name.empty())
		{
			return NULL;
		}
		return m_map_name.c_str();
	}

	GEnvelope& SaveMapRequest::GetExtent()
	{
		return m_extent;
	}

	GEnvelope& SaveMapRequest::GetViewer()
	{
		return m_viewer;
	}

	void SaveMapRequest::SetExtent(XElement* pxExtent)
	{
		if(pxExtent==NULL)
		{
			return;
		}
		XAttribute* pxAttr = pxExtent->GetAttribute("xmin");
		if(pxAttr)
		{
			m_extent.m_xmin = atof(pxAttr->GetContent());
		}
		pxAttr = pxExtent->GetAttribute("xmax");
		if(pxAttr)
		{
			m_extent.m_xmax = atof(pxAttr->GetContent());
		}
		pxAttr = pxExtent->GetAttribute("ymin");
		if(pxAttr)
		{
			m_extent.m_ymin = atof(pxAttr->GetContent());
		}
		pxAttr = pxExtent->GetAttribute("ymax");
		if(pxAttr)
		{
			m_extent.m_ymax = atof(pxAttr->GetContent());
		}
	}

	void SaveMapRequest::SetViewer(XElement* pxViewer)
	{
		if(pxViewer==NULL)
		{
			return;
		}
		XAttribute* pxAttr = pxViewer->GetAttribute("xmin");
		if(pxAttr)
		{
			m_viewer.m_xmin = atof(pxAttr->GetContent());
		}
		pxAttr = pxViewer->GetAttribute("xmax");
		if(pxAttr)
		{
			m_viewer.m_xmax = atof(pxAttr->GetContent());
		}
		pxAttr = pxViewer->GetAttribute("ymin");
		if(pxAttr)
		{
			m_viewer.m_ymin = atof(pxAttr->GetContent());
		}
		pxAttr = pxViewer->GetAttribute("ymax");
		if(pxAttr)
		{
			m_viewer.m_ymax = atof(pxAttr->GetContent());
		}
	}

	void SaveMapRequest::ParserLayers(XNode* pxLayers)
	{
		if(pxLayers==NULL)
		{
			return;
		}

		XAttribute* pxAttr = NULL;
		XElement* pxNode = NULL;
		XNodeSet* pxNodeSet = pxLayers->GetChildren();

		m_layer_count = pxNodeSet->Count();
		if(m_layers!=NULL)
		{
			free(m_layers);
			m_layers = NULL;
		}
		m_layers = (layer_info_t*)malloc(sizeof(layer_info_t)*m_layer_count);
		memset(m_layers, 0, sizeof(layer_info_t)*m_layer_count);
		for(int i=0; i<m_layer_count; i++)
		{
			pxNode = static_cast<XElement*>(pxNodeSet->Item(i));
			pxAttr = pxNode->GetAttribute("id");
			if(pxAttr!=NULL)
			{
				strcpy(m_layers[i].id, pxAttr->GetValue());
			}
			pxAttr = pxNode->GetAttribute("name");
			if(pxAttr!=NULL)
			{
				strcpy(m_layers[i].name, pxAttr->GetValue());
			}
			pxAttr = pxNode->GetAttribute("visible");
			if(pxAttr!=NULL)
			{
				m_layers[i].visible = g_stricmp(pxAttr->GetValue(),"false");
			}
		}

		pxNodeSet->Release();
	}

	const char*	SaveMapRequest::GetSRID()
	{
		return m_srid.c_str();
	}
	
	void SaveMapRequest::SetSRID(const char* srid)
	{
		if(srid==NULL)
		{
			m_srid.clear();
		}
		else
		{
			m_srid = srid;
		}
	}

	g_uint SaveMapRequest::GetLayerCount()
	{
		return m_layer_count;
	}

	const char*	SaveMapRequest::GetLayerID(g_uint i)
	{
		if(i>=m_layer_count)
		{
			return NULL;
		}
		return m_layers[i].id;
	}

	const char*	SaveMapRequest::GetLayerName(g_uint i)
	{
		if(i>=m_layer_count)
		{
			return NULL;
		}
		return m_layers[i].name;
	}

	bool SaveMapRequest::IsLayerVisible(g_uint i)
	{
		if(i>=m_layer_count)
		{
			return false;
		}
		return m_layers[i].visible;
	}
}