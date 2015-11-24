#ifndef __AUGE_POI_GET_POI__HANDLER_H__
#define __AUGE_POI_GET_POI__HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class GetPoiRequest;

	class GetPoiHandler : public WebHandler
	{
	public:
		GetPoiHandler();
		virtual ~GetPoiHandler();
	public:
		virtual const char*		GetName();
		virtual const char*		GetDescription();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);

	private:
		WebResponse*			SearchByName(GetPoiRequest* pRequest, WebContext* pWebContext, User* pUser);
		WebResponse*			SearchByNames(GetPoiRequest* pRequest, WebContext* pWebContext, User* pUser);
	};
}

#endif //__AUGE_POI_GET_POI__HANDLER_H__
