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
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, WebContext* pWebContext=NULL, Map* pMap=NULL);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, WebContext* pWebContext=NULL, Map* pMap=NULL);
		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap);

	private:
		WebResponse* DrawFeature(FeatureClass* pFeatureClass, GetPreviewRequest* pRequest);
	};
}

#endif //__AUGE_IMS_GET_PREVIEW_HANDLER_H__
