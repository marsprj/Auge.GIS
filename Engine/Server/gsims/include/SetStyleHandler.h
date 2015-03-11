#ifndef __AUGE_IMS_LAYER_SETSTYLE_HANDLER_H__
#define __AUGE_IMS_LAYER_SETSTYLE_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class SetStyleHandler : public WebHandler
	{
	public:
		SetStyleHandler();
		virtual ~SetStyleHandler();
	public:
		virtual const char*		GetName();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, WebContext* pWebContext=NULL, Map* pMap=NULL);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, WebContext* pWebContext=NULL, Map* pMap=NULL);
		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap);
	};
}

#endif //__AUGE_IMS_LAYER_SETSTYLE_HANDLER_H__
