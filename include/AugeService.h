#ifndef __AUGE_WEB_SERVICE_H__
#define __AUGE_WEB_SERVICE_H__

#include "AugeWebCore.h"

#ifdef WIN32
#	ifdef AUGE_SERVICE_EXPORTS
#		define AUGE_SERVICE_API __declspec(dllexport)
#	else
#		define AUGE_SERVICE_API __declspec(dllimport)
#	endif
#else
#	define AUGE_SERVICE_API
#endif

namespace auge
{
	class WebRequest;
	class WebResponse;
	class WebWriter;
	class Map;

	typedef enum
	{
		augeStateRun	 = 0,
		augeStateStop		,
		augeStatePause
	}augeServiceState;

	class Service : public GObject
	{
	protected:
		Service(){}
		virtual ~Service(){}
	public:
		virtual const char*		GetName() = 0;
		virtual const char*		GetURI() = 0;
		virtual WebResponse*		Execute(WebRequest* pRequest) = 0;

		virtual void			SetMap(Map* pMap) = 0;
		virtual Map*			GetMap() = 0;

		virtual WebContext*		GetWebContext() = 0;

		virtual	RESULTCODE		Start() = 0;
		virtual RESULTCODE		Shutdown() = 0;
		virtual	RESULTCODE		Pause() = 0;
		virtual RESULTCODE		Resume() = 0;
	};

	class EnumService : public GObject
	{
	protected:
		EnumService(){}
		virtual ~EnumService(){}
	public:
		virtual void			Reset() = 0;
		virtual	Service*		Next() = 0;
	};

	class ServiceManager
	{
	protected:
		ServiceManager(){}
		virtual ~ServiceManager(){}
	public:
		virtual g_uint			GetCount() = 0;
		virtual Service*		GetService(const char* szName) = 0;
		virtual EnumService*	GetServices() = 0;

		virtual RESULTCODE		Register(const char* szName, const char* szURI) = 0;
		virtual RESULTCODE		Unregister(const char* szName) = 0;

		virtual RESULTCODE		RegisterMap(g_uint s_id, g_uint m_id) = 0;

	public:
		virtual RESULTCODE		Initialize(GConnection* pConnection) = 0;
		virtual RESULTCODE		Unload() = 0;
	};

	class WebEngineManager
	{
	protected:
		WebEngineManager(){}
		virtual ~WebEngineManager(){}
	public:
		virtual g_uint			GetCount() = 0;
		virtual WebEngine*		GetEngine(const char* id) = 0;

		virtual WebEngine*		Register(const char* path) = 0;
		virtual RESULTCODE		Unregister(const char* id) = 0;

	public:
		virtual	RESULTCODE		Load(const char* config_path=NULL) = 0;
		virtual void			Unload() = 0;
	};

	extern "C"
	{
		AUGE_SERVICE_API ServiceManager*		augeGetServiceManagerInstance();
		AUGE_SERVICE_API WebEngineManager*		augeGetWebEngineManagerInstance();
	}
}

//==============================================================================
//------------------------------------------------------------------------------
// 连接所需动态库
//------------------------------------------------------------------------------
#ifdef WIN32
#	ifndef AUGE_SERVICE_EXPORTS
#		define AUGE_SERVICE_NAME "gsvrs"
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
#		pragma comment(lib, AUGE_SERVICE_NAME _AUGE_FILTER_CONFIGURATION ".lib")
#		pragma message("Automatically linking with " AUGE_SERVICE_NAME _AUGE_FILTER_CONFIGURATION ".dll")
#		undef _AUGE_FILTER_CONFIGURATION
#		undef AUGE_SERVICE_NAME
#	endif // AUGE_FILTER_EXPORTS 
#endif
//==============================================================================

#endif //__AUGE_WEB_SERVICE_H__
