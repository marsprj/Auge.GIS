#ifndef __AUGE_IMS_DATASOURCE_GET_RESPONSE_H__
#define __AUGE_IMS_DATASOURCE_GET_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class User;
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

	public:
		void	SetUser(User* pUser);

	private:
		void	AddDataSourceNode(XElement* pxRoot, Workspace* pWorkspace);
		void	AddDataSourceNode(XDocument* pxDoc, Workspace* pWorkspace);

	private:
		GetDataSourceRequest *m_pRequest;
		User*	m_pUser;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
