#ifndef __AUGE_IMS_REGISTER_LAYER_RESPONSE_H__
#define __AUGE_IMS_REGISTER_LAYER_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class Map;
	class Layer;
	class EnumMap;
	class FeatureLayer;
	class RegisterLayerRequest;
	class Style;

	class RegisterLayerResponse : public WebResponse
	{
	public:
		RegisterLayerResponse(RegisterLayerRequest* pRequest);
		virtual ~RegisterLayerResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
		
	public:
		void	SetLayer(Layer* pLayer);		
		void	SetWebContext(WebContext* pWebContext);

	private:
		void	WriteLayer(XDocument* pxDoc);
		void	AddLayerNode(XElement* pxRoot, FeatureLayer* pLayer);

		//void	AddMapNode(XElement* pxRoot, Map* pMap); 
		//void	AddMapNode(XDocument* pxDoc, Map* pMap);
		void	AddLayerNode(XElement* pxLayers, Layer* pLayer);
		void	AddBoundingBoxNode(XElement* pxRoot, GEnvelope& extent);

		

		void	AddLayerGeomTypeNode(XElement* pxLayer, FeatureLayer* pFeatureLayer);
		void	AddLayerGeographicBoundingNode(XElement* pxLayer, GEnvelope& extent);
		void	AddLayerBoundingNode(XElement* pxParent, GEnvelope& extent, int srid);
		void	AddViewerNode(XElement* pxParent, GEnvelope& extent, int srid);

		void	AddWebURLNode(XElement* pxLayer, const char* url);
		//void	AddThumbnailNode(XElement* pxThumbnail, Map* pMap);

		void	Cleanup();
		
	private:
		RegisterLayerRequest	*m_pRequest;
		WebContext	*m_pWebContext;
		Layer		*m_pLayer;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
