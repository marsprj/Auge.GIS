#ifndef __AUGE_WEB_MANAGER_ENGINE_H__
#define __AUGE_WEB_MANAGER_ENGINE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class GetCapabilitiesRequest;
	class GetCapabilitiesResponse;

	class WebManagerEngine : public WebEngine
	{
	public:
		WebManagerEngine();
		virtual ~WebManagerEngine();

	public:
		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap);

		virtual WebRequest*		ParseRequest(const char* url);
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

		WebResponse*			GetCapabilities(GetCapabilitiesRequest* pRequest);

	protected:
		void*		m_handler;
		std::string	m_requests;
	};
}

#endif //__AUGE_WEB_MANAGER_ENGINE_H__
