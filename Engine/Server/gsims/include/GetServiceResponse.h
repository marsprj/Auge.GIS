#ifndef __AUGE_IMS_SERVICE_GET_RESPONSE_H__
#define __AUGE_IMS_SERVICE_GET_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class Map;
	class Service;
	class GetServiceRequest;
	class XElement;

	class GetServiceResponse : public WebResponse
	{
	public:
		GetServiceResponse(GetServiceRequest* pRequest);
		virtual ~GetServiceResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
		
	private:
		bool	AddServiceNode(XElement* pxParent, Service* pService);
		bool	AddMapNode(XElement* pxService, Map* pMap);

	private:
		GetServiceRequest *m_pRequest;	
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
