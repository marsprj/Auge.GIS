#ifndef __AUGE_WEB_POI_ENGINE_H__
#define __AUGE_WEB_POI_ENGINE_H__

#include "AugeWebEngine.h"
#include <string>
#include <vector>

namespace auge
{
	class WPoiEngine : public WebEngine
	{
	public:
		WPoiEngine();
		virtual ~WPoiEngine();

	public:
		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);

		virtual WebRequest*		ParseRequest(const char* url);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual RESULTCODE		Initialize(void *pParam);

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

#endif //__AUGE_WEB_POI_ENGINE_H__
