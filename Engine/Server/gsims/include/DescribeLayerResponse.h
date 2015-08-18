#ifndef __AUGE_IMS_DESCRIBE_MAP_RESPONSE_H__
#define __AUGE_IMS_DESCRIBE_MAP_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class Map;
	class Layer;
	class EnumMap;
	class FeatureClass;
	class FeatureLayer;
	class DescribeLayerRequest;
	class Style;

	class DescribeLayerResponse : public WebResponse
	{
	public:
		DescribeLayerResponse(DescribeLayerRequest* pRequest);
		virtual ~DescribeLayerResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
		
	public:
		void	SetLayer(Layer* pLayer);
		void	SetWebContext(WebContext* pWebContext);

	private:
		void	AddMapNode(XElement* pxRoot, Map* pMap); 
		void	AddMapNode(XDocument* pxDoc, Map* pMap);
		void	AddLayerNode(XElement* pxLayers, Layer* pLayer);
		void	AddBoundingBoxNode(XElement* pxRoot, GEnvelope& extent);

		void	WriteLayer(XDocument* pxDoc);

		void	AddFeatureNode(XElement* pxLayer, FeatureClass* pFeatureClass);
		void	AddLayerGeographicBoundingNode(XElement* pxLayer, GEnvelope& extent);
		void	AddLayerBoundingNode(XElement* pxParent, GEnvelope& extent, int srid);
		void	AddViewerNode(XElement* pxParent, GEnvelope& extent, int srid);
		void	AddStyleNode(XElement* pxParent, Style* pStyle);

		void	AddWebURLNode(XElement* pxLayer, const char* url);
		void	AddThumbnailNode(XElement* pxThumbnail, Map* pMap);

		void	Cleanup();
		
	private:
		DescribeLayerRequest	*m_pRequest;
		WebContext	*m_pWebContext;
		Layer		*m_pLayer;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
