#ifndef __AUGE_IMS_REMOVE_STYLE_HANDLER_H__
#define __AUGE_IMS_REMOVE_STYLE_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class RemoveStyleHandler : public WebHandler
	{
	public:
		RemoveStyleHandler();
		virtual ~RemoveStyleHandler();
	public:
		virtual const char*		GetName();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext);
	};
}

#endif //__AUGE_IMS_REMOVE_STYLE_HANDLER_H__
