#ifndef __AUGE_IMS_GET_ONELINE_USER_RESPONSE_H__
#define __AUGE_IMS_GET_ONELINE_USER_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class User;
	class EnumUser;

	class GetOnlineUserResponse : public WebResponse
	{
	public:
		GetOnlineUserResponse(GetOnlineUserRequest* pRequest);
		virtual ~GetOnlineUserResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	public:
		void		SetUsers(EnumUser* pUsers);

		RESULTCODE	Write(WebWriter* pWriter, EnumUser* pUsers);

	private:
		EnumUser* m_pUsers;
		GetOnlineUserRequest *m_pRequest;	
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
