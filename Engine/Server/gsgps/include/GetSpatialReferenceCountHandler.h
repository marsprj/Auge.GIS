#ifndef __AUGE_GPS_GET_SPATIAL_REFERENCE_COUNT_HANDLER_H__
#define __AUGE_GPS_GET_SPATIAL_REFERENCE_COUNT_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "GProcessHandler.h"
#include <rude/cgi.h>

#include "sqlite3.h"

namespace auge
{
	class GetSpatialReferenceCountRequest;

	class GetSpatialReferenceCountHandler : public GProcessHandler
	{
	public:
		GetSpatialReferenceCountHandler();
		virtual ~GetSpatialReferenceCountHandler();
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

#endif //__AUGE_GPS_GET_SPATIAL_REFERENCE_COUNT_HANDLER_H__

