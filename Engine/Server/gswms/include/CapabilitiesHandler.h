#ifndef __AUGE_WMS_CAPABILITIES_HANDLER_H__
#define __AUGE_WMS_CAPABILITIES_HANDLER_H__

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

	class CapabilitiesHandler : public WebHandler
	{
	public:
		CapabilitiesHandler();
		virtual ~CapabilitiesHandler();
	public:
		virtual const char*		GetName();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc);
		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap);
	private:
		CapabilitiesResponse*	WriteCapabilities_1_0_0(CapabilitiesRequest* pRequest, WebContext* pWebContext, Map* pMap);
		CapabilitiesResponse*	WriteCapabilities_1_3_0(CapabilitiesRequest* pRequest, WebContext* pWebContext, Map* pMap);
	};
}

#endif //__AUGE_WMS_CAPABILITIES_HANDLER_H__

