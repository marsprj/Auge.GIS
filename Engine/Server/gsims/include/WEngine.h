#ifndef __AUGE_WEB_MANAGER_ENGINE_H__
#define __AUGE_WEB_MANAGER_ENGINE_H__

#include "AugeWebEngine.h"
#include <string>
#include <vector>

namespace auge
{
	class CapabilitiesRequest;
	class CreateServiceRequest;
	class RemoveServiceRequest;
	class GetServiceRequest;
	class RegisterHandlerRequest;
	class RegisterDataSourceRequest;

	class WEngine : public WebEngine
	{
		friend class CapabilitiesResponse;

	public:
		WEngine();
		virtual ~WEngine();

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

		virtual const char*		GetLibraryPath();

		virtual void*			GetHandler();
		virtual void			SetHandler(void* handler);

	protected:
		bool					Support(const char* request);

		WebResponse*			GetCapabilities(CapabilitiesRequest* pRequest);
		WebResponse*			CreateService(CreateServiceRequest* pRequest);
		WebResponse*			RemoveService(RemoveServiceRequest* pRequest);
		WebResponse*			GetService(GetServiceRequest* pRequest);
		//WebResponse*			RegisterHandler(RegisterHandlerRequest* pRequest);

		WebResponse*			RegisterDataSource(RegisterDataSourceRequest* pRequest);

	private:
		void			CleanupHandlers();
		WebHandler*		GetHandler(const char* name);


	protected:
		void*	m_handler;
		std::vector<WebHandler*>	m_handlers;
	};
}

#endif //__AUGE_WEB_MANAGER_ENGINE_H__
