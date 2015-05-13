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

		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext);

	protected:
		FeatureClass*			GetFeatureClass(CentroidRequest* pWebRequest, WebContext* pWebContext);
		FeatureClass*			GetFeatureClassByMap(CentroidRequest* pWebRequest, WebContext* pWebContext);
		FeatureClass*			GetFeatureClassBySource(CentroidRequest* pWebRequest, WebContext* pWebContext);
		

	protected:
		void*	m_handler;
		std::vector<WebHandler*>	m_handlers;
	};
}

#endif //__AUGE_GPS_CENTROID_HANDLER_H__
