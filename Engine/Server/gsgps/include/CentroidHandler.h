#ifndef __AUGE_GPS_CENTROID_HANDLER_H__
#define __AUGE_GPS_CENTROID_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class WebContext;
	class CentroidRequest;
	class CentroidResponse;
	class FeatureClass;

	class CentroidHandler : public WebHandler
	{
	public:
		CentroidHandler();
		virtual ~CentroidHandler();
	public:
		virtual const char*		GetName();

		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);

	protected:
		FeatureClass*			GetFeatureClass(CentroidRequest* pWebRequest, WebContext* pWebContext, User* pUser);
		FeatureClass*			GetFeatureClassByMap(CentroidRequest* pWebRequest, WebContext* pWebContext, User* pUser);
		FeatureClass*			GetFeatureClassBySource(CentroidRequest* pWebRequest, WebContext* pWebContext, User* pUser);
		
	};
}

#endif //__AUGE_GPS_CENTROID_HANDLER_H__
