#ifndef __AUGE_RDS_ADD_RASTER_HANDLER_H__
#define __AUGE_RDS_ADD_RASTER_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class AddRasterRequest;

	class AddRasterHandler : public WebHandler
	{
	public:
		AddRasterHandler();
		virtual ~AddRasterHandler();
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

#endif //__AUGE_RDS_ADD_RASTER_HANDLER_H__
