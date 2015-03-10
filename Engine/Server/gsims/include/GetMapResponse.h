#ifndef __AUGE_IMS_GET_MAP_RESPONSE_H__
#define __AUGE_IMS_GET_MAP_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class Map;
	class Layer;
	class GetMapRequest;

	class GetMapResponse : public WebResponse
	{
	public:
		GetMapResponse(GetMapRequest* pRequest);
		virtual ~GetMapResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	private:
		void	AddMapNode(XElement* pxRoot, Map* pMap);
		void	AddLayerNode(XElement* pxLayers, Layer* pLayer);
		void	AddBoundingBoxNode(XElement* pxRoot, GEnvelope& extent);
		
	private:
		GetMapRequest *m_pRequest;	
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
