#ifndef __AUGE_GPS_GET_SPATIAL_REFERENCE_HANDLER_H__
#define __AUGE_GPS_GET_SPATIAL_REFERENCE_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "GProcessHandler.h"
#include <rude/cgi.h>

#include "sqlite3.h"

namespace auge
{
	class GetSpatialReferenceRequest;

	class GetSpatialReferenceHandler : public GProcessHandler
	{
	public:
		GetSpatialReferenceHandler();
		virtual ~GetSpatialReferenceHandler();
	public:
		virtual const char*		GetName();
		virtual const char*		GetDescription();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);

	private:
		sqlite3*		OpenBase();
		void			CloseBase();
		sqlite3_stmt*	Query(GetSpatialReferenceRequest* pRequest);
	private:
		sqlite3* m_psqlite;
	};
}

#endif //__AUGE_GPS_GET_SPATIAL_REFERENCE_HANDLER_H__

