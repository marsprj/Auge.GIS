#ifndef __AUGE_UFS_LIST_RESPONSE_H__
#define __AUGE_UFS_LIST_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <vector>

namespace auge
{	
	class ListResponse : public WebResponse
	{
	private:
		typedef struct 
		{
			char fname[32];
			int	 isfolder;
		}g_file_t;

	public:
		ListResponse(ListRequest* pRequest);
		virtual ~ListResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	
	public:
		void	AddFile(const char* name, bool isfolder);
	private:
		ListRequest				*m_pRequest;
		std::vector<g_file_t*>	m_files;	
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
