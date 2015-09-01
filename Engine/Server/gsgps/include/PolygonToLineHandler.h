#ifndef __AUGE_GPS_POLYGON_TO_LINE_HANDLER_H__
#define __AUGE_GPS_POLYGON_TO_LINE_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include "GProcessHandler.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class WebContext;
	class PolygonToLineRequest;
	class PolygonToLineResponse;
	class FeatureClass;

	class PolygonToLineHandler : public GProcessHandler
	{
	public:
		PolygonToLineHandler();
		virtual ~PolygonToLineHandler();
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

#endif //__AUGE_GPS_POLYGON_TO_LINE_HANDLER_H__
