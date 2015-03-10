#ifndef __AUGE_WMS_GETMAP_HANDLER_H__
#define __AUGE_WMS_GETMAP_HANDLER_H__

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
	class GetMapRequest;
	class GetMapResponse;

	class GetMapHandler : public WebHandler
	{
	public:
		GetMapHandler();
		virtual ~GetMapHandler();
	public:
		virtual const char*		GetName();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc);
		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap);
	};
}

#endif //__AUGE_WMS_GETMAP_HANDLER_H__

