#ifndef __AUGE_IMS_CREATE_MAP_HANDLER_H__
#define __AUGE_IMS_CREATE_MAP_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class CreateMapHandler : public WebHandler
	{
	public:
		CreateMapHandler();
		virtual ~CreateMapHandler();
	public:
		virtual const char*		GetName();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc);
		virtual WebResponse*	Execute(WebRequest* pWebRequest);
	};
}

#endif //__AUGE_IMS_CREATE_MAP_HANDLER_H__