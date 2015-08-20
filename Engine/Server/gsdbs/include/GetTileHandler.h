#ifndef __AUGE_DBS_GET_TILE_HANDLER_H__
#define __AUGE_DBS_GET_TILE_HANDLER_H__

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
		virtual const char*		GetDescription();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);

	};
}

#endif //__AUGE_DBS_GET_TILE_HANDLER_H__

