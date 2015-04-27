#ifndef __AUGE_IMS_DATASOURCE_GET_RESPONSE_H__
#define __AUGE_IMS_DATASOURCE_GET_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class Service;
	class Workspace;
	class GetDataSourceRequest;

	class GetDataSourceResponse : public WebResponse
	{
	public:
		GetDataSourceResponse(GetDataSourceRequest* pRequest);
		virtual ~GetDataSourceResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	private:
		void	AddDataSourceNode(XElement* pxRoot, Workspace* pWorkspace);

	private:
		GetDataSourceRequest *m_pRequest;	
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
