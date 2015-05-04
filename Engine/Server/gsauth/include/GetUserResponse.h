#ifndef __AUGE_IMS_GET_USER_RESPONSE_H__
#define __AUGE_IMS_GET_USER_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class User;
	class EnumUser;

	class GetUserResponse : public WebResponse
	{
	public:
		GetUserResponse(GetUserRequest* pRequest);
		virtual ~GetUserResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	public:
		void		SetUsers(EnumUser* pUsers);
		void		SetUser(User* pUser);

		RESULTCODE	Write(WebWriter* pWriter, EnumUser* pUsers);
		RESULTCODE	Write(WebWriter* pWriter, User* pUser);

	private:
		User* m_pUser;
		EnumUser* m_pUsers;
		GetUserRequest *m_pRequest;	
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
