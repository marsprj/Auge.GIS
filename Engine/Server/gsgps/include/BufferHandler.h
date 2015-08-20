#ifndef __AUGE_GPS_BUFFER_HANDLER_H__
#define __AUGE_GPS_BUFFER_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class WebContext;
	class BufferRequest;
	class BufferResponse;
	class Feature;
	class FeatureClass;
	class FeatureWorkspace;

	class BufferHandler : public WebHandler
	{
	public:
		BufferHandler();
		virtual ~BufferHandler();
	public:
		virtual const char*		GetName();

		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);

	protected:
		FeatureClass*			GetFeatureClass(BufferRequest* pWebRequest, WebContext* pWebContext, User* pUser);
		FeatureClass*			GetFeatureClassByMap(BufferRequest* pWebRequest, WebContext* pWebContext, User* pUser);
		FeatureClass*			GetFeatureClassBySource(BufferRequest* pWebRequest, WebContext* pWebContext, User* pUser);

		FeatureClass*			CreateOutputFeatureClass(FeatureWorkspace* pFeatureWorkspace, const char* className, g_uint srid);

		Geometry*				CreateBuffer(Feature* pFeature, BufferRequest* pRequest);
		double					GetDistance(GValue* pValue);

		WebResponse*			Execute_2_Client(FeatureClass* pinFeatureClass, BufferRequest* pRequest, WebContext* pWebContext);
		WebResponse*			Execute_2_Datasource(FeatureClass* pinFeatureClass, BufferRequest* pRequest, WebContext* pWebContext, User* pUser);

	};
}

#endif //__AUGE_GPS_BUFFER_HANDLER_H__
