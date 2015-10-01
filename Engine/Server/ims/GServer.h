#ifndef __AUGE_GSERVER_H__
#define __AUGE_GSERVER_H__

#include <fcgi_stdio.h>
#include <fcgi_config.h>
#include <rude/cgi.h>

#include "AugeCore.h"
#include "AugeWebCore.h"

namespace auge
{
	class GLogger;
	class DataEngineManager;
	class WebEngineManager;
	class WebContext;
	class GConnection;
	class ConnectionManager; 
	class SymbolManager;
	class CartoManager;
	class ServiceManager;
	class UserManager;
	class WebRequest;
	class WebResponse;
	class Service;
	class User;

	//typedef enum
	//{
	//	augeHttpGet	= 0,
	//	augeHttpPost
	//}augeHttpMethodType;

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

		RESULTCODE	LoadUserManager();

		RESULTCODE	LoadJobManager();
		RESULTCODE	LoadServerConfig();
		RESULTCODE	OpenServerBase();
		
		RESULTCODE	LoadConnectionPool();
		RESULTCODE	LoadSymbolManager();
		RESULTCODE	LoadCartoPool();
		RESULTCODE	LoadServicePool();

	private:
		WebResponse*	DoGet(rude::CGI& cgi, User* pUser, Service* pService);
		WebResponse*	DoPost(rude::CGI& cgi, User* pUser, Service* pService);
		//WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);

		augeHttpMethodType	GetMethod();

		void			WriteTime(g_ulong ts, g_ulong te);
		void			DebugCGI(rude::CGI& cgi);

	private:
		GError				*m_pError;
		GLogger				*m_pLogger;
		DataEngineManager	*m_pDataEngineManager;
		WebEngineManager	*m_pWebEngineManager;
		WebContext			*m_pWebContext;

		GConnection			*m_pConnection;
		ConnectionManager	*m_pConnManager;
		SymbolManager		*m_pSymbolManager;
		CartoManager		*m_pCartoManager;
		ServiceManager		*m_pServiceManager;
		JobManager			*m_pJobManager;

		UserManager			*m_pUserManager;

		int					m_counter;
	};
}

#endif //__AUGE_GSERVER_H__
