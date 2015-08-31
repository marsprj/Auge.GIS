#ifndef __AUGE_WEB_CONTEXT_IMPL_H__
#define __AUGE_WEB_CONTEXT_IMPL_H__

#include "AugeWebCore.h"
#include <string>

namespace auge
{
	class WebContextImpl : public WebContext
	{
	public:
		WebContextImpl();
		virtual ~WebContextImpl();
	public:
		virtual	const char*		GetService();
		virtual void			SetService(const char* name);
		virtual	const char*		GetURI();
		virtual void			SetURI(const char* uri);

		virtual const char*		GetCachePath();
		virtual const char*		GetCacheMapPath();
		virtual const char*		GetCacheProtocolPath();
		virtual const char*		GetThumbnailPath();
		virtual const char*		GetUploadPath();
		virtual const char*		GetUserRoot();
		virtual RESULTCODE		GetUserPath(const char* user, char* user_path, g_uint size);

		virtual GConnection*	GetConnection();

		virtual const char*		GetServer();
		virtual const char*		GetPort();
		virtual const char*		GetScrpitName();
		virtual const char*		GetServerSoftware();
		virtual const char*		GetHttpUserAgent();
		virtual const char*		GetRequestMethod();
		virtual const char*		GetQueryString();

		virtual const char*		GetResponseEncoding();

		virtual bool			IsIE();
		virtual	const char*		ParameterEncoding(const char* value);

	public:
		virtual	RESULTCODE		Initialize(const char* config_path=NULL);		
		virtual RESULTCODE		Unload();

	private:
		RESULTCODE				LoadConfig(const char* config_path);
		void					MakeConfigPath();
		void					InitializeCache();

	private:
		std::string m_path;
		std::string m_name;
		std::string m_uri;

		std::string m_cache_path;
		std::string m_cache_map_path;
		std::string m_cache_protocol_path;
		std::string m_cache_thumbnail_path;
		std::string m_upload_path;
		std::string m_user_root;

		std::string m_data_engine;
		std::string m_data_uri;

		std::string m_response_encoding;

		GConnection	*m_pConnection;
	};
}

#endif //__AUGE_WEB_CONTEXT_IMPL_H__
