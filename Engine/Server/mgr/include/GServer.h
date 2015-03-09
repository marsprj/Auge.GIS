#ifndef __AUGE_GSERVER_H__
#define __AUGE_GSERVER_H__

#include <fcgi_stdio.h>
#include <fcgi_config.h>
#include <rude/cgi.h>

#include <AugeCore.h>

namespace auge
{
	class GLogger;
	class DataEngineManager;
	class WebEngineManager;
	class WebContext;
	class GConnection;
	class ConnectionManager;
	class CartoManager;
	class ServiceManager;
	class WebRequest;
	class WebResponse;

	typedef enum
	{
		augeHttpGet	= 0,
		augeHttpPost
	}augeHttpMethodType;

	class GServer
	{
	public:
		GServer();
		virtual ~GServer();
	public:
		bool	Startup();
		void	Shutdown();
		void	Run();

	private:
		//===========================================================================
		// Startup Functions
		//===========================================================================
		RESULTCODE	LoadConfig();
		RESULTCODE	LoadLogger();

		RESULTCODE	LoadDataEngine();
		RESULTCODE	LoadServiceEngine();

		RESULTCODE	LoadServerConfig();
		RESULTCODE	OpenServerBase();
		
		RESULTCODE	LoadConnectionPool();
		RESULTCODE	LoadCartoPool();
		RESULTCODE	LoadServicePool();

	private:
		WebResponse* DoGet(rude::CGI& cgi);
		WebResponse* DoPost(rude::CGI& cgi);
		WebResponse* Execute(WebRequest* pWebRequest);

		augeHttpMethodType	GetMethod();
		
	private:
		GError				*m_pError;
		GLogger				*m_pLogger;
		DataEngineManager	*m_pDataEngineManager;
		WebEngineManager	*m_pWebEngineManager;
		WebContext			*m_pWebContext;

		GConnection			*m_pConnection;
		ConnectionManager	*m_pConnManager;
		CartoManager		*m_pCartoManager;
		ServiceManager		*m_pServiceManager;

		int					m_counter;
	};
}

#endif //__AUGE_GSERVER_H__
