#ifndef __AUGE_IMS_GET_USER_COUNT_RESPONSE_H__
#define __AUGE_IMS_GET_USER_COUNT_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class User;
	class EnumUser;

	class GetUserCountResponse : public WebResponse
	{
	public:
		GetUserCountResponse(GetUserCountRequest* pRequest);
		virtual ~GetUserCountResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	public:
		void	SetCount(g_uint count);
	private:
		g_uint m_count;
		GetUserCountRequest *m_pRequest;	
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
