#ifndef __AUGE_IMS_SERVICE_GET_RESPONSE_H__
#define __AUGE_IMS_SERVICE_GET_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class Map;
	class Service;
	class DescribeServiceRequest;
	class XElement;

	class DescribeServiceResponse : public WebResponse
	{
	public:
		DescribeServiceResponse(DescribeServiceRequest* pRequest);
		virtual ~DescribeServiceResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
		void	SetUser(User* pUser);

	private:
		bool	AddServiceNode(XElement* pxParent, Service* pService);
		bool	AddMapNode(XElement* pxService, Map* pMap);
		
	private:
		User*	m_pUser;
		DescribeServiceRequest *m_pRequest;	
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
