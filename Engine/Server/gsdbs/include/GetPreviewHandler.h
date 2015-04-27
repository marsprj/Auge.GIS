#ifndef __AUGE_IMS_GET_PREVIEW_HANDLER_H__
#define __AUGE_IMS_GET_PREVIEW_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GetPreviewRequest;
	class FeatureClass;

	class GetPreviewHandler : public WebHandler
	{
	public:
		GetPreviewHandler();
		virtual ~GetPreviewHandler();
	public:
		virtual const char*		GetName();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext);

	private:
		WebResponse* DrawFeature(FeatureClass* pFeatureClass, GetPreviewRequest* pRequest);
	};
}

#endif //__AUGE_IMS_GET_PREVIEW_HANDLER_H__
