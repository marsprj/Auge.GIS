#ifndef __AUGE_IMS_GET_STYLE_HANDLER_H__
#define __AUGE_IMS_GET_STYLE_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GetStyleHandler : public WebHandler
	{
	public:
		GetStyleHandler();
		virtual ~GetStyleHandler();
	public:
		virtual const char*		GetName();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc);
		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap);
	};
}

#endif //__AUGE_IMS_GET_STYLE_HANDLER_H__
