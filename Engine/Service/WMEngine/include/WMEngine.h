#ifndef __AUGE_WEB_MAP_ENGINE_H__
#define __AUGE_WEB_MAP_ENGINE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class GetMapRequest;
	class GetCapabilitiesRequest;

	class WebMapEngine : public WebEngine
	{
	public:
		WebMapEngine();
		virtual ~WebMapEngine();

	public:
		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap);

		virtual WebRequest*		ParseRequest(const char* url);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc);

	public:
		virtual const char*		GetID();
		virtual const char*		GetType();
		virtual const char*		GetDescription();

		virtual void*			GetHandler();
		virtual void			SetHandler(void* handler);

		virtual const char*		GetLibraryPath();

	protected:
		bool					Support(const char* request);
		WebResponse*			GetMap(GetMapRequest* pRequest, WebContext* pWebContext, Map* pMap);

		WebResponse*			GetCapabilities(GetCapabilitiesRequest* pRequest, WebContext* pWebContext, Map* pMap);
		bool					WriteCapabilities(const char* version,WebContext* pWebContext, Map* pMap);
		bool					WriteCapabilities_1_3_0(WebContext* pWebContext, Map* pMap);

	protected:
		void*		m_handler;
		std::string	m_requests;
	};
}

#endif //__AUGE_WEB_MAP_ENGINE_H__
