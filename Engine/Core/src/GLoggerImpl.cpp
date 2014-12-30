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
		char log_path[AUGE_PATH_MAX] = {0};
	
		try
		{			
			char cdir[AUGE_PATH_MAX] = {0};
			auge_get_cwd(cdir, AUGE_PATH_MAX);

#ifdef WIN32
			auge_make_path(log_path, NULL, cdir, "conf\\glogger", "properties");
#else
			char pdir[AUGE_PATH_MAX] = {0};
			auge_get_parent_dir(cdir, pdir, AUGE_PATH_MAX);
			auge_make_path(log_path, NULL, pdir, "conf/data_engine", "xml");
#endif

			PropertyConfigurator::doConfigure(log_path);
		}
		catch (...)
		{
			printf("ע�⣺���������ļ������⣬��ʹ��Ĭ������!\n");
			std::string logfile = "auge.log";
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

	//bool GLoggerImpl::Initialize(const g_char* config_path/*=NULL*/)
	//{
	//	//��־�ļ�·��
	//	g_char szloggerPath[AUGE_PATH_MAX];
	//	memset(szloggerPath, 0, AUGE_PATH_MAX);
	//
	//	try
	//	{
	//		//�����ļ�·��
	//		g_char szConfigPath[AUGE_PATH_MAX];
	//		memset(szConfigPath, 0, AUGE_PATH_MAX);
	//	
	//		//��ǰ����·��
	//		g_char szcwd[AUGE_PATH_MAX];
	//		memset(szcwd, 0, AUGE_PATH_MAX);
	//	
	//		//��־·��
 //			g_char szlogDir[AUGE_PATH_MAX];
	//// 		g_char szDir[AUGE_PATH_MAX];//·��,�Լ������ļ�·��
	//#ifdef _WIN32           
	//		g_char szDrv[_MAX_DRIVE];//����
	//		g_char szDir[_MAX_DIR];//·��,�Լ������ļ�·��
	//	
	//		if(config_path==NULL)
	//		{
	//			::GetModuleFileName(NULL, szcwd, AUGE_PATH_MAX);
	//			_splitpath(szcwd, szDrv, szDir, NULL, NULL);
	//		
	//			strcpy(szlogDir,szDir);
	//			strcat(szlogDir,"log\\");
	//		
	//			strcat(szDir,"conf\\");
	//		
	//			//_makepath(szloggerPath, szDrv, szlogDir, "aglog.log", NULL);
	//			_makepath(szConfigPath, szDrv, szDir, "aglogger.properties", NULL);
	//		}
	//		else
	//		{
	//			_makepath(szConfigPath, NULL, config_path, "aglogger.properties", NULL);
	//		}
	//#else
	//		/*              getcwd(szcwd, AUGE_PATH_MAX);
	//		_splitpath(szcwd, szDrv, szDir, NULL, NULL);
	//	
	//		  strcpy(szlogDir,szDir);
	//		  strcat(szlogDir,"log/");
	//		  strcat(szDir,"conf/");
	//		*/
	//		sprintf(szloggerPath,"./log/aglog.log");
	//		sprintf(szConfigPath, "./conf/aglogger.properties");
	//#endif

	//		PropertyConfigurator::doConfigure(szConfigPath);
	//	}
	//	catch (...)
	//	{
	//		printf("ע�⣺���������ļ������⣬��ʹ��Ĭ������!\n");
	//		std::string logfile = "aglog.log";
	//		SharedAppenderPtr _append(new FileAppender(logfile,std::ios::app));
	//		_append->setName("file_aglog");
	//	
	//		std::string strPattern = "%D{%Y-%m-%d %H:%M:%S} - %p - %m [%l]%n";
	//		std::auto_ptr<Layout> _layout(new PatternLayout(strPattern));
	//	
	//		_append->setLayout(_layout);
	//		Logger::getRoot().addAppender(_append);
	//		Logger::getRoot().setLogLevel(INFO_LOG_LEVEL);
	//	}
	//
	//	m_IsInitialized = true;
	//	return true;
	//}


	bool GLoggerImpl::IsInitialized()
	{
		return m_IsInitialized;
	}

}
