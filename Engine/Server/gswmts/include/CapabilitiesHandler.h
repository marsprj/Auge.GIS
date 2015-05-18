#ifndef __AUGE_WMTS_CAPABILITIES_HANDLER_H__
#define __AUGE_WMTS_CAPABILITIES_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class Map;
	class WebContext;
	class CapabilitiesRequest;
	class CapabilitiesResponse;
	class TileWorkspace;

	class CapabilitiesHandler : public WebHandler
	{
	public:
		CapabilitiesHandler();
		virtual ~CapabilitiesHandler();
	public:
		virtual const char*		GetName();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext);

		

	private:
		CapabilitiesResponse*	WriteCapabilities_1_0_0(CapabilitiesRequest* pRequest, WebContext* pWebContext, const char* sourceName);
		CapabilitiesResponse*	WriteCapabilities_1_3_0(CapabilitiesRequest* pRequest, WebContext* pWebContext, const char* sourceName);

		void	SetRooteNode_1_0_0(XElement* pxRoot, const char* version);
		void	AddServiceIdentificationNode_1_0_0(XElement* pxParent);
		void	AddServiceProviderNode_1_0_0(XElement* pxParent);
		void	AddOperationsMetadataNode_1_0_0(XElement* pxParent, const char* wmts_xlink);
		void	AddTileLayerNode_1_0_0(XElement* pxParent,const char* sourceName);

		void	AddTileMatrixSetNode_1_0_0(XElement* pxParent);
		void	AddTileMatrixSet_GoogleCRS84Quad_1_0_0(XElement* pxParent);

	private:
		CapabilitiesRequest*	m_pRequest;
	};
}

#endif //__AUGE_WMTS_CAPABILITIES_HANDLER_H__

