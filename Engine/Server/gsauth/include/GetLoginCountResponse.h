#ifndef __AUGE_IMS_GET_LOGIN_COUNT_RESPONSE_H__
#define __AUGE_IMS_GET_LOGIN_COUNT_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class User;
	class EnumUser;

	class GetLoginCountResponse : public WebResponse
	{
	public:
		GetLoginCountResponse(GetLoginCountRequest* pRequest);
		virtual ~GetLoginCountResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	public:
		void	SetCount(g_uint count);
	private:
		g_uint m_count;
		GetLoginCountRequest *m_pRequest;	
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
