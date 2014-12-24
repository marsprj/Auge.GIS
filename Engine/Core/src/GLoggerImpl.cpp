#include "GLoggerImpl.h"

#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/stringhelper.h>

using namespace log4cplus;
using namespace log4cplus::helpers;

namespace auge
{
	GLogger* augeGetLoggerInstance()
	{
		static GLoggerImpl g_logger;
		return &g_logger;
	}
	
	GLoggerImpl::GLoggerImpl()
	{

	}

	GLoggerImpl::~GLoggerImpl()
	{

	}
	
	void GLoggerImpl::Debug(const g_char* info, const g_char* filename, g_uint line)
	{
		if(!m_IsInitialized)
		{
			Initialize();
		}
		Logger::getRoot().log(DEBUG_LOG_LEVEL,info, filename, line);
	}

	void GLoggerImpl::Trace(const g_char* info, const g_char* filename, g_uint line)
	{
		if(!m_IsInitialized)
		{
			Initialize();
		}
		Logger::getRoot().log(TRACE_LOG_LEVEL,info, filename, line);
	}

	void GLoggerImpl::Info (const g_char* info, const g_char* filename, g_uint line)
	{
		if(!m_IsInitialized)
		{
			Initialize();
		}
		Logger::getRoot().log(INFO_LOG_LEVEL,info, filename, line);
	}

	void GLoggerImpl::Warn (const g_char* info, const g_char* filename, g_uint line)
	{
		if(!m_IsInitialized)
		{
			Initialize();
		}
		Logger::getRoot().log(WARN_LOG_LEVEL,info, filename, line);
	}

	void GLoggerImpl::Error(const g_char* info, const g_char* filename, g_uint line)
	{
		if(!m_IsInitialized)
		{
			Initialize();
		}
		Logger::getRoot().log(ERROR_LOG_LEVEL,info, filename, line);
	}

	void GLoggerImpl::Fatal(const g_char* info, const g_char* filename, g_uint line)
	{
		if(!m_IsInitialized)
		{
			Initialize();
		}
		Logger::getRoot().log(FATAL_LOG_LEVEL,info, filename, line);
	}

	//////////////////////////////////////////////////////////////////////////

	bool GLoggerImpl::Initialize(const g_char* config_path/*=NULL*/)
	{
		//日志文件路径
		g_char szloggerPath[AUGE_PATH_MAX];
		memset(szloggerPath, 0, AUGE_PATH_MAX);
	
		try
		{
			//配置文件路径
			g_char szConfigPath[AUGE_PATH_MAX];
			memset(szConfigPath, 0, AUGE_PATH_MAX);
		
			//当前工作路径
			g_char szcwd[AUGE_PATH_MAX];
			memset(szcwd, 0, AUGE_PATH_MAX);
		
			//日志路径
 			g_char szlogDir[AUGE_PATH_MAX];
	// 		g_char szDir[AUGE_PATH_MAX];//路径,以及配置文件路径
	#ifdef _WIN32           
			g_char szDrv[_MAX_DRIVE];//驱动
			g_char szDir[_MAX_DIR];//路径,以及配置文件路径
		
			if(config_path==NULL)
			{
				::GetModuleFileName(NULL, szcwd, AUGE_PATH_MAX);
				_splitpath(szcwd, szDrv, szDir, NULL, NULL);
			
				strcpy(szlogDir,szDir);
				strcat(szlogDir,"log\\");
			
				strcat(szDir,"conf\\");
			
				//_makepath(szloggerPath, szDrv, szlogDir, "aglog.log", NULL);
				_makepath(szConfigPath, szDrv, szDir, "aglogger.properties", NULL);
			}
			else
			{
				_makepath(szConfigPath, NULL, config_path, "aglogger.properties", NULL);
			}
	#else
			/*              getcwd(szcwd, AUGE_PATH_MAX);
			_splitpath(szcwd, szDrv, szDir, NULL, NULL);
		
			  strcpy(szlogDir,szDir);
			  strcat(szlogDir,"log/");
			  strcat(szDir,"conf/");
			*/
			sprintf(szloggerPath,"./log/aglog.log");
			sprintf(szConfigPath, "./conf/aglogger.properties");
	#endif

			PropertyConfigurator::doConfigure(szConfigPath);
		}
		catch (...)
		{
			printf("注意：您的配置文件有问题，将使用默认配置!\n");
			std::string logfile = "aglog.log";
			SharedAppenderPtr _append(new FileAppender(logfile,std::ios::app));
			_append->setName("file_aglog");
		
			std::string strPattern = "%D{%Y-%m-%d %H:%M:%S} - %p - %m [%l]%n";
			std::auto_ptr<Layout> _layout(new PatternLayout(strPattern));
		
			_append->setLayout(_layout);
			Logger::getRoot().addAppender(_append);
			Logger::getRoot().setLogLevel(INFO_LOG_LEVEL);
		}
	
		m_IsInitialized = true;
		return true;
	}

	bool GLoggerImpl::IsInitialized()
	{
		return m_IsInitialized;
	}

}
