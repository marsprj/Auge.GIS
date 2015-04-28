#ifndef __AUGE_IMS_GET_MAP_RESPONSE_H__
#define __AUGE_IMS_GET_MAP_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class Map;
	class Layer;
	class EnumMap;
	class FeatureLayer;
	class GetMapRequest;
	class Style;

	class GetMapResponse : public WebResponse
	{
	public:
		GetMapResponse(GetMapRequest* pRequest);
		virtual ~GetMapResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
		
	public:
		void	SetMap(Map* pMap);
		void	SetMaps(EnumMap* pMaps);
		void	SetWebContext(WebContext* pWebContext);

	private:
		void	AddMapNode(XElement* pxRoot, Map* pMap); 
		void	AddMapNode(XDocument* pxDoc, Map* pMap);
		void	AddLayerNode(XElement* pxLayers, Layer* pLayer);
		void	AddBoundingBoxNode(XElement* pxRoot, GEnvelope& extent);

		void	WriteMap(XDocument* pxDoc);
		void	WriteMaps(XDocument* pxDoc);

		void	AddLayerGeomTypeNode(XElement* pxLayer, FeatureLayer* pFeatureLayer);
		void	AddLayerGeographicBoundingNode(XElement* pxLayer, GEnvelope& extent);
		void	AddLayerBoundingNode(XElement* pxParent, GEnvelope& extent, int srid);
		void	AddStyleNode(XElement* pxParent, Style* pStyle);

		void	Cleanup();
		
	private:
		GetMapRequest	*m_pRequest;
		WebContext	*m_pWebContext;
		EnumMap		*m_pMaps;
		Map			*m_pMap;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
