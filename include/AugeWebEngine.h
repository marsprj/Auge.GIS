#ifndef __AUGE_WEB_ENGINE_H__
#define __AUGE_WEB_ENGINE_H__

#ifdef WIN32
#	ifdef AUGE_WEB_ENGINE_EXPORTS
#		define AUGE_WEB_ENGINE_API __declspec(dllexport)
#	else
#		define AUGE_WEB_ENGINE_API __declspec(dllimport)
#	endif
#else
#	define AUGE_WEB_ENGINE_API
#endif

#include "AugeObject.h"
#include "AugeType.h"
#include <rude/cgi.h>

namespace auge
{
	class Map;
	class WebRequest;
	class WebResponse;
	class WebWriter;
	class WebContext;
	class XDocument;

	class WebRequest : public GObject
	{
	protected:
		WebRequest(){}
		virtual ~WebRequest(){}
	public:
		virtual const char*		GetEngine() = 0;
		virtual const char*		GetVersion() = 0;
		virtual const char*		GetRequest() = 0;
		virtual const char*		GetEncoding() = 0;
		/*virtual const char*		GetService() = 0;*/
	};

	class WebResponse : public GObject
	{
	protected:
		WebResponse(){}
		virtual ~WebResponse(){}

	public:
		//virtual	RESULTCODE		Write() = 0;
		virtual	RESULTCODE		Write(WebWriter* pWriter) = 0;
	};
	
	class WebHandler : public GObject
	{
	protected:
		WebHandler(){}
		virtual ~WebHandler(){}
	public:
		virtual const char*		GetName() = 0;
		//virtual const char*		GetDescription() = 0;
		virtual WebRequest*		ParseRequest(rude::CGI& cgi) = 0;
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName) = 0;
		//virtual WebRequest*		ParseRequest(rude::CGI& cgi, WebContext* pWebContext=NULL, Map* pMap=NULL) = 0;
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName) = 0;
		//virtual WebRequest*		ParseRequest(XDocument* pxDoc, WebContext* pWebContext=NULL, Map* pMap=NULL) = 0;

		virtual WebResponse*	Execute(WebRequest* pWebRequest) = 0;
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext) = 0;
		//virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap) = 0;
	};

	class WebEngine
	{
	protected:
		WebEngine(){}
		virtual ~WebEngine(){}

	public:
		virtual WebResponse*	Execute(WebRequest* pWebRequest) = 0;
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext) = 0;
		//virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap) = 0;

		virtual WebRequest*		ParseRequest(const char* url) = 0;
		virtual WebRequest*		ParseRequest(rude::CGI& cgi) = 0;
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName) = 0;

		//virtual WebRequest*		ParseRequest(rude::CGI& cgi, WebContext* pWebContext=NULL, Map* pMap=NULL) = 0;
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName) = 0;

	public:
		virtual const char*		GetID() = 0;
		virtual const char*		GetType() = 0;
		virtual const char*		GetDescription() = 0;

		virtual void*			GetHandler() = 0;
		virtual void			SetHandler(void* handler) = 0;

		virtual const char*		GetLibraryPath() = 0;
	};

	extern "C"
	{
		AUGE_WEB_ENGINE_API WebEngine* augeGetWebEngineInstance();
	}
}

#endif //__AUGE_WEB_ENGINE_H__
