#ifndef __AUGE_WPS_DESCRIBE_PROCESS_RESPONSE_H__
#define __AUGE_WPS_DESCRIBE_PROCESS_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class DescribeProcessRequest;

	class DescribeProcessResponse : public WebResponse
	{
	public:
		DescribeProcessResponse(DescribeProcessRequest* pRequest);
		virtual ~DescribeProcessResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetPath(const char* path);

	private:
		DescribeProcessRequest*	m_pRequest;
		std::string				m_path;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__

