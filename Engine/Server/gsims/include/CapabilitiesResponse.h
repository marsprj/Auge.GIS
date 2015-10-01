#ifndef __AUGE_IMS_CAPABILITIES_RESPONSE_H__
#define __AUGE_IMS_CAPABILITIES_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class User;
	class CapabilitiesRequest;

	class CapabilitiesResponse : public WebResponse
	{
	public:
		CapabilitiesResponse(CapabilitiesRequest* pRequest);
		virtual ~CapabilitiesResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetPath(const char* path);
		void	SetUser(User* pUser);

	private:
		User*	m_pUser;
		CapabilitiesRequest*	m_pRequest;
		std::string				m_path;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__

