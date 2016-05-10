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

#include "AugeCore.h"
#include "AugeWebEngine.h"
//#include "AugeData.h"

namespace auge
{
	typedef enum
	{
		augeHttpGet	= 0,
		augeHttpPost
	}augeHttpMethodType;

	typedef enum
	{
		augeStateRunning	= 0,
		augeStateFinished	= 1,
		augeStateCanceled	= 2,
		augeStateUnknown	= 3
	}augeProcssState;

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
		virtual RESULTCODE		WriteHead(const char* content_type, bool gzip=false) = 0;
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
		virtual const char*		GetThumbnailPath() = 0;
		virtual const char*		GetUploadPath() = 0;
		virtual const char*		GetUserRoot() = 0;
		virtual RESULTCODE		GetUserPath(const char* user, char* user_path, g_uint size) = 0;

		virtual const char*		GetServer() = 0;
		virtual const char*		GetPort() = 0;
		virtual const char*		GetScrpitName() = 0;
		virtual const char*		GetServerSoftware() = 0;
		virtual const char*		GetHttpUserAgent() = 0;
		virtual const char*		GetHttpClient() = 0;
		virtual const char*		GetRequestMethod() = 0;
		virtual const char*		GetQueryString() = 0;

		virtual const char*		GetResponseEncoding() = 0;

		virtual bool			IsIE() = 0;
		virtual	const char*		ParameterEncoding(const char* value) = 0;

		virtual RESULTCODE		RecordUserRequest(const char* user_name, const char* user_request, const char* request_service, const char* http_request) = 0;

	public:
		virtual GConnection*	GetConnection() = 0;

	public:
		virtual	RESULTCODE		Initialize(const char* config_path=NULL) = 0;
		virtual RESULTCODE		Unload() = 0;
	};


	class Job : public GObject
	{
	protected:
		Job(){}
		virtual ~Job(){}
	public:
		virtual const char* GetUUID() = 0;

		virtual const char* GetUser() = 0;
		virtual const char* GetClient() = 0;
		virtual const char* GetServer() = 0;

		virtual const char*	GetOperation() = 0;	
		virtual const char*	GetParams() = 0;
		virtual const char* GetService() = 0;
		virtual bool		GetStartTime(TIME_STRU* time) = 0;
		virtual bool		GetEndTime(TIME_STRU* time) = 0;
		virtual	bool		IsFinished() = 0;
		virtual augeProcssState GetState() = 0;
	};

	class EnumJob
	{
	protected:
		EnumJob(){}
		virtual ~EnumJob(){}
	public:
		virtual Job*		Next() = 0;
		virtual void		Reset() = 0;
		virtual void		Release() = 0; 
	};

	class JobManager
	{
	protected:
		JobManager(){}
		virtual ~JobManager(){}
	public:
		virtual	Job*			AddJob(g_uint user, const char* service, const char* operation, const char* params, const char* client, const char* server) = 0;
		virtual	Job*			AddJob(const char* user, const char* service, const char* operation, const char* params, const char* client, const char* server) = 0;
		virtual RESULTCODE		SetEndTime(const char* uuid) = 0;

		virtual Job*			GetJob(const char* uuid) = 0;
		virtual EnumJob*		GetJob(User* pUser, augeProcssState state ,g_int maxJobs=20, g_uint offset=0) = 0;

		virtual GStatistics*	Statistics(const char* field) = 0;
		virtual GStatistics*	Statistics(const char* field, const char* start_time, const char* end_time) = 0;
		
		virtual GStatistics*	Statistics(g_int user_id, const char* field) = 0;
		virtual GStatistics*	Statistics(g_int user_id, const char* field, const char* start_time, const char* end_time) = 0;

		virtual GStatistics*	StatisticsByServer(g_int user_id, const char* server, const char* field) = 0;
		virtual GStatistics*	StatisticsByServer(g_int user_id, const char* server, const char* field, const char* start_time, const char* end_time) = 0;

		virtual RESULTCODE		Initialize(GConnection* pConnection) = 0;
		virtual RESULTCODE		Unload() = 0;
	};

	class SubscriptionTheme : public GObject
	{
	public:
		SubscriptionTheme(){}
		virtual ~SubscriptionTheme(){}
	public:
		virtual const char*			GetTheme() = 0;
		virtual const char*			GetKeyword(g_uint index) = 0;
		virtual g_uint				GetKeywordCount() = 0;

	};

	class EnumSubscriptionTheme : public GObject
	{
	public:
		EnumSubscriptionTheme(){}
		virtual ~EnumSubscriptionTheme(){}
	public:
		virtual bool				IsEOF() = 0;
		virtual void				Reset() = 0;
		virtual SubscriptionTheme*	Next() = 0;
	};

	class SubscriptionManager
	{
	public:
		SubscriptionManager(){}
		virtual ~SubscriptionManager(){}
	public:
		virtual bool				IsSubscribed(g_uint user, const char* theme, const char* keyword) = 0;
		virtual RESULTCODE			Subscribe(g_uint user, const char* theme, const char* keyword) = 0;
		virtual RESULTCODE			Unsubscribe(g_uint user, const char* theme, const char* keyword) = 0;
		virtual EnumSubscriptionTheme*	GetThemes(g_uint user) = 0;
		virtual SubscriptionTheme*		GetTheme(g_uint user, const char* theme) = 0;

		virtual RESULTCODE			Initialize(GConnection* pConnection) = 0;
		virtual RESULTCODE			Unload() = 0;
	};


	extern "C"
	{
		AUGE_WEB_CORE_API WebWriter*			augeCreateWebWriter();
		AUGE_WEB_CORE_API WebSuccessResponse*	augeCreateWebSuccessResponse();
		AUGE_WEB_CORE_API WebExceptionResponse*	augeCreateWebExceptionResponse();
		AUGE_WEB_CORE_API WebContext*			augeGetWebContextInstance();
		AUGE_WEB_CORE_API JobManager*			augeGetJobManagerInstance();
		AUGE_WEB_CORE_API SubscriptionManager*  augeGetSubscriptionManagerInstance();
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
