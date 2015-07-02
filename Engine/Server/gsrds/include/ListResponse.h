#ifndef __AUGE_RDS_LIST_RESPONSE_H__
#define __AUGE_RDS_LIST_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <vector>
#include <string>

#ifdef WIN32
#	include <io.h>
#include <sys\stat.h>
#endif

namespace auge
{	
	class ListResponse : public WebResponse
	{
	private:
		typedef struct 
		{
			char fname[32];
			int	 isfolder;
#ifdef WIN32
			struct _stat fstat;
#else
			struct stat fstat;
#endif
		}g_file_t;

	public:
		ListResponse(ListRequest* pRequest);
		virtual ~ListResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	
	public:
		void	AddFile(const char* name);
		void	SetPath(const char* path);

	private:
		ListRequest				*m_pRequest;
		std::vector<g_file_t*>	m_files;	
		std::string				m_path;
	};
}

#endif //__AUGE_RDS_LIST_RESPONSE_H__