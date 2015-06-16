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
	class FeatureClass;

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

		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext);

	protected:
		FeatureClass*			GetFeatureClass(BufferRequest* pWebRequest, WebContext* pWebContext);
		FeatureClass*			GetFeatureClassByMap(BufferRequest* pWebRequest, WebContext* pWebContext);
		FeatureClass*			GetFeatureClassBySource(BufferRequest* pWebRequest, WebContext* pWebContext);

	};
}

#endif //__AUGE_GPS_BUFFER_HANDLER_H__
