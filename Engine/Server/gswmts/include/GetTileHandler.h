#ifndef __AUGE_WMTS_GET_TILE_HANDLER_H__
#define __AUGE_WMTS_GET_TILE_HANDLER_H__

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
	class GetTileRequest;
	class GetTileResponse;
	class FeatureLayer;
	class Style;

	class GetTileHandler : public WebHandler
	{
	public:
		GetTileHandler();
		virtual ~GetTileHandler();
	public:
		virtual const char*		GetName();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext);

	};
}

#endif //__AUGE_WMTS_GET_TILE_HANDLER_H__

