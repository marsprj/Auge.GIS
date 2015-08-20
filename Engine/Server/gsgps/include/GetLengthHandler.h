#ifndef __AUGE_GPS_GET_LENGTH_HANDLER_H__
#define __AUGE_GPS_GET_LENGTH_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class WebContext;
	class GetLengthRequest;
	class GetLengthResponse;
	class FeatureClass;
	class FeatureWorkspace;

	class GetLengthHandler : public WebHandler
	{
	public:
		GetLengthHandler();
		virtual ~GetLengthHandler();
	public:
		virtual const char*		GetName();
		virtual const char*		GetDescription();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);

	protected:
		FeatureClass*			GetFeatureClass(GetLengthRequest* pWebRequest, WebContext* pWebContext, User* pUser);
		FeatureClass*			GetFeatureClassByMap(GetLengthRequest* pWebRequest, WebContext* pWebContext, User* pUser);
		FeatureClass*			GetFeatureClassBySource(GetLengthRequest* pWebRequest, WebContext* pWebContext, User* pUser);

		WebResponse*			Execute_2_Client(FeatureClass* pinFeatureClass, GetLengthRequest* pRequest, WebContext* pWebContext);
		WebResponse*			Execute_2_Datasource(FeatureClass* pinFeatureClass, GetLengthRequest* pRequest, WebContext* pWebContext, User* pUser);

		FeatureClass*			CreateOutputFeatureClass(FeatureWorkspace* pFeatureWorkspace, const char* className, g_uint srid);

	};
}

#endif //__AUGE_GPS_GET_LENGTH_HANDLER_H__
