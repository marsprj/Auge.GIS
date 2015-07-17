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
	class WmtsCapabilitiesRequest;
	class WmtsCapabilitiesResponse;
	class TileWorkspace;
	class WTileEngine;

	class WmtsCapabilitiesHandler : public WebHandler
	{
	public:
		WmtsCapabilitiesHandler();
		virtual ~WmtsCapabilitiesHandler();
	public:
		virtual const char*		GetName();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);

	public:
		void	SetEngine(WTileEngine *pEngine);

	private:
		WmtsCapabilitiesResponse*	WriteCapabilities_1_0_0(WmtsCapabilitiesRequest* pRequest, WebContext* pWebContext, const char* sourceName, User* pUser);
		WmtsCapabilitiesResponse*	WriteCapabilities_1_3_0(WmtsCapabilitiesRequest* pRequest, WebContext* pWebContext, const char* sourceName, User* pUser);

		void	SetRooteNode_1_0_0(XElement* pxRoot, const char* version);
		void	AddServiceIdentificationNode_1_0_0(XElement* pxParent);
		void	AddServiceProviderNode_1_0_0(XElement* pxParent);
		void	AddOperationsMetadataNode_1_0_0(XElement* pxParent, const char* wmts_xlink);
		void	AddTileLayerNode_1_0_0(XElement* pxParent,const char* sourceName, User* pUser);
		void	AddTileLayersNode_1_0_0(XElement* pxParent, const char* sourceName, User* pUser);

		void	AddTileMatrixSetNode_1_0_0(XElement* pxParent);
		void	AddTileMatrixSet_GoogleCRS84Quad_1_0_0(XElement* pxParent);
		void	AddTileMatrixSet_PGIS_1_0_0(XElement* pxParent);

	private:
		WmtsCapabilitiesRequest*	m_pRequest;
		WTileEngine*				m_pEngine;
	};
}

#endif //__AUGE_WMTS_CAPABILITIES_HANDLER_H__

