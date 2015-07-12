#ifndef __AUGE_GPS_FEATURE_PROJECT_HANDLER_H__
#define __AUGE_GPS_FEATURE_PROJECT_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class WebContext;
	class FeatureProjectRequest;
	class FeatureProjectResponse;
	class FeatureClass;

	class FeatureProjectHandler : public WebHandler
	{
	public:
		FeatureProjectHandler();
		virtual ~FeatureProjectHandler();
	public:
		virtual const char*		GetName();

		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext);
	};
}

#endif //__AUGE_GPS_FEATURE_PROJECT_HANDLER_H__