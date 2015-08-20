#ifndef __AUGE_GPS_CONVEXHULL_HANDLER_H__
#define __AUGE_GPS_CONVEXHULL_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class WebContext;
	class ConvexHullRequest;
	class ConvexHullResponse;
	class Feature;
	class FeatureClass;
	class FeatureWorkspace;

	class ConvexHullHandler : public WebHandler
	{
	public:
		ConvexHullHandler();
		virtual ~ConvexHullHandler();
	public:
		virtual const char*		GetName();

		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);

	protected:
		FeatureClass*			GetFeatureClass(ConvexHullRequest* pWebRequest, WebContext* pWebContext, User* pUser);
		FeatureClass*			GetFeatureClassByMap(ConvexHullRequest* pWebRequest, WebContext* pWebContext, User* pUser);
		FeatureClass*			GetFeatureClassBySource(ConvexHullRequest* pWebRequest, WebContext* pWebContext, User* pUser);

		WebResponse*			Execute_2_Client(FeatureClass* pinFeatureClass, ConvexHullRequest* pRequest, WebContext* pWebContext);
		WebResponse*			Execute_2_Datasource(FeatureClass* pinFeatureClass, ConvexHullRequest* pRequest, WebContext* pWebContext, User* pUser);

		FeatureClass*			CreateOutputFeatureClass(FeatureWorkspace* pFeatureWorkspace, const char* className, g_uint srid);
		Geometry*				CreateConvexHull(Geometry* pGeometry);

	};
}

#endif //__AUGE_GPS_CONVEXHULL_HANDLER_H__
