#ifndef __AUGE_WEB_FEATURE_ENGINE_H__
#define __AUGE_WEB_FEATURE_ENGINE_H__

#include "AugeWebEngine.h"
#include <string>
#include <vector>

namespace auge
{
	//class GetMapRequest;
	//class GetCapabilitiesRequest;

	class WFeatureEngine : public WebEngine
	{
		friend class CapabilitiesHandler;

	public:
		WFeatureEngine();
		virtual ~WFeatureEngine();

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


	private:
		void			CleanupHandlers();
		WebHandler*		GetHandler(const char* name);

	protected:
		void*	m_handler;
		std::vector<WebHandler*>	m_handlers;
	};
}

#endif //__AUGE_WEB_FEATURE_ENGINE_H__
