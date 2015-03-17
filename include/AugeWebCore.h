#ifndef __AUGE_WEB_CORE_H__
#define __AUGE_WEB_CORE_H__

#ifdef WIN32
#	ifdef AUGE_WEB_CORE_EXPORTS
#		define AUGE_WEB_CORE_API __declspec(dllexport)
#	else
#		define AUGE_WEB_CORE_API __declspec(dllimport)
#	endif
#else
#	define AUGE_WEB_CORE_API
#endif

#include "AugeWebEngine.h"
//#include "AugeData.h"

namespace auge
{
	class GConnection;

	class WebSuccessResponse : public WebResponse
	{
	protected:
		WebSuccessResponse(){}
		virtual ~WebSuccessResponse(){}
	public:
		virtual	RESULTCODE		SetRequest(const char* request) = 0;
		virtual	const char*		GetRequest() = 0;
	};

	class WebExceptionResponse : public WebResponse
	{
	protected:
		WebExceptionResponse(){}
		virtual ~WebExceptionResponse(){}
	public:
		virtual	RESULTCODE		SetMessage(const char* message) = 0;
		virtual	const char*		GetMessage() = 0;
	};

	class WebWriter : public GObject
	{
	protected:
		WebWriter(){}
		virtual ~WebWriter(){}
	public:
		virtual RESULTCODE		WriteHead(const char* content_type) = 0;
		virtual void			WriteTail() = 0;
		virtual RESULTCODE		Write(g_uchar* data, g_uint size) = 0;
	};
	
	class WebContext : public GObject
	{
	protected:
		WebContext(){}
		virtual ~WebContext(){}
	public:
		virtual	const char*		GetService() = 0;
		virtual void			SetService(const char* name) = 0;
		virtual	const char*		GetURI() = 0;
		virtual void			SetURI(const char* uri) = 0;

		virtual const char*		GetCachePath() = 0;
		virtual const char*		GetCacheMapPath() = 0;
		virtual const char*		GetCacheProtocolPath() = 0;

		virtual const char*		GetServer() = 0;
		virtual const char*		GetPort() = 0;
		virtual const char*		GetScrpitName() = 0;
		virtual const char*		GetServerSoftware() = 0;
		virtual const char*		GetHttpUserAgent() = 0;
		virtual const char*		GetRequestMethod() = 0;
		virtual const char*		GetQueryString() = 0;

		virtual const char*		GetResponseEncoding() = 0;

	public:
		virtual GConnection*	GetConnection() = 0;

	public:
		virtual	RESULTCODE		Initialize(const char* config_path=NULL) = 0;
		virtual RESULTCODE		Unload() = 0;
	};

	extern "C"
	{
		AUGE_WEB_CORE_API WebWriter*			augeCreateWebWriter();
		AUGE_WEB_CORE_API WebSuccessResponse*	augeCreateWebSuccessResponse();
		AUGE_WEB_CORE_API WebExceptionResponse*	augeCreateWebExceptionResponse();
		AUGE_WEB_CORE_API WebContext*			augeGetWebContextInstance();
	}

}

//==============================================================================
//------------------------------------------------------------------------------
// 连接所需动态库
//------------------------------------------------------------------------------
#ifdef WIN32
#	ifndef AUGE_WEB_CORE_EXPORTS
#		define AUGE_WEB_CORE_NAME "gscore"
#		if defined(_DEBUG)
#			if defined(_UNICODE)
//#				define _AUGE_FILTER_CONFIGURATION "UD"
#				define _AUGE_FILTER_CONFIGURATION "D"
#			else
#				define _AUGE_FILTER_CONFIGURATION "D"
#			endif // _UNICODE
#		else
#			if defined(_UNICODE)
#				define _AUGE_FILTER_CONFIGURATION "U"
#			else
#				define _AUGE_FILTER_CONFIGURATION ""
#			endif // _UNICODE
#		endif // _DEBUG
#		pragma comment(lib, AUGE_WEB_CORE_NAME _AUGE_FILTER_CONFIGURATION ".lib")
#		pragma message("Automatically linking with " AUGE_WEB_CORE_NAME _AUGE_FILTER_CONFIGURATION ".dll")
#		undef _AUGE_FILTER_CONFIGURATION
#		undef AUGE_WEB_CORE_NAME
#	endif // AUGE_FILTER_EXPORTS 
#endif
//==============================================================================

#endif //__AUGE_WEB_CORE_H__
