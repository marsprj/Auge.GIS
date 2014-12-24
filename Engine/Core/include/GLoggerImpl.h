#ifndef __AUGE_LOGGER_IMPL_H__
#define __AUGE_LOGGER_IMPL_H__

#include "AugeCore.h"

namespace auge
{
	class GLoggerImpl : public GLogger
	{
	public:
		GLoggerImpl();
		virtual ~GLoggerImpl();
	public:
		virtual void	Trace(const g_char* info, const g_char* fname=NULL, g_uint line=0);
		virtual void	Debug(const g_char* info, const g_char* fname=NULL, g_uint line=0);
		virtual void	Info (const g_char* info, const g_char* fname=NULL, g_uint line=0);
		virtual void	Warn (const g_char* info, const g_char* fname=NULL, g_uint line=0);
		virtual void	Error(const g_char* info, const g_char* fname=NULL, g_uint line=0);
		virtual void	Fatal(const g_char* info, const g_char* fname=NULL, g_uint line=0);

		virtual bool	Initialize(const g_char* config_path=NULL);
		virtual bool	IsInitialized();
	private:
		bool	m_IsInitialized;
	};
}

#endif //__AUGE_LOGGER_IMPL_H__
