#ifndef __AUGE_RDS_GET_VALUE_RESPONSE_H__
#define __AUGE_RDS_GET_VALUE_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <vector>
#include <string>


namespace auge
{	
	class GetValueResponse : public WebResponse
	{
	public:
		GetValueResponse(GetValueRequest* pRequest);
		virtual ~GetValueResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	
	public:
		void	AddFile(const char* name);
		void	SetPath(const char* path);

	private:
		GetValueRequest		*m_pRequest;
		std::string				m_path;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
